#include "unusual.h"
#include "menu.h"
#include "ui_menu.h"
#include "ConfigManager.h"
#include "fileIndex.h"
#include <QDir>
#include <QDateTime>
#include <QPushButton>
#include <QPixmap>

bool swt_Control_flag = true;

menu::menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::menu),
    m_setup(nullptr),
    m_isRecording(false),
    m_photoCount(0),
    m_videoCount(0),
    m_recSeconds(0)
{
    ui->setupUi(this);

    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/images/menu.jpg")));
    this->setPalette(palette);

    /* 统一初始状态：显示手动控制面板 */
    swt_Control_flag = false;
    ui->swt_Control_bt->setText("切换自动");
    ui->ctl_Panel_sw->setCurrentIndex(0);

    /* UI Init */
    ui->noSignal_lb->show();
    ui->flashOverlay_lb->hide();
    ui->recTime_lb->hide();
    ui->timeWatermark_lb->hide(); // Hide the old software watermark since we now use physical watermark

    /* 录像计时器 */
    m_recTimer = new QTimer(this);// ← 创建定时器对象
    connect(m_recTimer, SIGNAL(timeout()), this, SLOT(updateRecTime()));
 // ↑ 连接信号：定时器超时 → 调用updateRecTime()函数更新时间显示
    /* Worker Thread and Object initialization */
    m_workerThread = new QThread(this);// ← 创建独立工作线程
    m_worker = new VideoWorker();// ← 创建视频处理对象
    m_worker->moveToThread(m_workerThread);// ← 将Worker移到工作线程中运行

    // Connect signals/slots
    connect(m_workerThread, &QThread::started, m_worker, &VideoWorker::startCamera);
    connect(m_workerThread, &QThread::finished, m_worker, &QObject::deleteLater);

    // Cross-thread UI updates
    connect(m_worker, &VideoWorker::frameReady, this, &menu::onFrameReady);
    connect(m_worker, &VideoWorker::photoSaved, this, &menu::onPhotoSaved);
    connect(m_worker, &VideoWorker::recordingStarted, this, &menu::onRecordingStarted);
    connect(m_worker, &VideoWorker::recordingStopped, this, &menu::onRecordingStopped);
    connect(m_worker, &VideoWorker::cameraError, this, &menu::onCameraError);

    m_workerThread->start();//我已启动

    // Check Auto Record from Config Manager right after boot
    if (ConfigManager::instance().autoRecord()) {// ↑ 检查配置文件是否设置了自动录制
        QMetaObject::invokeMethod(m_worker, "setRecording", Qt::QueuedConnection, Q_ARG(bool, true));
    }// ↑ 跨线程调用：在Worker线程中调用 setRecording(true)
}

menu::~menu()
{
    m_recTimer->stop();// ← 停止计时器
    m_workerThread->quit();// ← 信号线程退出
    m_workerThread->wait(); // ← 等待线程真正结束（阻塞）
    if (m_setup) delete m_setup; // ← 若设置窗口存在则删除
    delete ui; // ← 删除UI对象
}

void menu::onFlashHide()
{
    ui->flashOverlay_lb->hide();// ← 隐藏白色闪屏遮罩
}

/* ========== 录像计时 ========== */

void menu::updateRecTime()
{
    m_recSeconds++;
    int min = m_recSeconds / 60;
    int sec = m_recSeconds % 60;
    ui->recTime_lb->setText(QString("%1:%2")
        .arg(min, 2, 10, QLatin1Char('0'))
        .arg(sec, 2, 10, QLatin1Char('0')));
}

/* ========== Worker 信号接收 ========== */

void menu::onFrameReady(const QImage &img)
{
    ui->noSignal_lb->hide();// ← 隐藏"无信号"提示（说明已接收到视频）
    ui->cap_Frame_lb->setPixmap(
        QPixmap::fromImage(img).scaled(
            ui->cap_Frame_lb->size(), Qt::IgnoreAspectRatio));
}// ↑ 将QImage转为QPixmap，缩放到标签大小，并显示

void menu::onCameraError(const QString &msg)
{
    qWarning() << "Camera Error:" << msg;
    ui->noSignal_lb->show();
}

