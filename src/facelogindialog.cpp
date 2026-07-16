#include "facelogindialog.h"
#include "faceEngine.h"
#include "faceRegisterDialog.h"
#include <QDebug>
#include <QCloseEvent>
#include <QPainter>
#include <QThread>

FaceLoginDialog::FaceLoginDialog(FaceEngine *engine, QWidget *parent)
    : QDialog(parent)
    , m_engine(engine)
    , m_state(SCANNING)
    , m_failCount(0)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setGeometry(0, 0, 1024, 600);
    setStyleSheet("background-color: #1a2a4a;");

    // Camera preview
    m_cameraLabel = new QLabel(this);
    m_cameraLabel->setGeometry(312, 10, 400, 300);
    m_cameraLabel->setStyleSheet("background: black; border: 3px solid #4a90d9; border-radius: 8px;");

    // Status text
    m_statusLabel = new QLabel(this);
    m_statusLabel->setGeometry(312, 320, 400, 40);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setStyleSheet("color: white; font-size: 18px; font-weight: bold; background: transparent;");

    // Name label
    m_nameLabel = new QLabel(this);
    m_nameLabel->setGeometry(312, 365, 400, 40);
    m_nameLabel->setAlignment(Qt::AlignCenter);
    m_nameLabel->setStyleSheet("color: #27ae60; font-size: 20px; font-weight: bold; background: transparent;");

    // Register button
    m_registerBtn = new QPushButton("注册新用户", this);
    m_registerBtn->setGeometry(370, 415, 130, 44);
    m_registerBtn->setCursor(Qt::PointingHandCursor);
    m_registerBtn->setStyleSheet(
        "QPushButton { background-color: #f39c12; color: white; border: none; border-radius: 6px; font-size: 16px; }"
        "QPushButton:hover { background-color: #e67e22; }");
    m_registerBtn->hide();
    connect(m_registerBtn, &QPushButton::clicked, this, &FaceLoginDialog::onRegisterClicked);

    // Retry button
    m_retryBtn = new QPushButton("重新识别", this);
    m_retryBtn->setGeometry(510, 415, 124, 44);
    m_retryBtn->setCursor(Qt::PointingHandCursor);
    m_retryBtn->setStyleSheet(
        "QPushButton { background-color: #4a90d9; color: white; border: none; border-radius: 6px; font-size: 16px; }"
        "QPushButton:hover { background-color: #3a7bc8; }");
    m_retryBtn->hide();
    connect(m_retryBtn, &QPushButton::clicked, this, &FaceLoginDialog::onRetryClicked);

    // Back button
    m_backBtn = new QPushButton("返回登录", this);
    m_backBtn->setGeometry(450, 520, 124, 40);
    m_backBtn->setCursor(Qt::PointingHandCursor);
    m_backBtn->setStyleSheet(
        "QPushButton { background-color: #7f8c8d; color: white; border: none; border-radius: 6px; font-size: 14px; }"
        "QPushButton:hover { background-color: #6c7a7d; }");
    connect(m_backBtn, &QPushButton::clicked, this, &FaceLoginDialog::onBackClicked);

    // Try camera
    bool camOk = false;
    for (int ci = 0; ci <= 5; ci++) {
        if (m_cap.open(ci)) {
            camOk = true;
            qDebug() << "FaceLoginDialog: Camera /dev/video" << ci << "opened";
            break;
        }
    }
    if (!camOk) {
        m_statusLabel->setText("未检测到摄像头");
    }

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &FaceLoginDialog::onFrameCaptured);
    m_timer->start(50);

    setState(SCANNING);
}

FaceLoginDialog::~FaceLoginDialog()
{
    m_timer->stop();
    if (m_cap.isOpened()) m_cap.release();
}

void FaceLoginDialog::closeEvent(QCloseEvent *e)
{
    m_timer->stop();
    QDialog::closeEvent(e);
}

