#include "unusual.h"
#include "setup.h"
#include "menu.h"
#include "ui_menu.h"
#include <QDir>
#include <QDateTime>

bool swt_Control_flag = true;

menu::menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::menu),
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

    /* 创建保存目录 */
    QDir().mkpath("/opt/aicTrain/sMonitor/photo");
    QDir().mkpath("/opt/aicTrain/sMonitor/video");

    /* 初始化摄像头 */
    m_capTimer = new QTimer(this);
    connect(m_capTimer, SIGNAL(timeout()), this, SLOT(readFrame()));

    if (m_cap.open(0)) {
        qDebug() << "Camera /dev/video0 opened";
    } else if (m_cap.open("/opt/aicTrain/camCapture/adver.mp4")) {
        qDebug() << "Fallback: playing adver.mp4";
    } else {
        qWarning("menu: no camera source available");
    }

    m_capTimer->start(33);  // ~30 fps

    /* 时间水印定时器：每秒更新 */
    m_clockTimer = new QTimer(this);
    connect(m_clockTimer, SIGNAL(timeout()), this, SLOT(updateClock()));
    m_clockTimer->start(1000);
    updateClock();  // 立即显示初始时间

    /* 录像计时器 */
    m_recTimer = new QTimer(this);
    connect(m_recTimer, SIGNAL(timeout()), this, SLOT(updateRecTime()));

    /* 无信号占位：初始显示 */
    ui->noSignal_lb->show();

    /* 闪屏遮罩：初始隐藏 */
    ui->flashOverlay_lb->hide();

    /* 录像时长：初始隐藏 */
    ui->recTime_lb->hide();
}

menu::~menu()
{
    m_capTimer->stop();
    m_recTimer->stop();
    m_clockTimer->stop();
    if (m_cap.isOpened())
        m_cap.release();
    if (m_writer.isOpened())
        m_writer.release();
    delete ui;
}

/* ========== 时间水印 ========== */

QString menu::formatTimestamp()
{
    QDateTime now = QDateTime::currentDateTime();
    QStringList weekDays;
    weekDays << QString::fromUtf8("星期一")
             << QString::fromUtf8("星期二")
             << QString::fromUtf8("星期三")
             << QString::fromUtf8("星期四")
             << QString::fromUtf8("星期五")
             << QString::fromUtf8("星期六")
             << QString::fromUtf8("星期日");
    int dow = now.date().dayOfWeek();  // 1=Mon ... 7=Sun
    return now.toString("yyyy-MM-dd  ") + weekDays[dow - 1] + now.toString("  hh:mm:ss");
}

void menu::updateClock()
{
    ui->timeWatermark_lb->setText(formatTimestamp());
}

