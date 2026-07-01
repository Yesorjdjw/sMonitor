#include "fullShow.h"
#include "ui_fullShow.h"
#include <QShowEvent>
#include <QTimer>
#include <QDebug>



fullShow::fullShow(QWidget *parent) :
    QMainWindow(parent),                // 父窗口通常是 fileIndex
    ui(new Ui::fullShow),               // 加载 fullShow.ui 布局
    m_process(nullptr),                 // 视频播放进程指针，初始无进程
    m_isClosing(false)                  // 退出标志，防重复触发
{
    ui->setupUi(this);                  // 解析 .ui 文件，创建所有子控件

    // ---- 窗口属性 ----
    setWindowFlags(Qt::FramelessWindowHint);  // 无标题栏、无边框
    setGeometry(0, 0, 1024, 600);             // 全屏覆盖（开发板 LCD 分辨率）
    setFocusPolicy(Qt::StrongFocus);          // 接受键盘焦点（Esc 键）
    setMouseTracking(true);                   // 追踪鼠标移动（本项目中未使用，预留）

    // ---- 双击退出（通过 click_lable 自定义信号） ----
    // 当用户双击 label 区域 → click_lable::mouseDoubleClickEvent()
    //   → emit doubleclicked() → 这里收到 → on_label_doubleclicked() → closeShow()
    connect(ui->label, SIGNAL(doubleclicked()),
            this, SLOT(on_label_doubleclicked()));

    // ---- 透明覆盖层 — 事件劫持 ----
    // 为什么需要覆盖层？
    //   在 ARM 上 mplayer 直接写 framebuffer，会覆盖 Qt 的绘图。
    //   覆盖层放在 label 之上，所有鼠标/触摸事件先经覆盖层 → eventFilter → closeShow()
    //   这样即使 label 被 mplayer 画面盖住，仍然能响应双击/单击退出。
    QWidget *overlay = new QWidget(ui->label);
    overlay->setObjectName(QStringLiteral("clickOverlay"));
    overlay->setGeometry(0, 0, 1024, 600);           // 和窗口一样大
    overlay->setAttribute(Qt::WA_TransparentForMouseEvents, false); // 不穿透事件
    overlay->setStyleSheet("background: transparent;");              // 完全透明
    overlay->show();
    overlay->raise();                                  // 置于最顶层
    overlay->installEventFilter(this);                 // 将事件转发给 eventFilter()
}

// ============================================================================
//  析构函数 — 最后一道防线清理 mplayer 僵尸进程
// ============================================================================
fullShow::~fullShow()
{
    // 如果 closeShow() 中的 kill() 没来得及清理完，这里兜底
    if (m_process && m_process->state() == QProcess::Running) {
        m_process->kill();              // SIGKILL 强制终止
        m_process->waitForFinished(500);// 最多等 500ms 等进程真正结束
    }
    delete ui;                          // 释放 UI 对象
}

// ============================================================================
//  showEvent — 窗口显示时回调
//  用途：延迟加载视频（等窗口完全显示后再启动 mplayer）
// ============================================================================
void fullShow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);

    // 检查是否有 pending 视频（show_video() 只是保存路径，等 show 时才播放）
    if (!m_pendingVideo.isEmpty()) {
        if (!QFile::exists(m_pendingVideo)) {
            qWarning("fullShow: video not found: %s", qPrintable(m_pendingVideo));
            close();                     // 文件不存在直接关闭
            return;
        }
        play_video(m_pendingVideo);      // 真正启动 mplayer
        m_pendingVideo.clear();          // 清空，防止重复播放
    }
}

// ============================================================================
//  show_picture — 显示一张图片
//  参数：QPixmap 对象（由 fileIndex 加载好传入）
// ============================================================================
void fullShow::show_picture(const QPixmap &pic_name)
{
    // KeepAspectRatio = 保持原始宽高比，留黑边
    // SmoothTransformation = 双线性插值，缩放质量好但稍慢
    ui->label->setPixmap(
        pic_name.scaled(1024, 600,
                        Qt::KeepAspectRatio,
                        Qt::SmoothTransformation));
    ui->label->setAlignment(Qt::AlignCenter);  // 居中显示
}

// ============================================================================
//  show_video — "准备"播放视频（仅保存路径，真正播放在 showEvent 中）
//  这样调用方可以先 show() 窗口，再播放，避免黑屏期
// ============================================================================
void fullShow::show_video(const QString &vid_name)
{
    m_pendingVideo = vid_name;  // 暂存路径
}