void FaceLoginDialog::setState(State s)
{
    m_state = s;
    switch (s) {
    case SCANNING:
        m_statusLabel->setStyleSheet("color: white; font-size: 18px; font-weight: bold; background: transparent;");
        m_statusLabel->setText("正在扫描人脸...");
        m_nameLabel->setText("");
        m_retryBtn->hide();
        m_registerBtn->hide();
        break;
    case DETECTED:
        m_statusLabel->setText("检测到人脸，正在识别...");
        break;
    case SUCCESS:
        m_statusLabel->setStyleSheet("color: #27ae60; font-size: 18px; font-weight: bold; background: transparent;");
        m_statusLabel->setText("识别成功！");
        m_retryBtn->hide();
        m_registerBtn->hide();
        break;
    case FAILED:
        m_statusLabel->setStyleSheet("color: #e74c3c; font-size: 18px; font-weight: bold; background: transparent;");
        m_statusLabel->setText("人脸未注册");
        m_nameLabel->setText("未注册用户");
        m_retryBtn->show();
        m_registerBtn->show();
        break;
    }
}

void FaceLoginDialog::onRetryClicked()
{
    m_failCount = 0;
    setState(SCANNING);
}

void FaceLoginDialog::onRegisterClicked()
{
    m_timer->stop();
    if (m_cap.isOpened()) m_cap.release();

    FaceRegisterDialog dlg(m_engine, this);
    if (dlg.exec() == QDialog::Accepted) {
        m_engine->loadNames("/opt/aicTrain/sMonitor_server/facedata/Faces/name.txt");
        m_engine->loadModel("/opt/aicTrain/sMonitor_server/facedata/MyFacePCAModel.xml");
    }

    // Reopen camera
    bool opened = false;
    for (int ci = 0; ci <= 5; ci++) { if (m_cap.open(ci)) { opened = true; break; } }
    m_failCount = 0;
    setState(SCANNING);
    m_timer->start(50);
}

void FaceLoginDialog::onBackClicked()
{
    reject();
}

void FaceLoginDialog::onFrameCaptured()
{
    cv::Mat frame;
    m_cap >> frame;
    if (frame.empty()) return;
    frame = frame.clone();

    cv::Mat display = frame.clone();
    QList<cv::Rect> faces = m_engine->detectFaces(frame);

    for (const cv::Rect &r : faces) {
        int radius = (r.width + r.height) / 4;
        cv::Point center(r.x + r.width / 2, r.y + r.height / 2);
        cv::Scalar color;
        if (m_state == SUCCESS)
            color = cv::Scalar(0, 255, 0);
        else if (m_state == FAILED)
            color = cv::Scalar(0, 0, 255);
        else
            color = cv::Scalar(0, 255, 255);
        cv::circle(display, center, radius, color, 3, 8, 0);
    }

    switch (m_state) {
    case SCANNING:
        if (!faces.isEmpty()) setState(DETECTED);
        break;
    case DETECTED:
        if (faces.isEmpty()) { setState(SCANNING); break; }
        {
            cv::Mat faceImg = m_engine->extractFace(frame, faces.first());
            if (!faceImg.empty()) {
                auto result = m_engine->recognize(faceImg);
                int label = result.first;
                double confidence = result.second;
                qDebug() << "FaceLogin: label=" << label << "confidence=" << confidence;
                if (label >= 0 && confidence < 3000) {
                    m_loggedInUser = m_engine->nameForLabel(label);
                    if (m_loggedInUser.isEmpty()) m_loggedInUser = "admin";
                    m_nameLabel->setText("欢迎，" + m_loggedInUser);
                    setState(SUCCESS);
                    QTimer::singleShot(1500, this, &QDialog::accept);
                } else {
                    m_failCount++;
                    if (m_failCount >= 3) {
                        m_failCount = 0;
                        setState(FAILED);
                    }
                }
            }
        }
        break;
    case SUCCESS:
    case FAILED:
        break;
    }

    cv::Mat rgb;
    cv::cvtColor(display, rgb, cv::COLOR_BGR2RGB);
    QImage img(rgb.cols, rgb.rows, QImage::Format_RGB888);
    for (int y = 0; y < rgb.rows; y++)
        memcpy(img.scanLine(y), rgb.ptr(y), rgb.cols * 3);
    m_cameraLabel->setPixmap(
        QPixmap::fromImage(img).scaled(m_cameraLabel->size(), Qt::KeepAspectRatio));
}
