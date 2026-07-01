#include "faceEngine.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <fstream>

FaceEngine::FaceEngine(QObject *parent)
    : QObject(parent)
    , m_scaleFactor(1.3)
    , m_minNeighbors(3)
{
}

FaceEngine::~FaceEngine()
{
}

bool FaceEngine::loadCascades(const QString &dirPath)
{
    QString frontal = dirPath + "/haarcascade_frontalface_alt.xml";
    QString profile = dirPath + "/haarcascade_profileface.xml";

    bool ok = true;
    if (!m_frontalCascade.load(frontal.toStdString())) {
        qWarning() << "FaceEngine: Failed to load" << frontal;
        ok = false;
    }
    if (!m_profileCascade.load(profile.toStdString())) {
        qWarning() << "FaceEngine: Failed to load" << profile;
        // not fatal — profile cascade is optional
    }
    return ok;
}

bool FaceEngine::loadModel(const QString &modelPath)
{
    m_recognizer = cv::face::EigenFaceRecognizer::create();
    try {
        m_recognizer->read(modelPath.toStdString());
        qDebug() << "FaceEngine: Model loaded from" << modelPath;
        return true;
    } catch (cv::Exception &e) {
        qWarning() << "FaceEngine: Failed to load model:" << e.what();
        // Train a new one from scratch if model doesn't exist
        m_recognizer = cv::face::EigenFaceRecognizer::create();
        return false;
    }
}

bool FaceEngine::loadNames(const QString &namesPath)
{
    m_labelNames.clear();
    QFile file(namesPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "FaceEngine: Cannot open" << namesPath;
        return false;
    }
    QTextStream in(&file);
    int label = 0;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty()) {
            m_labelNames[label] = line;
            label++;
        }
    }
    file.close();
    qDebug() << "FaceEngine: Loaded" << m_labelNames.size() << "names";
    return true;
}

int FaceEngine::maxLabel() const
{
    if (m_labelNames.isEmpty()) return 0;
    return m_labelNames.lastKey() + 1;
}

QList<cv::Rect> FaceEngine::detectFaces(const cv::Mat &frame)
{
    QList<cv::Rect> results;
    if (m_frontalCascade.empty()) return results;

    double scale = 3.0;
    cv::Mat gray, smallImg;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    int sw = cvRound(frame.cols / scale);
    int sh = cvRound(frame.rows / scale);
    cv::resize(gray, smallImg, cv::Size(sw, sh));
    cv::equalizeHist(smallImg, smallImg);

    std::vector<cv::Rect> faces, faces2;
    m_frontalCascade.detectMultiScale(smallImg, faces,
        m_scaleFactor, m_minNeighbors,
        0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));

    if (!m_profileCascade.empty()) {
        m_profileCascade.detectMultiScale(smallImg, faces2,
            m_scaleFactor, m_minNeighbors,
            0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));
        faces.insert(faces.end(), faces2.begin(), faces2.end());
    }

    for (const auto &r : faces) {
        double aspect = (double)r.width / r.height;
        if (aspect < 0.75 || aspect > 1.3) continue;
        // Map coordinates back to original image
        int x = cvRound(r.x * scale);
        int y = cvRound(r.y * scale);
        int w = cvRound(r.width * scale);
        int h = cvRound(r.height * scale);
        results.append(cv::Rect(x, y, w, h));
    }
    return results;
}

cv::Mat FaceEngine::extractFace(const cv::Mat &frame, const cv::Rect &faceRect)
{
    // Add margin around face
    int margin = 20;
    int x = std::max(0, faceRect.x - margin);
    int y = std::max(0, faceRect.y - margin);
    int w = std::min(frame.cols - x, faceRect.width + margin * 2);
    int h = std::min(frame.rows - y, faceRect.height + margin * 2);

    cv::Mat faceROI = frame(cv::Rect(x, y, w, h)).clone();
    cv::cvtColor(faceROI, faceROI, cv::COLOR_BGR2GRAY);
    cv::resize(faceROI, faceROI, cv::Size(200, 200));
        return faceROI;
}

std::pair<int, double> FaceEngine::recognize(const cv::Mat &faceImg)
{
    if (!m_recognizer) return {-1, 99999.0};

    try {
        int predictedLabel = -1;
        double confidence = 99999.0;
        m_recognizer->predict(faceImg, predictedLabel, confidence);
        return {predictedLabel, confidence};
    } catch (cv::Exception &e) {
        qWarning() << "FaceEngine: recognize error:" << e.what();
        return {-1, 99999.0};
    }
}

QString FaceEngine::nameForLabel(int label) const
{
    return m_labelNames.value(label, QString("用户%1").arg(label));
}

bool FaceEngine::trainModel(const QString &atPath, const QString &modelPath)
{
    std::vector<cv::Mat> images;
    std::vector<int> labels;

    // Read at.txt
    std::ifstream file(atPath.toStdString());
    if (!file.is_open()) {
        qWarning() << "FaceEngine: Cannot open" << atPath;
        return false;
    }

    std::string line, path, classlabel;
    while (std::getline(file, line)) {
        std::stringstream liness(line);
        if (!std::getline(liness, path, ';')) continue;
        if (!std::getline(liness, classlabel)) continue;
        cv::Mat img = cv::imread(path, cv::IMREAD_GRAYSCALE);
        if (img.empty()) {
            qWarning() << "FaceEngine: Cannot read" << QString::fromStdString(path);
            continue;
        }
        cv::resize(img, img, cv::Size(200, 200));
        images.push_back(img);
        labels.push_back(std::atoi(classlabel.c_str()));
    }
    file.close();

    if (images.size() < 2) {
        qWarning() << "FaceEngine: Need at least 2 samples to train";
        return false;
    }

    // Train
    m_recognizer = cv::face::EigenFaceRecognizer::create();
    m_recognizer->train(images, labels);
    m_recognizer->write(modelPath.toStdString());

    qDebug() << "FaceEngine: Model trained with" << images.size() << "samples, saved to" << modelPath;
    return true;
}
