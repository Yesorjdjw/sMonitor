#ifndef FACEREGISTERDIALOG_H
#define FACEREGISTERDIALOG_H

#include <QDialog>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include "faceEngine.h"

namespace Ui { class FaceRegisterDialog; }

class FaceRegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FaceRegisterDialog(FaceEngine *engine, QWidget *parent = nullptr);
    ~FaceRegisterDialog();

private slots:
    void onFrameCaptured();
    void on_captureBt_clicked();
    void on_confirmBt_clicked();
    void on_doneBt_clicked();
    void on_cancelBt_clicked();

private:
    Ui::FaceRegisterDialog *ui;
    FaceEngine *m_engine;

    cv::VideoCapture m_cap;
    QTimer *m_timer;

    QString m_name;
    int m_label;
    QString m_faceDir;
    int m_photoCount;
    static const int MAX_PHOTOS = 10;

    void refreshPreview();
};

#endif // FACEREGISTERDIALOG_H
