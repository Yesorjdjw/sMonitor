#ifndef FACELOGINDIALOG_H
#define FACELOGINDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include <opencv2/opencv.hpp>

class FaceEngine;

class FaceLoginDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FaceLoginDialog(FaceEngine *engine, QWidget *parent = nullptr);
    ~FaceLoginDialog();
    QString loggedInUser() const { return m_loggedInUser; }

protected:
    void closeEvent(QCloseEvent *e) override;

private slots:
    void onFrameCaptured();
    void onRetryClicked();
    void onRegisterClicked();
    void onBackClicked();

private:
    enum State { SCANNING, DETECTED, SUCCESS, FAILED };
    void setState(State s);

    FaceEngine *m_engine;
    cv::VideoCapture m_cap;
    QTimer *m_timer;
    QLabel *m_cameraLabel;
    QLabel *m_statusLabel;
    QLabel *m_nameLabel;
    QPushButton *m_retryBtn;
    QPushButton *m_registerBtn;
    QPushButton *m_backBtn;
    State m_state;
    QString m_loggedInUser;
    int m_failCount;
};

#endif
