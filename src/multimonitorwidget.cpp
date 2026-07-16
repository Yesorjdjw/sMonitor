#include "multimonitorwidget.h"

#include <QGridLayout>
#include <QPainter>
#include <QMouseEvent>
#include <QImage>

// ============================================================
// MonitorCell — 单个监控窗口（一路通道）
// ============================================================
class MonitorCell : public QWidget
{
    Q_OBJECT
public:
    explicit MonitorCell(int channel, QWidget *parent = nullptr)
        : QWidget(parent), m_channel(channel)
    {
        setMinimumSize(160, 120);
        setCursor(Qt::PointingHandCursor);
    }

    void setFrame(const QByteArray &jpeg)
    {
        QImage img;
        if (img.loadFromData(jpeg, "JPEG")) {
            m_frame = QPixmap::fromImage(img);
            m_online = true;
            update();
        }
    }

signals:
    void clicked(int channel);

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        if (m_online && !m_frame.isNull()) {
            // ---- 有信号：实景画面居中铺展 ----
            p.fillRect(rect(), Qt::black);
            QPixmap scaled = m_frame.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            int x = (width() - scaled.width()) / 2;
            int y = (height() - scaled.height()) / 2;
            p.drawPixmap(x, y, scaled);
        } else {
            // ---- 无信号：中性灰底 + 品牌图形（与单路占位统一） ----
            p.fillRect(rect(), QColor("#2a2a2e"));
            int cx = width() / 2, cy = height() / 2;
            p.setBrush(QColor(74, 144, 217, 60));
            p.setPen(QPen(QColor("#4a90d9"), 2));
            QPolygonF tri;
            tri << QPointF(cx, cy - 28) << QPointF(cx - 30, cy + 32) << QPointF(cx + 30, cy + 32);
            p.drawPolygon(tri);
            p.drawEllipse(QPointF(cx, cy + 4), 15, 15);
            p.setPen(QColor("#6a8aaa"));
            QFont f = font();
            f.setPixelSize(13);
            p.setFont(f);
            p.drawText(QRect(0, cy + 46, width(), 22), Qt::AlignCenter,
                       QString("通道 %1 · 无信号").arg(m_channel));
        }

        // ---- 通道角标 ----
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(0, 0, 0, 110));
        p.drawRoundedRect(QRect(8, 8, 52, 22), 3, 3);
        p.setPen(Qt::white);
        QFont bf = font();
        bf.setPixelSize(12);
        bf.setBold(true);
        p.setFont(bf);
        p.drawText(QRect(8, 8, 52, 22), Qt::AlignCenter, QString("CH%1").arg(m_channel));
    }

    void mousePressEvent(QMouseEvent *) override
    {
        emit clicked(m_channel);
    }

private:
    int m_channel;
    bool m_online = false;
    QPixmap m_frame;
};

// ============================================================
// MultiMonitorWidget — 四宫格
// ============================================================
MultiMonitorWidget::MultiMonitorWidget(QWidget *parent)
    : QWidget(parent)
{
    // 白色背景透过格间距形成细白线分隔
    setStyleSheet("background-color: #ffffff;");

    QGridLayout *grid = new QGridLayout(this);
    grid->setContentsMargins(2, 2, 2, 2);
    grid->setSpacing(2);

    for (int i = 0; i < 4; i++) {
        m_cells[i] = new MonitorCell(i + 1, this);
        connect(m_cells[i], &MonitorCell::clicked, this, &MultiMonitorWidget::cellClicked);
        grid->addWidget(m_cells[i], i / 2, i % 2);
    }

    grid->setRowStretch(0, 1);
    grid->setRowStretch(1, 1);
    grid->setColumnStretch(0, 1);
    grid->setColumnStretch(1, 1);
}

void MultiMonitorWidget::updateVideoFrame(const QByteArray &jpegData)
{
    m_cells[0]->setFrame(jpegData); // 通道1为实时视频源
}

#include "multimonitorwidget.moc"
