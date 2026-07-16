#include "monitorwidget.h"
#include "ui_monitorwidget.h"

#include <QVBoxLayout>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QImage>

// ============================================================
// PTZControlWidget — 十字方向键（全 QPainter 绘制）
// ============================================================
PTZControlWidget::PTZControlWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(200, 200);
    setMaximumSize(260, 260);
    setCursor(Qt::PointingHandCursor);
}

void PTZControlWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    int cx = width() / 2;
    int cy = height() / 2;
    int sz = 56;
    int gap = 4;

    m_upRect    = QRect(cx - sz/2, cy - sz - gap - sz/2, sz, sz);
    m_downRect  = QRect(cx - sz/2, cy + gap + sz/2, sz, sz);
    m_leftRect  = QRect(cx - sz - gap - sz/2, cy - sz/2, sz, sz);
    m_rightRect = QRect(cx + gap + sz/2, cy - sz/2, sz, sz);
    m_homeRect  = QRect(cx - sz/2, cy - sz/2, sz, sz);

    struct { QRect rect; QString label; int dir; } btns[] = {
        {m_upRect,    "▲", 0},
        {m_downRect,  "▼", 1},
        {m_leftRect,  "◀", 2},
        {m_rightRect, "▶", 3},
        {m_homeRect,  "⊙", 4},
    };

    for (auto &b : btns) {
        bool pressed = (m_pressedDir == b.dir);
        bool isHome  = (b.dir == 4);
        QColor bg   = pressed ? (isHome ? QColor("#dce0e6") : QColor("#2a6ab8"))
                              : (isHome ? QColor("#e8ecf0") : QColor("#4a90d9"));
        QColor border = isHome ? QColor("#4a90d9") : QColor("#3a7bc8");
        QColor txt    = isHome ? QColor("#4a90d9") : Qt::white;

        p.setBrush(bg);
        p.setPen(QPen(border, pressed ? 1 : 2));
        p.drawRoundedRect(b.rect, 8, 8);

        QFont f = font();
        f.setPixelSize(24);
        f.setBold(true);
        p.setFont(f);
        p.setPen(txt);
        p.drawText(b.rect, Qt::AlignCenter, b.label);
    }

    // Cross lines
    p.setPen(QPen(QColor("#c0c8d4"), 2, Qt::DotLine));
    p.drawLine(cx, m_upRect.bottom() + gap, cx, m_downRect.top() - gap);
    p.drawLine(m_leftRect.right() + gap, cy, m_rightRect.left() - gap, cy);
}

void PTZControlWidget::mousePressEvent(QMouseEvent *e)
{
    QPoint pos = e->pos();
    if      (m_upRect.contains(pos))    m_pressedDir = 0;
    else if (m_downRect.contains(pos))  m_pressedDir = 1;
    else if (m_leftRect.contains(pos))  m_pressedDir = 2;
    else if (m_rightRect.contains(pos)) m_pressedDir = 3;
    else if (m_homeRect.contains(pos))  m_pressedDir = 4;
    else { QWidget::mousePressEvent(e); return; }
    update();

    QStringList d = {"up", "down", "left", "right", "home"};
    emit directionClicked(d[m_pressedDir]);
}

void PTZControlWidget::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
    if (m_pressedDir >= 0 && m_pressedDir < 4) {
        QStringList d = {"up", "down", "left", "right"};
        emit directionClicked(d[m_pressedDir] + "_off");
    }
    m_pressedDir = -1;
    update();
}

// ============================================================
// MonitorWidget — 实时监控页面
// ============================================================
MonitorWidget::MonitorWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::MonitorWidget)
{
    ui->setupUi(this);

    // Set stretch factors (Qt 5.5 uic doesn't support item stretch attribute)
    ui->rootLayout->setStretch(0, 7); // video container
    ui->rootLayout->setStretch(1, 3); // control panel

    // Draw placeholder logo on video label
    QPixmap ph(320, 240);
    ph.fill(QColor("#2a2a2e"));
    QPainter pp(&ph);
    pp.setRenderHint(QPainter::Antialiasing);
    pp.setBrush(QColor(74, 144, 217, 60));
    pp.setPen(QPen(QColor("#4a90d9"), 2));
    QPolygonF tri;
    tri << QPointF(160, 80) << QPointF(100, 200) << QPointF(220, 200);
    pp.drawPolygon(tri);
    pp.drawEllipse(QPointF(160, 140), 30, 30);
    pp.setPen(QColor("#6a8aaa"));
    QFont pf = pp.font();
    pf.setPixelSize(16);
    pp.setFont(pf);
    pp.drawText(QRect(0, 210, 320, 30), Qt::AlignCenter, "等待视频信号...");
    pp.end();
    ui->videoLabel->setPixmap(ph);

    // Build PTZ control
    buildPTZControl();

    // 保存录像按钮原始样式,用于停止录像时还原
    m_recordBtnStyle = ui->recordBtn->styleSheet();

    // Connect function buttons
    connect(ui->snapBtn, &QPushButton::clicked, [this]() {
        qDebug() << "拍照";
        emit ptzCommand("snap");
    });
    connect(ui->recordBtn, &QPushButton::clicked, [this]() {
        qDebug() << "录像";
        emit ptzCommand("record");
    });
    connect(ui->autoBtn, &QPushButton::clicked, [this]() {
        qDebug() << "自动模式";
        emit ptzCommand("auto");
    });
    connect(ui->fullBtn, &QPushButton::clicked, [this]() {
        qDebug() << "返回全监";
        emit ptzCommand("full");
    });
}

MonitorWidget::~MonitorWidget()
{
    delete ui;
}

void MonitorWidget::setRecordingState(bool recording)
{
    if (recording) {
        ui->recordBtn->setText("● 录像中");
        ui->recordBtn->setStyleSheet(
            "QPushButton {"
            "  background-color: #e74c3c;"
            "  color: white;"
            "  border: 1px solid #c0392b;"
            "  border-radius: 6px;"
            "  font-size: 15px;"
            "  font-weight: bold;"
            "}"
            "QPushButton:hover { background-color: #d44333; }"
            "QPushButton:pressed { background-color: #c0392b; }");
    } else {
        ui->recordBtn->setText("录  像");
        ui->recordBtn->setStyleSheet(m_recordBtnStyle);
    }
}

void MonitorWidget::buildPTZControl()
{
    // Replace placeholder widget with actual PTZControlWidget
    QWidget *placeholder = ui->ptzPlaceholder;
    QVBoxLayout *ly = new QVBoxLayout(placeholder);
    ly->setContentsMargins(0, 0, 0, 0);

    m_ptzCtrl = new PTZControlWidget(placeholder);
    ly->addWidget(m_ptzCtrl, 0, Qt::AlignCenter);

    connect(m_ptzCtrl, &PTZControlWidget::directionClicked, [this](const QString &dir) {
        qDebug() << "PTZ:" << dir;
        if (dir == "home") {
            emit ptzCommand("reset");
        } else if (dir.endsWith("_off")) {
            QString d = dir.left(dir.size() - 4);
            emit ptzCommand(d + ".off");
        } else {
            emit ptzCommand(dir + ".on");
        }
    });
}

void MonitorWidget::updateVideoFrame(const QByteArray &jpegData)
{
    QImage img;
    if (img.loadFromData(jpegData, "JPEG")) {
        QPixmap pix = QPixmap::fromImage(img);
        QSize ls = ui->videoLabel->size();
        if (ls.isValid()) {
            pix = pix.scaled(ls, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }
        ui->videoLabel->setPixmap(pix);
    }
}
