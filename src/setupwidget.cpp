#include "setupwidget.h"
#include "ui_setupwidget.h"
#include "configmanager.h"
#include "mqttmanager.h"

#include <QMessageBox>
#include <QTime>
#include <QDebug>

SetupWidget::SetupWidget(ConfigManager *cfg, QWidget *parent)
    : QScrollArea(parent), ui(new Ui::SetupWidget), m_cfg(cfg)
{
    ui->setupUi(this);

    // --- 全选/取消全选 ---
    connect(ui->btnSelectAll, &QPushButton::clicked, [this]() {
        bool all = ui->ckMotion->isChecked() && ui->ckCover->isChecked()
                && ui->ckIntrude->isChecked() && ui->ckStorage->isChecked()
                && ui->ckFace->isChecked();
        bool set = !all;
        ui->ckMotion->setChecked(set);
        ui->ckCover->setChecked(set);
        ui->ckIntrude->setChecked(set);
        ui->ckStorage->setChecked(set);
        ui->ckFace->setChecked(set);
        ui->btnSelectAll->setText(set ? "取消全选" : "全选");
    });

    // --- TTS 循环播报联动 ---
    connect(ui->ttsLoopEnabled, &QCheckBox::toggled, [this](bool on) {
        ui->ttsLoopCountLabel->setEnabled(on);
        ui->ttsLoopCount->setEnabled(on);
        ui->ttsLoopIntervalLabel->setEnabled(on);
        ui->ttsLoopInterval->setEnabled(on);
    });

    // --- TTS 恢复默认 ---
    connect(ui->ttsResetBt, &QPushButton::clicked, [this]() {
        ui->ttsPort->setText("/dev/ttySAC2");
        ui->ttsBaud->setCurrentText("9600");
        ui->ttsSpeaker->setCurrentText("m3");
        ui->ttsVolume->setCurrentText("5");
        ui->ttsSpeed->setCurrentText("5");
        ui->ttsLoopEnabled->setChecked(true);
        ui->ttsLoopCount->setValue(2);
        ui->ttsLoopInterval->setValue(10);
    });

    // --- 保存/重置 ---
    connect(ui->saveBt,  &QPushButton::clicked, this, &SetupWidget::onSave);
    connect(ui->resetBt, &QPushButton::clicked, this, &SetupWidget::onReset);

    loadConfig();
    applyTtsLoopLinkage();
}

SetupWidget::~SetupWidget() { delete ui; }

void SetupWidget::setMqttManager(MqttManager *mqtt) { m_mqtt = mqtt; }

void SetupWidget::loadConfig()
{
    if (!m_cfg) return;
    ui->ckMotion->setChecked(m_cfg->alarmMotion());
    ui->ckCover->setChecked(m_cfg->alarmCover());
    ui->ckIntrude->setChecked(m_cfg->alarmIntrude());
    ui->ckStorage->setChecked(m_cfg->alarmStorage());
    ui->ckFace->setChecked(m_cfg->alarmFace());
    ui->linkCombo->setCurrentIndex(m_cfg->alarmLinkage());
    ui->timeStart->setTime(m_cfg->alarmTimeStart());
    ui->timeEnd->setTime(m_cfg->alarmTimeEnd());

    // TTS
    ui->ttsPort->setText(m_cfg->ttsPort());
    ui->ttsBaud->setCurrentText(QString::number(m_cfg->ttsBaud()));
    ui->ttsSpeaker->setCurrentText(m_cfg->ttsSpeaker());
    ui->ttsVolume->setCurrentText(QString::number(m_cfg->ttsVolume()));
    ui->ttsSpeed->setCurrentText(QString::number(m_cfg->ttsSpeed()));
    ui->ttsLoopEnabled->setChecked(m_cfg->ttsLoopEnabled());
    ui->ttsLoopCount->setValue(m_cfg->ttsLoopCount());
    ui->ttsLoopInterval->setValue(m_cfg->ttsLoopInterval());
}

void SetupWidget::applyTtsLoopLinkage()
{
    bool on = ui->ttsLoopEnabled->isChecked();
    ui->ttsLoopCountLabel->setEnabled(on);
    ui->ttsLoopCount->setEnabled(on);
    ui->ttsLoopIntervalLabel->setEnabled(on);
    ui->ttsLoopInterval->setEnabled(on);
}

void SetupWidget::onSave()
{
    if (!m_cfg) return;

    // 校验布防时段
    if (ui->timeStart->time() > ui->timeEnd->time()) {
        QMessageBox::warning(this, "参数校验失败",
            "布防起始时间不能晚于结束时间，请重新设置。");
        ui->timeStart->setFocus();
        return;
    }

    // 校验波特率
    int baud = ui->ttsBaud->currentText().toInt();
    if (baud <= 0) {
        QMessageBox::warning(this, "参数校验失败", "波特率格式无效。");
        ui->ttsBaud->setFocus();
        return;
    }

    m_cfg->setAlarmMotion(ui->ckMotion->isChecked());
    m_cfg->setAlarmCover(ui->ckCover->isChecked());
    m_cfg->setAlarmIntrude(ui->ckIntrude->isChecked());
    m_cfg->setAlarmStorage(ui->ckStorage->isChecked());
    m_cfg->setAlarmFace(ui->ckFace->isChecked());
    m_cfg->setAlarmLinkage(ui->linkCombo->currentIndex());
    m_cfg->setAlarmTimeStart(ui->timeStart->time());
    m_cfg->setAlarmTimeEnd(ui->timeEnd->time());

    // TTS
    m_cfg->setTtsPort(ui->ttsPort->text().trimmed());
    m_cfg->setTtsBaud(baud);
    m_cfg->setTtsSpeaker(ui->ttsSpeaker->currentText().section(' ', 0, 0));
    m_cfg->setTtsVolume(ui->ttsVolume->currentText().toInt());
    m_cfg->setTtsSpeed(ui->ttsSpeed->currentText().toInt());
    m_cfg->setTtsLoopEnabled(ui->ttsLoopEnabled->isChecked());
    m_cfg->setTtsLoopCount(ui->ttsLoopCount->value());
    m_cfg->setTtsLoopInterval(ui->ttsLoopInterval->value());
    m_cfg->saveConfig();

    // MQTT 下发
    if (m_mqtt && m_mqtt->isConnected()) {
        QString json = m_cfg->toJson();
        m_mqtt->publish("/ALARM_CFG", json.toUtf8());
        qDebug() << "MQTT /ALARM_CFG published:" << json;
    }

    QMessageBox::information(this, "保存成功", "告警策略配置已保存。");
}

void SetupWidget::onReset()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "确认重置",
        "确定要重置所有参数为默认值吗？\n当前未保存的修改将丢失。",
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (reply != QMessageBox::Yes)
        return;

    ui->ckMotion->setChecked(true);
    ui->ckCover->setChecked(true);
    ui->ckIntrude->setChecked(false);
    ui->ckStorage->setChecked(true);
    ui->ckFace->setChecked(true);
    ui->linkCombo->setCurrentIndex(0);
    ui->timeStart->setTime(QTime(0, 0));
    ui->timeEnd->setTime(QTime(23, 59));
    ui->ttsPort->setText("/dev/ttySAC2");
    ui->ttsBaud->setCurrentText("9600");
    ui->ttsSpeaker->setCurrentText("m3");
    ui->ttsVolume->setCurrentText("5");
    ui->ttsSpeed->setCurrentText("5");
    ui->ttsLoopEnabled->setChecked(true);
    ui->ttsLoopCount->setValue(2);
    ui->ttsLoopInterval->setValue(10);
}