void menu::onFlashHide()
{
    ui->flashOverlay_lb->hide();
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

/* ========== 摄像头帧读取 ========== */

void menu::readFrame()
{
    if (!m_cap.isOpened())
        return;

    m_cap >> m_frame;
    if (m_frame.empty()) {
        ui->noSignal_lb->show();
        return;
    }

    ui->noSignal_lb->hide();

    /* 录像写入 */
    if (m_isRecording && m_writer.isOpened()) {
        /* 缩小至最大宽度480，保证MJPG单帧<64KB AVI上限 */
        cv::Mat rec;
        double scale = 480.0 / m_frame.cols;
        if (scale < 1.0) {
            cv::resize(m_frame, rec,
                       cv::Size(480, (int)(m_frame.rows * scale)));
        } else {
            rec = m_frame;
        }
        m_writer << rec;
    }

    /* BGR → RGB → QImage → QPixmap → QLabel */
    cv::cvtColor(m_frame, m_frame, cv::COLOR_BGR2RGB);
    QImage img(m_frame.data, m_frame.cols, m_frame.rows,
               m_frame.step, QImage::Format_RGB888);
    ui->cap_Frame_lb->setPixmap(
        QPixmap::fromImage(img).scaled(
            ui->cap_Frame_lb->size(), Qt::KeepAspectRatio));
}

/* ========== 拍照（闪屏 + 缩略图飞入动画） ========== */

void menu::on_get_photo_bt_pressed()
{
    if (m_frame.empty())
        return;

    QString path = QString("/opt/aicTrain/sMonitor/photo/%1.jpg")
                       .arg(QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss"));

    /* m_frame 已被 readFrame() 转为 RGB，需转回 BGR 再保存 */
    cv::Mat bgr;
    cv::cvtColor(m_frame, bgr, cv::COLOR_RGB2BGR);
    cv::imwrite(path.toStdString(), bgr);
    qDebug() << "Photo saved:" << path;

    /* 闪屏：白色遮罩闪现 150ms */
    ui->flashOverlay_lb->show();
    QTimer::singleShot(150, this, SLOT(onFlashHide()));

    /* 缩略图飞入动画：从视频区中央飞向文件管理按钮 */
    QImage img(m_frame.data, m_frame.cols, m_frame.rows,
               m_frame.step, QImage::Format_RGB888);

    QLabel *thumb = new QLabel(this);
    thumb->setStyleSheet(
        "background: #1a1a1a; border: 2px solid rgba(255,255,255,0.5);"
        "border-radius: 6px;");
    thumb->setFixedSize(80, 60);
    thumb->setPixmap(QPixmap::fromImage(img).scaled(
        80, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    /* 起点：视频区中央 */
    QPoint startPt(ui->cap_Frame_lb->x() + ui->cap_Frame_lb->width() / 2 - 40,
                   ui->cap_Frame_lb->y() + ui->cap_Frame_lb->height() / 2 - 30);
    thumb->move(startPt);
    thumb->show();
    thumb->raise();

    /* 终点：文件管理按钮中央 */
    QPoint endPt(ui->file_Manage_bt->x() + ui->file_Manage_bt->width() / 2 - 40,
                 ui->file_Manage_bt->y() + ui->file_Manage_bt->height() / 2 - 30);

    QPropertyAnimation *anim = new QPropertyAnimation(thumb, "pos", this);
    anim->setDuration(500);
    anim->setStartValue(startPt);
    anim->setEndValue(endPt);
    anim->setEasingCurve(QEasingCurve::InCubic);
    connect(anim, &QPropertyAnimation::finished, thumb, &QLabel::deleteLater);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

/* ========== 录像 ========== */

void menu::on_get_vedio_bt_clicked()
{
    if (!m_isRecording) {
        /* 开始录制 */
        if (m_frame.empty())
            return;

        QDir().mkpath("/opt/aicTrain/sMonitor/video");

        QString path = QString("/opt/aicTrain/sMonitor/video/%1.avi")
                           .arg(QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss"));

        /* 写入尺寸必须与 readFrame() 中的缩放尺寸一致 */
        double scale = 480.0 / m_frame.cols;
        int recW = 480;
        int recH = (scale < 1.0) ? (int)(m_frame.rows * scale) : m_frame.rows;

        m_writer.open(path.toStdString(),
                      cv::VideoWriter::fourcc('M','J','P','G'),
                      30,
                      cv::Size(recW, recH));
        if (!m_writer.isOpened()) {
            qWarning("Failed to open VideoWriter: %s", qPrintable(path));
            return;
        }

        m_isRecording = true;
        m_lastVideoPath = path;
        ui->get_vedio_bt->setText(QString::fromUtf8("停止录制"));
        ui->get_vedio_bt->setStyleSheet(
            "background: rgba(255,59,48,0.5); border: 1px solid rgba(255,59,48,0.8);"
            "border-radius: 16px; font-size: 12px;");

        /* 启动录像计时 */
        m_recSeconds = 0;
        ui->recTime_lb->setText("00:00");
        ui->recTime_lb->show();
        m_recTimer->start(1000);

        qDebug() << "Recording started:" << path;
    } else {
        /* 停止录制 */
        m_isRecording = false;
        m_writer.release();

        /* 停止录像计时 */
        m_recTimer->stop();
        ui->recTime_lb->hide();

        ui->get_vedio_bt->setText(QString::fromUtf8("开始录制"));
        ui->get_vedio_bt->setStyleSheet(
            "background: rgba(255,255,255,0.08); border: 1px solid rgba(255,255,255,0.15);"
            "border-radius: 16px; font-size: 12px;");
        qDebug() << "Recording stopped";
    }
}

/* ========== 导航按钮 ========== */

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

void menu::on_exp_Check_bt_clicked()
{
    unusual *un = new unusual();
    un->show();
}

void menu::on_file_Manage_bt_clicked()
{
    fops *fo = new fops();
    fo->show();
}

void menu::on_mode_Setup_bt_clicked()
{
    setup *st = new setup();
    st->setEngPtr(&sEng);
    st->setRecordingState(m_isRecording);
    connect(st, SIGNAL(autoRecordToggled(bool)),
            this, SLOT(onAutoRecordToggled(bool)));
    st->show();
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
