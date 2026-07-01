#ifndef FACEENGINE_H
#define FACEENGINE_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QList>
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>

class FaceEngine : public QObject
{
    Q_OBJECT

public:
    explicit FaceEngine(QObject *parent = nullptr);
    ~FaceEngine();

    /* 加载分类器和模型 */
    bool loadCascades(const QString &dirPath);
    bool loadModel(const QString &modelPath);
    bool loadNames(const QString &namesPath);

    /* 人脸检测：返回图像中所有人脸矩形（坐标已映射回原图） */
    QList<cv::Rect> detectFaces(const cv::Mat &frame);

    /* 从原图中裁剪并预处理人脸区域 */
    cv::Mat extractFace(const cv::Mat &frame, const cv::Rect &faceRect);

    /* 识别：返回 (label, confidence) */
    std::pair<int, double> recognize(const cv::Mat &faceImg);

    /* 根据 label 获取姓名 */
    QString nameForLabel(int label) const;

    /* 训练新模型 */
    bool trainModel(const QString &atPath, const QString &modelPath);

    /* 获取已注册人数 */
    int registeredCount() const { return m_labelNames.size(); }

    /* 获取最大 label */
    int maxLabel() const;

    /* 设置检测参数 */
    void setScaleFactor(double f) { m_scaleFactor = f; }
    void setMinNeighbors(int n) { m_minNeighbors = n; }

private:
    cv::CascadeClassifier m_frontalCascade;
    cv::CascadeClassifier m_profileCascade;
    cv::Ptr<cv::face::BasicFaceRecognizer> m_recognizer;

    QMap<int, QString> m_labelNames;  // label -> name
    double m_scaleFactor;
    int m_minNeighbors;
};

#endif // FACEENGINE_H
