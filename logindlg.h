#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QDialog>
#include <QTimer>
#include <QImage>
#include <opencv2/opencv.hpp>
#include "faceEngine.h"

namespace Ui { class LoginDlg; }

class LoginDlg : public QDialog
{
    Q_OBJECT
public:
    explicit LoginDlg(QWidget *parent = nullptr);
    ~LoginDlg();
    QString loggedInUser() const { return m_loggedInUser; }
protected:
    void closeEvent(QCloseEvent *e) override;
private slots:
    void onFrameCaptured();
    void on_registerBt_clicked();
    void on_retryBt_clicked();
private:
    Ui::LoginDlg *ui;
    FaceEngine *m_engine;
    cv::VideoCapture m_cap;
    QTimer *m_timer;
    enum State { SCANNING, DETECTED, RECOGNIZING, SUCCESS, FAILED };
    State m_state;
    QString m_loggedInUser;
    int m_noFaceCount;
    int m_recognitionFailures;
    cv::VideoWriter m_anomalyWriter;
    int m_anomalyFrames;
    bool m_isRecordingAnomaly;
    QString m_anomalyVideoPath;
    void setState(State s);
    void startAnomalyRecording();
    void stopAnomalyRecording();
    void saveAnomalyEvent();
    QString formatTimestamp();
};
#endif