// ============================================================================
//  play_video — 用 mplayer 播放视频
//  返回 true 表示启动成功
// ============================================================================
bool fullShow::play_video(const QString &vid_name)
{
    if (!QFile::exists(vid_name)) {
        return false;
    }

    // ---- 防重入：如果已有播放进程，先杀掉 ----
    if (m_process) {
        m_process->kill();
        m_process->waitForFinished(500);
        delete m_process;              // 删除旧的 QProcess 对象
    }

    // winId() 确保 label 的 native 窗口句柄已创建（mplayer嵌入需要）
    ui->label->winId();
    m_process = new QProcess(this);    // 创建新进程管理器

#ifdef __arm__
    // ======== ARM 嵌入式平台 ========
    // 开发板无 X11 服务器，mplayer 直接写 /dev/fb0（framebuffer）
    // -geometry 0:0  = 从屏幕左上角开始
    // -zoom -x 1024 -y 600 = 缩放到全屏
    QString cmd = QString("mplayer -quiet -geometry 0:0 ")
                      + QString("-zoom -x 1024 -y 600 ")
                      + vid_name;
    m_process->start(cmd);            // 启动 mplayer，不等待返回
#else
    // ======== PC/Linux 桌面 ========
    // mplayer 作为子进程嵌入到 QLabel 的 native 窗口中
    // -slave  = 接受标准输入命令（如 quit）
    // -wid    = 指定父窗口 ID，mplayer 画在 label 内
    // -vo x11 = 使用 X11 视频输出
    QStringList args;
    args << "-slave" << "-quiet"
         << "-wid" << QString::number((unsigned long)ui->label->winId())
         << "-zoom" << "-vo" << "x11"
         << vid_name;
    m_process->start("mplayer", args);
#endif

    // 连接：mplayer 正常结束（播完或用户关闭）→ onProcessFinished()
    connect(m_process, SIGNAL(finished(int)),
            this, SLOT(onProcessFinished()));

    return true;
}

// ============================================================================
//  on_label_doubleclicked — label 双击 → 退出全屏
//  由 click_lable::mouseDoubleClickEvent → doubleclicked() 信号触发
// ============================================================================
void fullShow::on_label_doubleclicked()
{
    closeShow();
}

// ============================================================================
//  onProcessFinished — mplayer 正常结束时的回调
//  区分"用户主动退出"和"视频自然播完"
// ============================================================================
void fullShow::onProcessFinished()
{
    // 如果 m_isClosing = true，说明是 closeShow() 主动杀死的，不重复关闭
    if (!m_isClosing) {
        close();    // 视频播完自动退出全屏
    }
}

// ============================================================================
//  keyPressEvent — 键盘按键处理（触摸屏上一般不触发，预留）
// ============================================================================
void fullShow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        closeShow();  // Esc 键退出全屏
    }
    QMainWindow::keyPressEvent(event);
}

// ============================================================================
//  eventFilter — 事件过滤器
//  捕获覆盖层上的鼠标/触摸事件，实现"点一下退出视频"功能
//
//  触发逻辑：
//    - 图片模式：双击退出
//    - 视频模式：单击或双击都退出（因为 mplayer 占用了 framebuffer，
//      单击更易操作，用户不用等双击检测时间）
// ============================================================================
bool fullShow::eventFilter(QObject *obj, QEvent *event)
{
    // 双击 → 退出（图片和视频都支持）
    if (event->type() == QEvent::MouseButtonDblClick) {
        closeShow();
        return true;      // 事件已处理，不再传播
    }

    // 单击 → 只有视频模式才退出（视频全屏时方便快速退出）
    // 图片模式下单击不做任何事（防止误触）
    if (event->type() == QEvent::MouseButtonPress) {
        if (m_process) {  // 有 m_process = 正在播放视频
            closeShow();
            return true;
        }
    }

    // 其他事件交给父类处理
    return QMainWindow::eventFilter(obj, event);
}

// ============================================================================
//  closeShow — 核心退出逻辑
//
//  设计原则：先隐藏界面给用户反馈，再清理资源。
//  改造前：write("quit\n") + 500ms 看门狗定时器 → 退出至少等 500ms
//  改造后：直接 kill() + waitForFinished(200) → 退出约 200ms，用户无感知
// ============================================================================
void fullShow::closeShow()
{
    // 防重复触发（双击 + 单击 + Esc 同时触发时只进一次）
    if (m_isClosing) return;
    m_isClosing = true;

    // ---- 第1步：立即隐藏窗口 ----
    // 用户看到"瞬间退出了"，体验流畅
    // 后续清理（kill mplayer、释放内存）在后台进行
    this->hide();

    // ---- 第2步：释放图片内存 ----
    // 清除 label 上的 QPixmap，及时归还内存（尤其是大图片）
    ui->label->clear();

    // ---- 第3步：如果正在播放视频，强杀 mplayer 进程 ----
    if (m_process && m_process->state() == QProcess::Running) {
        // 直接 SIGKILL，不等 mplayer 自己退出（原 500ms 看门狗太慢）
        m_process->kill();
        // 等最多 200ms，确保进程已死亡，释放 framebuffer
        m_process->waitForFinished(200);
    }

    // ---- 第4步：发送关闭事件 ----
    // 由于设置了 WA_DeleteOnClose，close() 会触发 deleteLater()
    // 最终调用 ~fullShow() 释放所有资源
    this->close();
}
