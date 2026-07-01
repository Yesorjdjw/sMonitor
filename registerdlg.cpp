#include "registerdlg.h"
#include "ui_registerdlg.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QCoreApplication>

RegisterDlg::RegisterDlg(FaceEngine *engine, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterDlg)
    , m_engine(engine)
    , m_photoCount(0)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setGeometry(0, 0, 1024, 600);

    // Open camera
    // Try camera indices 0-3
    bool camOk = false;
    for (int ci = 0; ci <= 3; ci++) { if (m_cap.open(ci)) { camOk = true; break; } }
    if (!camOk) {
        m_cap.open((QCoreApplication::applicationDirPath() + "/camCapture/adver.mp4").toStdString());
    }

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &RegisterDlg::onFrameCaptured);
    m_timer->start(50);

    ui->progressLabel->setText(QString::fromUtf8("0 / %1 张").arg(MAX_PHOTOS));
    ui->captureBt->setEnabled(false);
}

RegisterDlg::~RegisterDlg()
{
    m_timer->stop();
    if (m_cap.isOpened()) m_cap.release();
    delete ui;
}

void RegisterDlg::onFrameCaptured()
{
    cv::Mat frame;
    m_cap >> frame;
    if (frame.empty()) return;

    // Detect faces
    QList<cv::Rect> faces = m_engine->detectFaces(frame);

    // Draw circles on detected faces
    cv::Mat display = frame.clone();
    for (const cv::Rect &r : faces) {
        int radius = (r.width + r.height) / 4;
        cv::Point center(r.x + r.width / 2, r.y + r.height / 2);
        cv::circle(display, center, radius, cv::Scalar(0, 0, 255), 3, 8, 0);
    }

    // Enable capture button when face is detected
    if (!faces.isEmpty() && !m_faceDir.isEmpty()) {
        ui->captureBt->setEnabled(true);
    } else {
        ui->captureBt->setEnabled(false);
    }

    cv::Mat rgb;
    cv::cvtColor(display, rgb, cv::COLOR_BGR2RGB);
    QImage img(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);
    ui->cameraLabel->setPixmap(
        QPixmap::fromImage(img).scaled(
            ui->cameraLabel->size(), Qt::KeepAspectRatio));
}

void RegisterDlg::on_confirmBt_clicked()
{
    m_name = ui->nameEdit->text().trimmed();
    if (m_name.isEmpty()) return;

    m_label = m_engine->maxLabel();

    QString faceDataDir = QCoreApplication::applicationDirPath() + "/facedata";
    m_faceDir = faceDataDir + QString("/Faces/s%1").arg(m_label);
    QDir().mkpath(m_faceDir);

    m_photoCount = 0;
    ui->progressLabel->setText(QString("0 / %1").arg(MAX_PHOTOS));
    ui->statusLabel->setText(QString::fromUtf8("已确认姓名: ") + m_name + QString::fromUtf8("，请开始拍照"));
    ui->confirmBt->setEnabled(false);
    ui->nameEdit->setEnabled(false);
    qDebug() << "Register: new user" << m_name << "label" << m_label << "dir" << m_faceDir;
}

void RegisterDlg::on_captureBt_clicked()
{
    if (m_faceDir.isEmpty()) return;

    cv::Mat frame;
    m_cap >> frame;
    if (frame.empty()) return;

    QList<cv::Rect> faces = m_engine->detectFaces(frame);
    if (faces.isEmpty()) return;

    cv::Mat faceImg = m_engine->extractFace(frame, faces.first());

    m_photoCount++;
    QString fileName = QString("%1.jpg").arg(m_photoCount);
    QString savePath = m_faceDir + "/" + fileName;
    cv::imwrite(savePath.toStdString(), faceImg);

    qDebug() << "Registered photo:" << savePath;
    ui->progressLabel->setText(QString("%1 / %2").arg(m_photoCount).arg(MAX_PHOTOS));

    if (m_photoCount >= MAX_PHOTOS) {
        QString faceDataDir = QCoreApplication::applicationDirPath() + "/facedata";
        QString atPath = faceDataDir + "/Faces/at.txt";
        QFile af(atPath);
        if (af.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&af);
            for (int i = 1; i <= MAX_PHOTOS; i++) {
                out << "./facedata/Faces/s" << m_label << "/" << i << ".jpg;" << m_label << "\n";
            }
            af.close();
        }

        QString namePath = faceDataDir + "/Faces/name.txt";
        QFile nf(namePath);
        if (nf.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&nf);
            out << m_name << "\n";
            nf.close();
        }

        m_engine->trainModel(atPath, faceDataDir + "/MyFacePCAModel.xml");

        ui->statusLabel->setText(QString::fromUtf8("注册完成！"));
        ui->captureBt->setEnabled(false);
        ui->doneBt->setEnabled(true);
        m_timer->stop();
        if (m_cap.isOpened()) m_cap.release();
    }
}

void RegisterDlg::on_doneBt_clicked()
{
    accept();
}

void RegisterDlg::on_cancelBt_clicked()
{
    reject();
}
