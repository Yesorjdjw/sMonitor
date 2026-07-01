#include "logindlg.h"
#include "ui_logindlg.h"
#include "registerdlg.h"
#include <QDebug>
#include <QCloseEvent>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QThread>

#define FACE_DATA_DIR "/opt/aicTrain/sMonitor/facedata"
#define CASCADE_DIR   "/opt/aicTrain/sMonitor"
#define UNUSUAL_VIDEO_DIR "/opt/aicTrain/sMonitor/video/unusual"
#define UNUSUAL_EVENTS_JSON "/opt/aicTrain/sMonitor/unusual/unusual_events.json"
#define ANOMALY_RECORD_SECS 5

LoginDlg::LoginDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDlg)
    , m_engine(new FaceEngine(this))
    , m_state(SCANNING)
    , m_noFaceCount(0)
    , m_recognitionFailures(0)
    , m_isRecordingAnomaly(false)
    , m_anomalyFrames(0)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setGeometry(0, 0, 1024, 600);

    // Initialize face engine
    m_engine->loadCascades(CASCADE_DIR);
    m_engine->loadNames(QString(FACE_DATA_DIR) + "/Faces/name.txt");
    m_engine->loadModel(QString(FACE_DATA_DIR) + "/MyFacePCAModel.xml");

    // Start camera
    // Try camera indices 0-3
    bool camOk = false;
    for (int ci = 0; ci <= 3; ci++) { if (m_cap.open(ci)) { camOk = true; break; } }
    if (!camOk) {
        m_cap.open("/opt/aicTrain/camCapture/adver.mp4");
    }

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &LoginDlg::onFrameCaptured);
    m_timer->start(50); // ~20 fps for smoother UI

    setState(SCANNING);
}

LoginDlg::~LoginDlg()
{
    m_timer->stop();
    if (m_cap.isOpened()) m_cap.release();
    if (m_anomalyWriter.isOpened()) m_anomalyWriter.release();
    delete ui;
}

void LoginDlg::closeEvent(QCloseEvent *e)
{
    m_timer->stop();
    QDialog::closeEvent(e);
}

QString LoginDlg::formatTimestamp()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
}

void LoginDlg::setState(State s)
{
    m_state = s;
    switch (s) {
    case SCANNING:
        ui->statusLabel->setText(QString::fromUtf8("正在扫描人脸..."));
        ui->nameLabel->setText("");
        ui->registerBt->hide();
        ui->retryBt->hide();
        break;
    case DETECTED:
        ui->statusLabel->setText(QString::fromUtf8("检测到人脸，正在识别..."));
        break;
    case SUCCESS:
        ui->statusLabel->setText(QString::fromUtf8("识别成功！"));
        ui->registerBt->hide();
        ui->retryBt->hide();
        break;
    case FAILED:
        ui->statusLabel->setText(QString::fromUtf8("人脸未注册"));
        ui->nameLabel->setText(QString::fromUtf8("未注册用户"));
        ui->registerBt->show();
        ui->retryBt->show();
        break;
    default:
        break;
    }
}

void LoginDlg::startAnomalyRecording()
{
    if (m_isRecordingAnomaly) return;

    QDir().mkpath(UNUSUAL_VIDEO_DIR);
    m_anomalyVideoPath = QString("%1/login_anomaly_%2.avi")
        .arg(UNUSUAL_VIDEO_DIR)
        .arg(QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss"));

    m_anomalyWriter.open(m_anomalyVideoPath.toStdString(),
        cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 15, cv::Size(640, 480));

    if (m_anomalyWriter.isOpened()) {
        m_isRecordingAnomaly = true;
        m_anomalyFrames = 0;
        qDebug() << "Anomaly recording started:" << m_anomalyVideoPath;
    }
}

void LoginDlg::stopAnomalyRecording()
{
    if (!m_isRecordingAnomaly) return;
    m_isRecordingAnomaly = false;
    if (m_anomalyWriter.isOpened()) {
        m_anomalyWriter.release();
    }
    qDebug() << "Anomaly recording stopped";
}

void LoginDlg::saveAnomalyEvent()
{
    QDir().mkpath(QFileInfo(UNUSUAL_EVENTS_JSON).absolutePath());

    // Save thumbnail from video's first frame
    QString thumbPath;
    cv::VideoCapture tmpCap(m_anomalyVideoPath.toStdString());
    if (tmpCap.isOpened()) {
        cv::Mat firstFrame;
        tmpCap >> firstFrame;
        if (!firstFrame.empty()) {
            thumbPath = m_anomalyVideoPath;
            thumbPath.replace(".avi", "_thumb.jpg");
            cv::resize(firstFrame, firstFrame, cv::Size(120, 90));
            cv::imwrite(thumbPath.toStdString(), firstFrame);
        }
        tmpCap.release();
    }

    // Load existing events
    QJsonArray events;
    QFile file(UNUSUAL_EVENTS_JSON);
    if (file.open(QIODevice::ReadOnly)) {
        events = QJsonDocument::fromJson(file.readAll()).array();
        file.close();
    }

    QJsonObject ev;
    ev["screenshot"] = thumbPath;
    ev["type"] = QString::fromUtf8("人脸识别异常");
    ev["level"] = QString::fromUtf8("警告");
    ev["time"] = formatTimestamp();
    ev["status"] = QString::fromUtf8("待处理");
    ev["videoPath"] = m_anomalyVideoPath;
    events.append(ev);

    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(events).toJson());
        file.close();
    }
}