void menu::onPhotoSaved(const QString &path)
{
    qDebug() << "Photo saved:" << path;

    /* 缩略图飞入动画：从视频区中央飞向文件管理按钮 */
    const QPixmap *pix = ui->cap_Frame_lb->pixmap();// ← 获取当前显示的视频帧
    if (!pix) return;

    QLabel *thumb = new QLabel(this);
    thumb->setObjectName("thumb_animation");
    thumb->setFixedSize(80, 60);
    thumb->setPixmap(pix->scaled(
        80, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        // ↑ 将视频帧缩小到80×60，平滑变换
    /* 起点：视频区中央 */
    QPoint startPt(ui->cap_Frame_lb->x() + ui->cap_Frame_lb->width() / 2 - 40,
                   ui->cap_Frame_lb->y() + ui->cap_Frame_lb->height() / 2 - 30);
    thumb->move(startPt); // ← 移动缩略图到起点
    thumb->show();// ← 显示缩略图
    thumb->raise();// ← 提升到最前面（z轴最高）

    /* 终点：文件管理按钮中央 */
    QPoint endPt(ui->file_Manage_bt->x() + ui->file_Manage_bt->width() / 2 - 40,
                 ui->file_Manage_bt->y() + ui->file_Manage_bt->height() / 2 - 30);
// ↑ 计算文件管理按钮的中心位置
    QPropertyAnimation *anim = new QPropertyAnimation(thumb, "pos", this);
    anim->setDuration(500);
    anim->setStartValue(startPt);
    anim->setEndValue(endPt);
    anim->setEasingCurve(QEasingCurve::InCubic);

    connect(anim, &QPropertyAnimation::finished, thumb, &QLabel::deleteLater);

    anim->start(QAbstractAnimation::DeleteWhenStopped);

}

void menu::onRecordingStarted(const QString &path)
{
    m_isRecording = true;
    m_lastVideoPath = path;

     // === 更新按钮样式 ===
    ui->get_vedio_bt->setText(QString::fromUtf8("停止录制"));
    // ↑ 按钮文字改为"停止录制"
    ui->get_vedio_bt->setProperty("recording", "true");
    // ↑ 设置按钮的自定义属性 recording=true
    ui->get_vedio_bt->style()->unpolish(ui->get_vedio_bt);
    ui->get_vedio_bt->style()->polish(ui->get_vedio_bt);
    // ↑ 刷新样式：清除旧样式 → 重新应用新样式

    /* 启动录像计时 */
    m_recSeconds = 0;
    ui->recTime_lb->setText("00:00");
    ui->recTime_lb->show();
    m_recTimer->start(1000);

    qDebug() << "Recording started:" << path;
}

void menu::onRecordingStopped()
{
    m_isRecording = false;

    /* 停止录像计时 */
    m_recTimer->stop();
    ui->recTime_lb->hide();

    ui->get_vedio_bt->setText(QString::fromUtf8("开始录制"));
    ui->get_vedio_bt->setProperty("recording", "false");
    ui->get_vedio_bt->style()->unpolish(ui->get_vedio_bt);
    ui->get_vedio_bt->style()->polish(ui->get_vedio_bt);

    qDebug() << "Recording stopped";
}

/* ========== 拍照触发 ========== */

void menu::on_get_photo_bt_pressed()
{
    // Delegate file IO and capture to worker thread
    QMetaObject::invokeMethod(m_worker, "takePhoto", Qt::QueuedConnection);

    /* 闪屏：白色遮罩闪现 150ms */
    ui->flashOverlay_lb->show();
    QTimer::singleShot(150, this, SLOT(onFlashHide()));
}

/* ========== 录制开始/停止切换========== */

void menu::on_get_vedio_bt_clicked()
{
    if (!m_isRecording) {
        QMetaObject::invokeMethod(m_worker, "setRecording", Qt::QueuedConnection, Q_ARG(bool, true));
    } else {
        QMetaObject::invokeMethod(m_worker, "setRecording", Qt::QueuedConnection, Q_ARG(bool, false));
    }
}

/* ========== 手动/自动模式切换（========== */

void menu::on_swt_Control_bt_clicked()
{
    if (swt_Control_flag) {
        swt_Control_flag = false;
        ui->swt_Control_bt->setText("切换手动");
        ui->ctl_Panel_sw->setCurrentIndex(1);
    } else {
        swt_Control_flag = true;
        ui->swt_Control_bt->setText("切换自动");
        ui->ctl_Panel_sw->setCurrentIndex(0);
    }
}
//导航按钮 - 打开各个功能窗口
void menu::on_exp_Check_bt_clicked()
{
    unusual *un = new unusual();
    un->setAttribute(Qt::WA_DeleteOnClose);
    un->show();
}

void menu::on_file_Manage_bt_clicked()
{
    // 先隐藏主菜单（避免后台摄像头画面在framebuffer上残留）
    this->hide();

    // 直接打开缩略图页（跳过中间文件夹图标页）
    fileIndex *fi = new fileIndex();
    fi->setAttribute(Qt::WA_DeleteOnClose);
    fi->updatePicData("/opt/aicTrain/sMonitor/photo");
    fi->updataVidData("/opt/aicTrain/sMonitor/photo", "/opt/aicTrain/sMonitor/video");

    // fileIndex关闭时恢复主菜单
    QObject::connect(fi, &fileIndex::exitRequested, [this, fi]() {
        fi->close();
    });
    QObject::connect(fi, &QObject::destroyed, [this]() {
        this->show();
    });

    fi->show();
}

void menu::on_mode_Setup_bt_clicked()
{
    if (!m_setup) {
        m_setup = new setup();
        m_setup->setEngPtr(&sEng);
        // Link autoRecord signal up
        connect(m_setup, SIGNAL(autoRecordToggled(bool)), this, SLOT(onAutoRecordToggled(bool)));
    }

    // Ensure the recording state in setup matches actual running state before showing
    m_setup->setRecordingState(m_isRecording);
//导航按钮 - 打开各个功能窗口
    m_setup->show();
    m_setup->raise();
}

/* ========== 自动录像：设置页开关联动主界面录制 ========== */

void menu::onAutoRecordToggled(bool on)
{
    if (on && !m_isRecording) {
        /* 开启自动录像 → 触发录制 */
        on_get_vedio_bt_clicked();
    } else if (!on && m_isRecording) {
        /* 关闭自动录像 → 停止录制 */
        on_get_vedio_bt_clicked();
    }
}

/* ========== PTZ 云台 ========== */

void menu::on_turn_up_bt_pressed()
{
    sEng.turnUp();
}

void menu::on_turn_up_bt_released()
{
    sEng.stop();
}

void menu::on_turn_down_bt_pressed()
{
    sEng.turnDown();
}

void menu::on_turn_down_bt_released()
{
    sEng.stop();
}

void menu::on_turn_left_bt_pressed()
{
    sEng.turnLeft();
}

void menu::on_turn_left_bt_released()
{
    sEng.stop();
}

void menu::on_turn_right_bt_pressed()
{
    sEng.turnRight();
}

void menu::on_turn_right_bt_released()
{
    sEng.stop();
}

void menu::on_turn_reset_bt_clicked()
{
    sEng.backLrpos();
    sEng.backUdpos();
}
