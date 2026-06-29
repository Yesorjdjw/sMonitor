#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QTimer>
#include <QPropertyAnimation>
#include <QDebug>
#include <opencv2/opencv.hpp>
#include <fops.h>
#include "steng.h"

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
    void readFrame();

    /* 录像计时 / 时间水印 / 自动录像 */
    void updateRecTime();
    void updateClock();
    void onAutoRecordToggled(bool on);
    void onFlashHide();

private:
    Ui::menu *ui;
    setEng sEng;

    /* OpenCV 摄像头 */
    QTimer *m_capTimer;
    cv::VideoCapture m_cap;
    cv::Mat m_frame;

    /* 录像 */
    cv::VideoWriter m_writer;
    bool m_isRecording;
    QString m_lastVideoPath;
    int m_photoCount;
    int m_videoCount;

    /* 录像计时 */
    QTimer *m_recTimer;
    int     m_recSeconds;

    /* 时间水印 */
    QTimer *m_clockTimer;

    QString formatTimestamp();
};

#endif // MENU_H
