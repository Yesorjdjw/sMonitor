#ifndef MONITORWIDGET_H
#define MONITORWIDGET_H

#include <QWidget>
#include <QByteArray>

namespace Ui { class MonitorWidget; }

// ---- PTZ 十字方向键（全 QPainter 自定义控件，无 .ui） ----
class PTZControlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PTZControlWidget(QWidget *parent = nullptr);

signals:
    void directionClicked(const QString &direction);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QRect m_upRect, m_downRect, m_leftRect, m_rightRect, m_homeRect;
    int m_pressedDir = -1;
};

// ---- 实时监控页面 ----
class MonitorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MonitorWidget(QWidget *parent = nullptr);
    ~MonitorWidget();

    void updateVideoFrame(const QByteArray &jpegData);
    void setRecordingState(bool recording);

signals:
    void ptzCommand(const QString &cmd);

private:
    void buildPTZControl();

    Ui::MonitorWidget *ui;
    PTZControlWidget *m_ptzCtrl;
    QString m_recordBtnStyle;   // recordBtn 原始样式,停止录像时还原
};

#endif // MONITORWIDGET_H
