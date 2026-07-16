#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "monitorwidget.h"
#include "filemanagerwidget.h"
#include "mqttmanager.h"
#include "databasemanager.h"

#include <QPainter>
#include <QDateTime>
#include <QFile>
#include <QDebug>

MainWindow::MainWindow(const QString &username, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_username(username)
{
    ui->setupUi(this);
    setWindowTitle("安防管理系统");

    setWindowFlags(Qt::FramelessWindowHint);
    showMaximized();

    // Draw logo programmatically onto the logoLabel
    QPixmap logo(36, 36);
    logo.fill(Qt::transparent);
    QPainter lp(&logo);
    lp.setRenderHint(QPainter::Antialiasing);
    lp.setBrush(QColor("#4a90d9"));
    lp.setPen(Qt::NoPen);
    lp.drawEllipse(0, 0, 36, 36);
    lp.setBrush(Qt::white);
    QPolygonF tri;
    tri << QPointF(12, 26) << QPointF(12, 10) << QPointF(24, 18);
    lp.drawPolygon(tri);
    lp.end();
    ui->logoLabel->setPixmap(logo);

    // Set username
    ui->userLabel->setText(QString("当前登录：系统管理员 %1").arg(username));

    setupPages();
    setupConnections();
    updateClock();
    switchPage(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupPages()
{
    m_monitorWidget = new MonitorWidget(this);
    m_fileWidget = new FileManagerWidget(this);

    // Placeholder pages
    QWidget *alarmPage = new QWidget(this);
    alarmPage->setStyleSheet("background-color: #f5f6f8;");
    QLabel *alarmLabel = new QLabel("报警信息管理（开发中）", alarmPage);
    alarmLabel->setAlignment(Qt::AlignCenter);
    alarmLabel->setStyleSheet("color: #8a9aaa; font-size: 20px; background: transparent;");

    QWidget *sysPage = new QWidget(this);
    sysPage->setStyleSheet("background-color: #f5f6f8;");
    QLabel *sysLabel = new QLabel("系统设置（开发中）", sysPage);
    sysLabel->setAlignment(Qt::AlignCenter);
    sysLabel->setStyleSheet("color: #8a9aaa; font-size: 20px; background: transparent;");

    // Layouts for placeholder pages
    QVBoxLayout *alarmLy = new QVBoxLayout(alarmPage);
    alarmLy->addWidget(alarmLabel);
    QVBoxLayout *sysLy = new QVBoxLayout(sysPage);
    sysLy->addWidget(sysLabel);

    ui->contentStack->addWidget(m_monitorWidget);  // 0: 实时监控
    ui->contentStack->addWidget(m_fileWidget);     // 1: 录像照片
    ui->contentStack->addWidget(alarmPage);        // 2: 报警信息
    ui->contentStack->addWidget(sysPage);          // 3: 系统管理
}

void MainWindow::setupConnections()
{
    // Tab buttons
    connect(ui->tabMonitor, &QPushButton::clicked, [this]() { switchPage(0); });
    connect(ui->tabFiles,   &QPushButton::clicked, [this]() { switchPage(1); });
    connect(ui->tabAlarm,   &QPushButton::clicked, [this]() { switchPage(2); });
    connect(ui->tabSettings,&QPushButton::clicked, [this]() { switchPage(3); });

    // Clock timer
    m_clockTimer = new QTimer(this);
    connect(m_clockTimer, &QTimer::timeout, this, &MainWindow::updateClock);
    m_clockTimer->start(1000);
}

void MainWindow::switchPage(int index)
{
    if (index < 0 || index > 3) return;
    ui->contentStack->setCurrentIndex(index);

    QPushButton *tabs[] = {ui->tabMonitor, ui->tabFiles, ui->tabAlarm, ui->tabSettings};
    for (int i = 0; i < 4; i++)
        tabs[i]->setChecked(i == index);

    qDebug() << "Page:" << index;
}

void MainWindow::updateClock()
{
    QDateTime now = QDateTime::currentDateTime();
    ui->clockLabel->setText(now.toString("yyyy-MM-dd  dddd  HH:mm:ss"));
}

void MainWindow::setMqttManager(MqttManager *mgr)
{
    m_mqtt = mgr;
    if (!m_mqtt) return;

    connect(m_mqtt, &MqttManager::messageReceived,
            this, &MainWindow::onMqttMessage);
    connect(m_mqtt, &MqttManager::connected, [this]() {
        ui->statusbar->showMessage("系统就绪  |  MQTT: 已连接");
        ui->statusbar->setStyleSheet(
            "QStatusBar { background-color: #e8f0fe; border-top: 1px solid #d0d9e8; "
            "font-size: 12px; color: #27ae60; }");
    });
    connect(m_mqtt, &MqttManager::disconnected, [this]() {
        ui->statusbar->showMessage("系统就绪  |  MQTT: 已断开");
        ui->statusbar->setStyleSheet(
            "QStatusBar { background-color: #e8f0fe; border-top: 1px solid #d0d9e8; "
            "font-size: 12px; color: #e74c3c; }");
    });
}

void MainWindow::setDatabaseManager(DatabaseManager *db)
{
    m_db = db;
    if (m_fileWidget)
        m_fileWidget->setDatabase(m_db);
}

MonitorWidget *MainWindow::monitorWidget() const { return m_monitorWidget; }
FileManagerWidget *MainWindow::fileWidget() const { return m_fileWidget; }

void MainWindow::onMqttMessage(const QString &topic, const QByteArray &payload)
{
    if (topic == "/VIDEO_DATA") {
        m_monitorWidget->updateVideoFrame(payload);
    } else if (topic == "/PHOTO_DATA") {
        QString fn = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss") + ".jpg";
        QString fp = "/opt/aicTrain/sMonitor_server/media/photos/" + fn;
        QFile f(fp);
        if (f.open(QIODevice::WriteOnly)) {
            f.write(payload);
            f.close();
            qDebug() << "Photo:" << fp;
            if (m_db) {
                MediaFile mf;
                mf.type = "photo";
                mf.filename = fn;
                mf.filepath = fp;
                mf.size = payload.size();
                m_db->addMediaFile(mf);
                m_fileWidget->refreshFiles();
            }
        }
    } else if (topic == "/RECORD_DATA") {
        QString fn = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss") + ".avi";
        QString fp = "/opt/aicTrain/sMonitor_server/media/recordings/" + fn;
        QFile f(fp);
        if (f.open(QIODevice::WriteOnly)) {
            f.write(payload);
            f.close();
            qDebug() << "Recording:" << fp;
            if (m_db) {
                MediaFile mf;
                mf.type = "recording";
                mf.filename = fn;
                mf.filepath = fp;
                mf.size = payload.size();
                m_db->addMediaFile(mf);
                m_fileWidget->refreshFiles();
            }
        }
    }
}
