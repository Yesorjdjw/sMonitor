#include "VideoWorker.h"
#include <QDir>
#include <QDebug>
#include "ConfigManager.h"

VideoWorker::VideoWorker(QObject *parent)
    : QObject(parent), m_isRecording(false), m_takePhotoFlag(false)
{
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(processFrame()));
}

VideoWorker::~VideoWorker()
{
    stopCamera();
}

void VideoWorker::startCamera()
{
    if (m_cap.open(0)) {
        qDebug() << "VideoWorker: Camera /dev/video0 opened";
    } else if (m_cap.open("/opt/aicTrain/camCapture/adver.mp4")) {
        qDebug() << "VideoWorker: Fallback: playing adver.mp4";
    } else {
        emit cameraError("no camera source available");
        return;
    }

    emit cameraOpened();
    m_timer->start(33); // ~30 fps
}

void VideoWorker::stopCamera()
{
    m_timer->stop();
    if (m_cap.isOpened()) m_cap.release();
    if (m_writer.isOpened()) m_writer.release();
}

void VideoWorker::setRecording(bool record)
{
    if (record == m_isRecording) return;

    if (record) {
        // Just set the flag. VideoWriter will lazily open in processFrame()
        m_isRecording = true;
    } else {
        m_isRecording = false;
        if (m_writer.isOpened()) m_writer.release();
        emit recordingStopped();
    }
}

void VideoWorker::takePhoto()
{
    m_takePhotoFlag = true;
}

QString VideoWorker::formatTimestamp()
{
    QDateTime now = QDateTime::currentDateTime();
    QStringList weekDays;
    weekDays << QString::fromUtf8("星期一") << QString::fromUtf8("星期二")
             << QString::fromUtf8("星期三") << QString::fromUtf8("星期四")
             << QString::fromUtf8("星期五") << QString::fromUtf8("星期六")
             << QString::fromUtf8("星期日");
    int dow = now.date().dayOfWeek();
    return now.toString("yyyy-MM-dd  ") + weekDays[dow - 1] + now.toString("  hh:mm:ss");
}

void VideoWorker::addPhysicalWatermark(cv::Mat &frame)
{
    // Simplified ASCII version since cv::putText might not support UTF-8 chinese well without FreeType
    // But we will use simple date/time to avoid font issues in edge devices
    QString text = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    // Add black outline
    cv::putText(frame, text.toStdString(), cv::Point(10, 30),
                cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 0), 3, cv::LINE_AA);
    // Add white text
    cv::putText(frame, text.toStdString(), cv::Point(10, 30),
                cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
}

void VideoWorker::processFrame()
{
    if (!m_cap.isOpened()) return;

    cv::Mat m_frame;
    m_cap >> m_frame;
    if (m_frame.empty()) return;

    // 1. Add Physical Watermark first so it applies everywhere
    addPhysicalWatermark(m_frame);

    // 2. Handle Photo Capture
    if (m_takePhotoFlag.exchange(false)) {
        QString storagePath = ConfigManager::instance().storagePath();
        if (storagePath.isEmpty()) storagePath = "/opt/aicTrain/sMonitor";
        QString dirPath = storagePath;
        if (dirPath.endsWith("/video")) dirPath.replace("/video", "/photo"); // Simple hack if storage is setup strictly to video
        QDir().mkpath(dirPath);

        QString path = QString("%1/%2.jpg").arg(dirPath).arg(QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss"));

        // Write the native BGR frame, no conversion needed!
        cv::imwrite(path.toStdString(), m_frame);
        emit photoSaved(path);
    }

    // 3. Handle Video Recording
    if (m_isRecording) {
        cv::Mat rec;
        double scale = 480.0 / m_frame.cols;
        if (scale < 1.0) {
            cv::resize(m_frame, rec, cv::Size(480, (int)(m_frame.rows * scale)));
        } else {
            rec = m_frame; // Copy is shallow unless modified
        }

        // Lazily initialize writer with exact dimensions
        if (!m_writer.isOpened()) {
            QString storagePath = ConfigManager::instance().storagePath();
            if (storagePath.isEmpty()) storagePath = "/opt/aicTrain/sMonitor/video";
            QDir().mkpath(storagePath);
            QString path = QString("%1/%2.avi").arg(storagePath).arg(QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss"));

            m_writer.open(path.toStdString(), cv::VideoWriter::fourcc('M','J','P','G'), 30, cv::Size(rec.cols, rec.rows));

            if (m_writer.isOpened()) {
                emit recordingStarted(path);
            } else {
                m_isRecording = false; // abort recording if opening fails
            }
        }

        if (m_writer.isOpened()) {
            m_writer << rec;
        }
    }

    // 4. Dispatch to UI (Convert BGR to RGB)
    // We only convert for the UI, leaving the recording/photo pipeline clean
    cv::Mat rgb;
    cv::cvtColor(m_frame, rgb, cv::COLOR_BGR2RGB);

    // Deep copy required because this crosses thread boundaries via signal/slot
    QImage img(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);
    QImage copiedImg = img.copy();

    emit frameReady(copiedImg);
}
