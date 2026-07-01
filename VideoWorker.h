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
    void startCamera();
    void stopCamera();
    void setRecording(bool record);
    void takePhoto();

signals:
    void frameReady(const QImage &image);
    void photoSaved(const QString &path);
    void recordingStarted(const QString &path);
    void recordingStopped();
    void cameraError(const QString &msg);
    void cameraOpened();

    /* Occlusion detection signals */
    void occlusionStarted(const QString &videoPath);
    void occlusionStopped(const QString &videoPath);

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

    /* Occlusion detection */
    bool m_wasOccluded;
    bool m_isOcclusionRecording;
    cv::VideoWriter m_occlusionWriter;
    QString m_occlusionVideoPath;
    int m_occlusionFrameCount;

    void checkOcclusion(const cv::Mat &frame);
    void startOcclusionRecording(const cv::Mat &frame);
    void stopOcclusionRecording();
    void saveOcclusionEvent();
};

#endif // VIDEOWORKER_H
