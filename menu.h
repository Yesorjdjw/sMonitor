#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QTimer>
#include <QPropertyAnimation>
#include <QDebug>
#include <QThread>
#include "steng.h"
#include "fileIndex.h"
#include "VideoWorker.h"
#include "setup.h"

namespace Ui {
class menu;
}

class menu : public QWidget
{
    Q_OBJECT

public:
    explicit menu(QWidget *parent = 0);
    ~menu();

private slots:
    /* 导航按钮 */
    void on_swt_Control_bt_clicked();
    void on_exp_Check_bt_clicked();
    void on_file_Manage_bt_clicked();
    void on_mode_Setup_bt_clicked();

    /* PTZ 云台 */
    void on_turn_up_bt_pressed();
    void on_turn_up_bt_released();
    void on_turn_down_bt_pressed();
    void on_turn_down_bt_released();
    void on_turn_left_bt_pressed();
    void on_turn_left_bt_released();
    void on_turn_right_bt_pressed();
    void on_turn_right_bt_released();
    void on_turn_reset_bt_clicked();

    /* 摄像头 */
    void on_get_photo_bt_pressed();
    void on_get_vedio_bt_clicked();

    /* 录像计时 / 自动录像 */
    void updateRecTime();
    void onAutoRecordToggled(bool on);
    void onFlashHide();

    /* Worker 信号接收 */
    void onFrameReady(const QImage &img);
    void onPhotoSaved(const QString &path);
    void onRecordingStarted(const QString &path);
    void onRecordingStopped();
    void onCameraError(const QString &msg);

private:
    Ui::menu *ui;
    setEng sEng;

    // Single-Instance Lazy Loading for Setup
    setup *m_setup;

    /* 后台线程与 OpenCV */
    QThread *m_workerThread;
    VideoWorker *m_worker;

    /* 录像 */
    bool m_isRecording;
    QString m_lastVideoPath;
    int m_photoCount;
    int m_videoCount;

    /* 录像计时 */
    QTimer *m_recTimer;
    int     m_recSeconds;
};

#endif // MENU_H
