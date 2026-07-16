#ifndef MULTIMONITORWIDGET_H
#define MULTIMONITORWIDGET_H

#include <QWidget>
#include <QByteArray>

class MonitorCell;

// ---- 多路分屏监控页面（2x2 四宫格） ----
class MultiMonitorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MultiMonitorWidget(QWidget *parent = nullptr);

    // 实时视频帧路由到通道1
    void updateVideoFrame(const QByteArray &jpegData);

signals:
    void cellClicked(int channel);

private:
    MonitorCell *m_cells[4];
};

#endif // MULTIMONITORWIDGET_H
