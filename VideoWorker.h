#ifndef VIDEOWORKER_H
#define VIDEOWORKER_H

#include <QObject>
#include <QTimer>
#include <QImage>
#include <QString>
#include <QDateTime>
#include <opencv2/opencv.hpp>
#include <atomic>

class VideoWorker : public QObject
{
    Q_OBJECT

public:
    explicit VideoWorker(QObject *parent = nullptr);
    ~VideoWorker();

public slots:
    // Core control
    void startCamera();
    void stopCamera();

    // Recording
    void setRecording(bool record);

    // Snapshot
    void takePhoto();

signals:
    // Dispatched to UI thread
    void frameReady(const QImage &image);
    void photoSaved(const QString &path);
    void recordingStarted(const QString &path);
    void recordingStopped();
    void cameraError(const QString &msg);
    void cameraOpened();

private slots:
    void processFrame();

private:
    QTimer *m_timer;
    cv::VideoCapture m_cap;
    cv::VideoWriter m_writer;

    std::atomic<bool> m_isRecording;
    std::atomic<bool> m_takePhotoFlag;

    QString formatTimestamp();
    void addPhysicalWatermark(cv::Mat &frame);
};

#endif // VIDEOWORKER_H
