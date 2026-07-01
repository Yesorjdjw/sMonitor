#include "VideoWorker.h"
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include "ConfigManager.h"

#define OCCLUSION_VIDEO_DIR "/opt/aicTrain/sMonitor/video/unusual"
#define OCCLUSION_EVENTS_JSON "/opt/aicTrain/sMonitor/unusual/unusual_events.json"
#define OCCLUSION_DARK_THRESHOLD 30
#define OCCLUSION_UNIFORM_THRESHOLD 20
#define OCCLUSION_MIN_FRAMES 10
#define OCCLUSION_MAX_FRAMES 75  // 5 sec @ 15 fps
#define OCCLUSION_COOLDOWN_FRAMES 150  // 10 sec cooldown before next detection

VideoWorker::VideoWorker(QObject *parent)
    : QObject(parent)
    , m_isRecording(false)
    , m_takePhotoFlag(false)
    , m_wasOccluded(false)
    , m_isOcclusionRecording(false)
    , m_occlusionFrameCount(0)
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
    // Try camera indices 0-3
    bool camOk = false;
    for (int ci = 0; ci <= 3; ci++) { if (m_cap.open(ci)) { camOk = true; break; } }
    if (camOk) {
        qDebug() << "VideoWorker: Camera opened";
    } else if (m_cap.open("/opt/aicTrain/camCapture/adver.mp4")) {
        qDebug() << "VideoWorker: Fallback: playing adver.mp4";
    } else {
        emit cameraError("no camera source available");
        return;
    }

    emit cameraOpened();
    m_timer->start(33);
}

void VideoWorker::stopCamera()
{
    m_timer->stop();
    if (m_cap.isOpened()) m_cap.release();
    if (m_writer.isOpened()) m_writer.release();
    if (m_occlusionWriter.isOpened()) m_occlusionWriter.release();
}