void LoginDlg::onFrameCaptured()
{
    cv::Mat frame;
    m_cap >> frame;
    if (frame.empty()) return;

    cv::Mat display = frame.clone();

    // Detect faces
    QList<cv::Rect> faces = m_engine->detectFaces(frame);

    // Draw circles around detected faces
    for (const cv::Rect &r : faces) {
        int radius = (r.width + r.height) / 4;
        cv::Point center(r.x + r.width / 2, r.y + r.height / 2);
        cv::Scalar color;
        if (m_state == SUCCESS)
            color = cv::Scalar(0, 255, 0);   // green on success
        else if (m_state == FAILED)
            color = cv::Scalar(0, 0, 255);    // red on failed
        else
            color = cv::Scalar(0, 255, 255);   // yellow scanning
        cv::circle(display, center, radius, color, 3, 8, 0);
    }

    // State machine
    switch (m_state) {
    case SCANNING:
        if (!faces.isEmpty()) {
            setState(DETECTED);
            m_noFaceCount = 0;
        } else {
            m_noFaceCount++;
        }
        break;

    case DETECTED:
        if (faces.isEmpty()) {
            setState(SCANNING);
            break;
        }
        // Try to recognize the first face
        {
            cv::Mat faceImg = m_engine->extractFace(frame, faces.first());
            auto result = m_engine->recognize(faceImg);
            int label = result.first;
            double confidence = result.second;

            qDebug() << "Recognition: label=" << label << "confidence=" << confidence;

            if (label >= 0 && confidence < 5000.0) {
                // Recognized!
                m_loggedInUser = m_engine->nameForLabel(label);
                ui->nameLabel->setText(QString::fromUtf8("欢迎，") + m_loggedInUser);
                setState(SUCCESS);

                // Auto-login after 1 second
                QTimer::singleShot(1000, this, [this]() {
                    accept();
                });
            } else {
                // Not recognized
                m_recognitionFailures++;
                if (m_recognitionFailures >= 3) {
                    m_recognitionFailures = 0;
                    setState(FAILED);

                    // Start anomaly recording
                    if (!m_isRecordingAnomaly) {
                        startAnomalyRecording();
                    }
                }
            }
        }
        break;

    case RECOGNIZING:
        break;

    case SUCCESS:
        break;

    case FAILED:
        // If anomaly recording active, keep recording
        break;
    }

    // Record anomaly video frames
    if (m_isRecordingAnomaly) {
        cv::Mat recFrame;
        cv::resize(frame, recFrame, cv::Size(640, 480));
        m_anomalyWriter << recFrame;
        m_anomalyFrames++;

        // Record for ~5 seconds (15 fps * 5 = 75 frames)
        if (m_anomalyFrames >= 15 * ANOMALY_RECORD_SECS) {
            stopAnomalyRecording();
            saveAnomalyEvent();
        }
    }

    // Show frame in UI
    cv::Mat rgb;
    cv::cvtColor(display, rgb, cv::COLOR_BGR2RGB);
    QImage img(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);
    ui->cameraLabel->setPixmap(
        QPixmap::fromImage(img).scaled(
            ui->cameraLabel->size(), Qt::KeepAspectRatio));
}

void LoginDlg::on_registerBt_clicked()
{
    m_timer->stop();
    if (m_cap.isOpened()) m_cap.release();

    // Scope block: RegisterDlg must be destroyed before camera reopen
    {
        RegisterDlg dlg(m_engine, this);
        if (dlg.exec() == QDialog::Accepted) {
            m_engine->loadNames(QString(FACE_DATA_DIR) + "/Faces/name.txt");
            m_engine->loadModel(QString(FACE_DATA_DIR) + "/MyFacePCAModel.xml");
        }
    } // dlg destroyed here, camera released

    // Re-start camera with retry
    bool opened = false;
    for (int retry = 0; retry < 5; retry++) {
        for (int ci = 0; ci <= 3; ci++) { if (m_cap.open(ci)) { opened = true; break; } }
        if (opened) break;
        QThread::msleep(500);
    }
    if (!opened) {
        m_cap.open("/opt/aicTrain/camCapture/adver.mp4");
    }
    m_recognitionFailures = 0;
    setState(SCANNING);
    m_timer->start(50);
}

void LoginDlg::on_retryBt_clicked()
{
    m_recognitionFailures = 0;
    setState(SCANNING);
}
