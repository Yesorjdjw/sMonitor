#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "monitorwidget.h"
#include "filemanagerwidget.h"
#include "alarmwidget.h"
#include "setupwidget.h"
#include "mqttmanager.h"
#include "databasemanager.h"
#include "configmanager.h"

#include <QPainter>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QStackedWidget>
#include <QDebug>

MainWindow::MainWindow(const QString &username, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_username(username)
{
    ui->setupUi(this);
    setWindowTitle("安防管理系统");
    resize(1280, 800);
    show();

    // Draw logo
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
    // 监控页：单路监控
    m_monitorWidget = new MonitorWidget(this);
    connect(m_monitorWidget, &MonitorWidget::ptzCommand, this, &MainWindow::onControlCommand);

    m_fileWidget = new FileManagerWidget(this);

    // 报警信息(异常查看)与系统管理(告警策略)—— 由 v2 的 unusual/setup 整合而来
    m_cfg = new ConfigManager("/opt/aicTrain/sMonitor_server", this);
    m_alarmWidget = new AlarmWidget(this);
    m_setupWidget = new SetupWidget(m_cfg, this);

    ui->contentStack->addWidget(m_monitorWidget);
    ui->contentStack->addWidget(m_fileWidget);
    ui->contentStack->addWidget(m_alarmWidget);   // index 2 = 报警信息
    ui->contentStack->addWidget(m_setupWidget);   // index 3 = 系统管理
}

void MainWindow::setupConnections()
{
    connect(ui->tabMonitor, &QPushButton::clicked, [this]() { switchPage(0); });
    connect(ui->tabFiles,   &QPushButton::clicked, [this]() { switchPage(1); });
    connect(ui->tabAlarm,   &QPushButton::clicked, [this]() { switchPage(2); });
    connect(ui->tabSettings,&QPushButton::clicked, [this]() { switchPage(3); });

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

    // 切到文件页时刷新列表，切到告警页时刷新告警
    if (index == 1 && m_fileWidget)
        m_fileWidget->refreshFiles();
    else if (index == 2 && m_alarmWidget)
        m_alarmWidget->refresh();

    qDebug() << "Page:" << index;
}

void MainWindow::updateClock()
{
    ui->clockLabel->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd  dddd  HH:mm:ss"));
}

void MainWindow::setMqttManager(MqttManager *mgr)
{
    m_mqtt = mgr;
    if (!m_mqtt) return;
    connect(m_mqtt, &MqttManager::messageReceived, this, &MainWindow::onMqttMessage);
    connect(m_mqtt, &MqttManager::connected, [this]() {
        ui->statusbar->showMessage("系统就绪  |  MQTT: 已连接");
        ui->statusbar->setStyleSheet("QStatusBar { background-color: #e8f0fe; border-top: 1px solid #d0d9e8; font-size: 12px; color: #27ae60; }");
        subscribeTopics();
    });
    connect(m_mqtt, &MqttManager::disconnected, [this]() {
        ui->statusbar->showMessage("系统就绪  |  MQTT: 已断开");
        ui->statusbar->setStyleSheet("QStatusBar { background-color: #e8f0fe; border-top: 1px solid #d0d9e8; font-size: 12px; color: #e74c3c; }");
    });

    // 若创建本窗口时 MQTT 已连上(登录流程),connected 信号已错过,立即补订阅+开流
    if (m_mqtt->isConnected())
        subscribeTopics();
    else
        QTimer::singleShot(2000, this, [this]() { if (m_mqtt && m_mqtt->isConnected()) subscribeTopics(); });
}

void MainWindow::subscribeTopics()
{
    if (!m_mqtt) return;
    m_mqtt->subscribe("/VIDEO_DATA");   // 直播
    m_mqtt->subscribe("/MEDIA_NEW");    // 中心库新增通知(实时同步)
    m_mqtt->subscribe("/MEDIA_DELETE"); // 中心库删除通知
    m_mqtt->subscribe("/ALARM");        // 开发板报警事件
    m_mqtt->publish("/VIDEO_CMD", "start"); // 通知设备开始推流
    qDebug() << "MQTT topics subscribed";
}

void MainWindow::setDatabaseManager(DatabaseManager *db)
{
    m_db = db;
    if (m_fileWidget) m_fileWidget->setDatabase(m_db);
    if (m_alarmWidget) m_alarmWidget->setDatabase(m_db);
    if (m_setupWidget) m_setupWidget->setMqttManager(m_mqtt);
}

MonitorWidget *MainWindow::monitorWidget() const { return m_monitorWidget; }
FileManagerWidget *MainWindow::fileWidget() const { return m_fileWidget; }

void MainWindow::onMqttMessage(const QString &topic, const QByteArray &payload)
{
    if (topic == "/VIDEO_DATA") {
        m_monitorWidget->updateVideoFrame(payload);
    } else if (topic == "/MEDIA_NEW" || topic == "/MEDIA_DELETE") {
        // 中心库变更(本端或它端触发)→ 刷新文件列表 + 告警缩略图
        Q_UNUSED(payload);
        if (m_fileWidget) m_fileWidget->refreshFiles();
        if (m_alarmWidget) m_alarmWidget->refresh();
    } else if (topic == "/ALARM") {
        // 开发板报警: {"type":"cover","desc":"画面遮挡","level":"紧急","ts":"..."}
        QJsonDocument doc = QJsonDocument::fromJson(payload);
        if (doc.isObject()) {
            QJsonObject obj = doc.object();
            EventRecord evt;
            evt.type = obj.value("type").toString("unknown");
            evt.description = obj.value("desc").toString();
            evt.thumbnail = obj.value("thumb").toString();
            // 开发板发英文 level，统一转为中文 UI 可识别的值
            QString rawLevel = obj.value("level").toString("一般");
            if (rawLevel == "Critical" || rawLevel == "紧急")      evt.level = "紧急";
            else if (rawLevel == "Warning" || rawLevel == "重要")  evt.level = "重要";
            else evt.level = "一般";
            if (m_db) {
                bool ok = m_db->addEvent(evt);
                qDebug() << "ALARM received:" << evt.type << evt.description << "db_ok=" << ok;
            }
            if (m_alarmWidget) {
                m_alarmWidget->refresh();
                qDebug() << "alarmWidget refreshed, events count:" << (m_db ? m_db->getEventCount("") : -1);
            }
            // 触发设备端语音播报
            triggerVoiceAlert(evt.type, evt.description);
        }
    }
}

void MainWindow::onControlCommand(const QString &cmd)
{
    if (!m_mqtt) return;

    if (cmd == "snap") {
        m_mqtt->publish("/PHOTO_CMD", "snap");                       // 拍照
    } else if (cmd == "record") {
        m_recording = !m_recording;                                 // 录像开关翻转
        m_mqtt->publish("/RECORD_CMD", m_recording ? "start" : "stop");
        m_monitorWidget->setRecordingState(m_recording);            // 更新按钮状态反馈
    } else if (cmd == "auto") {
        // 设备端暂无自动巡航指令,忽略
    } else {
        // 云台方向(up.on/up.off/…) 与复位(reset) → /PTZCTL
        m_mqtt->publish("/PTZCTL", cmd.toUtf8());
    }
}

void MainWindow::triggerVoiceAlert(const QString &type, const QString &description)
{
    Q_UNUSED(description);
    if (!m_mqtt || !m_mqtt->isConnected() || !m_cfg) return;

    // 根据告警策略开关决定是否播报
    if (type == "cover"   && !m_cfg->alarmCover())   return;
    if (type == "intrude" && !m_cfg->alarmIntrude()) return;
    if (type == "motion"  && !m_cfg->alarmMotion())  return;
    if (type == "storage" && !m_cfg->alarmStorage()) return;
    if (type == "face"    && !m_cfg->alarmFace())    return;

    // 始终使用中文文本（SYN6658 芯片需要 GBK 编码的中文）
    QString text;
    if (type == "cover")      text = QString::fromUtf8("画面遮挡告警");
    else if (type == "intrude") text = QString::fromUtf8("区域入侵告警");
    else if (type == "motion")  text = QString::fromUtf8("运动检测告警");
    else if (type == "storage") text = QString::fromUtf8("存储空间告警");
    else if (type == "face")    text = QString::fromUtf8("警戒区域，立即离开");
    else                        text = QString::fromUtf8("安防告警");

    qDebug() << "Voice alert triggered:" << type << text;
    m_mqtt->publish("/TTS_CMD", text.toUtf8());
}