void VideoWorker::setRecording(bool record)
{
    if (record == m_isRecording) return;
    if (record) {
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
    return now.toString("yyyy-MM-dd hh:mm:ss");
}

void VideoWorker::addPhysicalWatermark(cv::Mat &frame)
{
    QString text = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    cv::putText(frame, text.toStdString(), cv::Point(10, 30),
                cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 0), 3, cv::LINE_AA);
    cv::putText(frame, text.toStdString(), cv::Point(10, 30),
                cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
}

void VideoWorker::checkOcclusion(const cv::Mat &frame)
{
    // Convert to grayscale and calculate statistics
    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    cv::Scalar mean, stddev;
    cv::meanStdDev(gray, mean, stddev);

    double meanVal = mean[0];
    double stdVal = stddev[0];

    bool isOccluded = (meanVal < OCCLUSION_DARK_THRESHOLD && stdVal < OCCLUSION_UNIFORM_THRESHOLD);

    if (isOccluded && !m_wasOccluded) {
        // Transition: normal -> occluded
        m_wasOccluded = true;
        m_occlusionFrameCount = 1;
        qDebug() << "Occlusion detected! mean=" << meanVal << "stddev=" << stdVal;
    }
    else if (isOccluded && m_wasOccluded) {
        // Still occluded
        m_occlusionFrameCount++;

        // Start recording after sustained occlusion
        if (m_occlusionFrameCount >= OCCLUSION_MIN_FRAMES && !m_isOcclusionRecording) {
            startOcclusionRecording(frame);
        }
    }
    else if (!isOccluded && m_wasOccluded) {
        // Transition: occluded -> normal
        m_wasOccluded = false;

        if (m_isOcclusionRecording) {
            stopOcclusionRecording();
            saveOcclusionEvent();
        }
        m_occlusionFrameCount = 0;
        qDebug() << "Occlusion cleared";
    }

    // Record occlusion video frames - max 5 seconds
    if (m_isOcclusionRecording) {
        cv::Mat recFrame;
        cv::resize(frame, recFrame, cv::Size(480, 360));
        m_occlusionWriter << recFrame;
        m_occlusionFrameCount++;
        // Stop after 5 seconds max, regardless of occlusion state
        if (m_occlusionFrameCount >= OCCLUSION_MIN_FRAMES + OCCLUSION_MAX_FRAMES) {
            stopOcclusionRecording();
            saveOcclusionEvent();
            m_occlusionFrameCount = OCCLUSION_COOLDOWN_FRAMES; // start cooldown
            qDebug() << "Occlusion recording finished (5s max)";
        }
    }
    // Cooldown: count down to re-enable detection
    if (m_occlusionFrameCount > 0 && !m_wasOccluded && !m_isOcclusionRecording) {
        m_occlusionFrameCount--;
    }
}

void VideoWorker::startOcclusionRecording(const cv::Mat &frame)
{
    QDir().mkpath(OCCLUSION_VIDEO_DIR);
    m_occlusionVideoPath = QString("%1/occlusion_%2.avi")
        .arg(OCCLUSION_VIDEO_DIR)
        .arg(QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss"));

    m_occlusionWriter.open(m_occlusionVideoPath.toStdString(),
        cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 15, cv::Size(480, 360));

    if (m_occlusionWriter.isOpened()) {
        m_isOcclusionRecording = true;
        emit occlusionStarted(m_occlusionVideoPath);
        qDebug() << "Occlusion recording started:" << m_occlusionVideoPath;
    }
}

void VideoWorker::stopOcclusionRecording()
{
    m_isOcclusionRecording = false;
    if (m_occlusionWriter.isOpened()) {
        m_occlusionWriter.release();
    }
    emit occlusionStopped(m_occlusionVideoPath);
    qDebug() << "Occlusion recording stopped";
}

void VideoWorker::saveOcclusionEvent()
{
    // Save thumbnail from video's first frame
    QString thumbPath;
    cv::VideoCapture tmpCap(m_occlusionVideoPath.toStdString());
    if (tmpCap.isOpened()) {
        cv::Mat firstFrame;
        tmpCap >> firstFrame;
        if (!firstFrame.empty()) {
            thumbPath = m_occlusionVideoPath;
            thumbPath.replace(".avi", "_thumb.jpg");
            cv::resize(firstFrame, firstFrame, cv::Size(120, 90));
            cv::imwrite(thumbPath.toStdString(), firstFrame);
        }
        tmpCap.release();
    }

    QJsonArray events;
    QFile file(OCCLUSION_EVENTS_JSON);
    if (file.open(QIODevice::ReadOnly)) {
        events = QJsonDocument::fromJson(file.readAll()).array();
        file.close();
    }

    QJsonObject ev;
    ev["screenshot"] = thumbPath;
    ev["type"] = QString::fromUtf8("画面遮挡");
    ev["level"] = QString::fromUtf8("紧急");
    ev["time"] = formatTimestamp();
    ev["status"] = QString::fromUtf8("待处理");
    ev["videoPath"] = m_occlusionVideoPath;
    events.append(ev);

    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(events).toJson());
        file.close();
    }
}

void VideoWorker::processFrame()
{
    if (!m_cap.isOpened()) return;

    cv::Mat m_frame;
    m_cap >> m_frame;
    if (m_frame.empty()) return;

    // 1. Add Physical Watermark
    addPhysicalWatermark(m_frame);

    // 2. Handle Photo Capture
    if (m_takePhotoFlag.exchange(false)) {
        QString storagePath = ConfigManager::instance().storagePath();
        if (storagePath.isEmpty()) storagePath = "/opt/aicTrain/sMonitor";
        QString dirPath = storagePath;
        QDir().mkpath(dirPath);

        QString path = QString("%1/%2.jpg").arg(dirPath)
            .arg(QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss"));

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
            rec = m_frame;
        }

        if (!m_writer.isOpened()) {
            QString storagePath = ConfigManager::instance().storagePath();
            if (storagePath.isEmpty()) storagePath = "/opt/aicTrain/sMonitor/video";
            QDir().mkpath(storagePath);
            QString path = QString("%1/%2.avi").arg(storagePath)
                .arg(QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss"));

            m_writer.open(path.toStdString(),
                cv::VideoWriter::fourcc('M','J','P','G'), 30, cv::Size(rec.cols, rec.rows));

            if (m_writer.isOpened()) {
                emit recordingStarted(path);
            } else {
                m_isRecording = false;
            }
        }

        if (m_writer.isOpened()) {
            m_writer << rec;
        }
    }

    // 4. Occlusion Detection
    checkOcclusion(m_frame);

    // 5. Dispatch to UI
    cv::Mat rgb;
    cv::cvtColor(m_frame, rgb, cv::COLOR_BGR2RGB);
    QImage img(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);
    QImage copiedImg = img.copy();
    emit frameReady(copiedImg);
}
