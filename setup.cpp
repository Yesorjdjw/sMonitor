#include "setup.h"
#include "ui_setup.h"
#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <sys/statvfs.h>

#define CONFIG_FILE "/opt/aicTrain/sMonitor/sMonitor.cfg"

/* ========== 工具 ========== */

QString setup::fmtSize(qint64 bytes)
{
    if (bytes < 1024 * 1024)
        return QString::number(bytes / 1024.0, 'f', 1) + " KB";
    if (bytes < 1024LL * 1024 * 1024)
        return QString::number(bytes / (1024.0 * 1024), 'f', 1) + " MB";
    return QString::number(bytes / (1024.0 * 1024 * 1024), 'f', 2) + " GB";
}

qint64 setup::dirSize(const QString &path)
{
    qint64 total = 0;
    QDir dir(path);
    if (!dir.exists()) return 0;
    QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    foreach (const QFileInfo &fi, list)
        total += fi.size();
    return total;
}

/* ========== 构造/析构 ========== */

setup::setup(QWidget *parent) :
    QWidget(parent), ui(new Ui::setup), m_eng(0),
    m_storagePath("/opt/aicTrain/sMonitor/video"),
    m_autoRecord(false), m_retainDays(30), m_diskFullPolicy(0)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setGeometry(0, 0, 1024, 600);
    selectCategory(0);

    m_presets[0].lr = 60;  m_presets[0].ud = 30;  m_presets[0].enabled = false;
    m_presets[1].lr = 30;  m_presets[1].ud = 90;  m_presets[1].enabled = false;
    m_presets[2].lr = 32;  m_presets[2].ud = 100; m_presets[2].enabled = false;
    m_presets[3].lr = 20;  m_presets[3].ud = 70;  m_presets[3].enabled = false;
    m_cruiseIndex = 0; m_cruiseCount = 0; m_cruiseSpeed = 7; m_cruiseRunning = false;
    ui->toggleCruise->setChecked(false);

    connect(ui->speedSlider, SIGNAL(valueChanged(int)), this, SLOT(on_speedChanged(int)));
    connect(ui->toggleCruise, SIGNAL(clicked(bool)), this, SLOT(onToggleCruise(bool)));
    connect(ui->browseBt, SIGNAL(clicked()), this, SLOT(on_browseBt_clicked()));
    connect(ui->saveBtRec, SIGNAL(clicked()), this, SLOT(on_saveBtRec_clicked()));
    connect(ui->resetBtRec, SIGNAL(clicked()), this, SLOT(on_resetBtRec_clicked()));

    loadStorageConfig();
}

setup::~setup() { delete ui; }
void setup::on_backBt_clicked() { close(); }

/* ========== 导航 ========== */

void setup::selectCategory(int index)
{
    clearSelection();
    ui->contentStack->setCurrentIndex(index);
    QStringList titles;
    titles << QString::fromUtf8("巡航设置") << QString::fromUtf8("告警策略配置")
           << QString::fromUtf8("录像存储设置") << QString::fromUtf8("网络与设备管理")
           << QString::fromUtf8("用户权限管理");
    ui->rightTitleText->setText(titles[index]);
    QString sel = "background: #e8f0fe; border: none; text-align: left;"
                  "font-size: 13px; font-weight: 500; color: #333333; padding-left: 44px;";
    QPushButton *b = findChild<QPushButton*>(QString("nav%1").arg(index + 1));
    if (b) b->setStyleSheet(sel);
    if (index == 2) refreshStorageInfo();
}

void setup::clearSelection()
{
    QString n = "background: transparent; border: none; text-align: left;"
                "font-size: 13px; font-weight: 500; color: #333333; padding-left: 44px;";
    for (int i = 1; i <= 5; i++) {
        QPushButton *b = findChild<QPushButton*>(QString("nav%1").arg(i));
        if (b) b->setStyleSheet(n);
    }
}

void setup::on_nav1_clicked() { selectCategory(0); }
void setup::on_nav2_clicked() { selectCategory(1); }
void setup::on_nav3_clicked() { selectCategory(2); }
void setup::on_nav4_clicked() { selectCategory(3); }
void setup::on_nav5_clicked() { selectCategory(4); }
void setup::on_saveBt_clicked() { }
void setup::on_resetBt_clicked() { }

/* ========== 巡航 ========== */

void setup::on_speedChanged(int val) { ui->speedVal->setText(QString::number(val)); m_cruiseSpeed = val; if (m_eng) m_eng->setCruiseSpeed(val); }
void setup::onToggleCruise(bool checked) { if (checked) startCruise(); else stopCruise(); }

void setup::startCruise()
{
    int lrs[4], uds[4], c = 0;
    for (int i = 0; i < 4; i++) { if (m_presets[i].enabled) { lrs[c]=m_presets[i].lr; uds[c]=m_presets[i].ud; c++; } }
    if (c < 2) { stopCruise(); return; }
    m_cruiseRunning = true; m_cruiseCount = c; m_cruiseIndex = 0;
    if (m_eng) { disconnect(m_eng, SIGNAL(cruiseDone()), this, SLOT(onCruiseTick())); connect(m_eng, SIGNAL(cruiseDone()), this, SLOT(onCruiseTick())); m_eng->startCruise(lrs, uds, c, m_cruiseSpeed); }
}

void setup::stopCruise()
{
    m_cruiseRunning = false;
    if (m_eng) { disconnect(m_eng, SIGNAL(cruiseDone()), this, SLOT(onCruiseTick())); m_eng->setCruiseEnabled(false); m_eng->stop(); }
    if (ui->toggleCruise->isChecked()) ui->toggleCruise->setChecked(false);
}

void setup::onCruiseTick()
{
    if (!m_cruiseRunning || !m_eng) return;
    int lrs[4], uds[4], c = 0;
    for (int i = 0; i < 4; i++) { if (m_presets[i].enabled) { lrs[c]=m_presets[i].lr; uds[c]=m_presets[i].ud; c++; } }
    if (c < 2) { stopCruise(); return; }
    if (m_cruiseIndex >= c) m_cruiseIndex = 0;
    if (abs(m_eng->getLrAngle()-lrs[m_cruiseIndex])<=1 && abs(m_eng->getUdAngle()-uds[m_cruiseIndex])<=1) {
        if (++m_cruiseIndex >= c) m_cruiseIndex = 0;
        m_eng->gotoAngle(lrs[m_cruiseIndex], uds[m_cruiseIndex], m_cruiseSpeed);
    }
}

/* ========== 快捷键 ========== */

void setup::on_ckDoor_clicked()      { m_presets[0].enabled = ui->ckDoor->isChecked(); if (m_presets[0].enabled && m_eng) m_eng->gotoAngle(m_presets[0].lr, m_presets[0].ud, m_cruiseSpeed); if (ui->toggleCruise->isChecked()) startCruise(); }
void setup::on_ckWarehouse_clicked() { m_presets[1].enabled = ui->ckWarehouse->isChecked(); if (m_presets[1].enabled && m_eng) m_eng->gotoAngle(m_presets[1].lr, m_presets[1].ud, m_cruiseSpeed); if (ui->toggleCruise->isChecked()) startCruise(); }
void setup::on_ckParking_clicked()   { m_presets[2].enabled = ui->ckParking->isChecked(); if (m_presets[2].enabled && m_eng) m_eng->gotoAngle(m_presets[2].lr, m_presets[2].ud, m_cruiseSpeed); if (ui->toggleCruise->isChecked()) startCruise(); }
void setup::on_ckWall_clicked()      { m_presets[3].enabled = ui->ckWall->isChecked(); if (m_presets[3].enabled && m_eng) m_eng->gotoAngle(m_presets[3].lr, m_presets[3].ud, m_cruiseSpeed); if (ui->toggleCruise->isChecked()) startCruise(); }

/* ========== 存储容量刷新 ========== */

void setup::refreshStorageInfo()
{
    /* 1. 磁盘容量 */
    struct statvfs vfs;
    qint64 total = 0, free = 0;
    if (statvfs("/opt/aicTrain/sMonitor", &vfs) == 0) {
        total = (qint64)vfs.f_frsize * vfs.f_blocks;
        free  = (qint64)vfs.f_frsize * vfs.f_bavail;
    }
    if (total == 0) return;

    /* 2. 文件扫描 */
    qint64 vidB = dirSize("/opt/aicTrain/sMonitor/video");
    qint64 phoB = dirSize("/opt/aicTrain/sMonitor/photo");
    qint64 used = vidB + phoB;
    qint64 other = (total - free > used) ? (total - free - used) : 0;

    /* 3. 总览标签 */
    int freePct = (int)(free * 100 / total);
    ui->totalLabel->setText(
        QString::fromUtf8("总容量 %1  |  已用 %2  |  剩余 %3 (%4%)")
            .arg(fmtSize(total)).arg(fmtSize(total - free))
            .arg(fmtSize(free)).arg(freePct));

    /* 4. 进度条分段宽度 (bar 总宽 690px) */
    int barW = 690;
    int vidW = (int)((double)vidB / total * barW);
    int phoW = (int)((double)phoB / total * barW);
    int othW = (int)((double)other / total * barW);
    int freW = barW - vidW - phoW - othW;
    if (freW < 0) freW = 0;

    ui->videoSeg->setGeometry(24, 66, vidW, 14);
    ui->photoSeg->setGeometry(24 + vidW, 66, phoW, 14);
    ui->otherSeg->setGeometry(24 + vidW + phoW, 66, othW, 14);
    ui->freeSeg->setGeometry(24 + vidW + phoW + othW, 66, freW, 14);

    /* 动态圆角：左段左圆角 */
    if (vidW > 0)
        ui->videoSeg->setStyleSheet("background:#1a7bbd; border-top-left-radius:7px; border-bottom-left-radius:7px;");
    else
        ui->videoSeg->setStyleSheet("background:transparent;");

    /* 5. 图例 */
    int vidPct = (int)((double)vidB / total * 100);
    int phoPct = (int)((double)phoB / total * 100);
    int othPct = (int)((double)other / total * 100);

    ui->legendVideo->setText(QString::fromUtf8("录像 %1 (%2%)").arg(fmtSize(vidB)).arg(vidPct));
    ui->legendPhoto->setText(QString::fromUtf8("图片 %1 (%2%)").arg(fmtSize(phoB)).arg(phoPct));
    ui->legendOther->setText(QString::fromUtf8("其他 %1 (%2%)").arg(fmtSize(other)).arg(othPct));
    ui->legendFree->setText(QString::fromUtf8("空闲 %1 (%2%)").arg(fmtSize(free)).arg(freePct));

    /* 6. 预估天数 */
    double freeGB = free / (1024.0 * 1024.0 * 1024.0);
    int estDays = (int)(freeGB / 1.3);
    ui->estimateLabel->setText(
        QString::fromUtf8("预计还可存储约 %1 天").arg(estDays > 0 ? estDays : 0));
}

/* ========== 存储路径浏览 ========== */

void setup::on_browseBt_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this,
        QString::fromUtf8("选择存储目录"), m_storagePath,
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty()) { m_storagePath = dir; ui->pathInput->setText(dir); }
}

/* ========== 自动录像开关 ========== */

void setup::setRecordingState(bool recording) { ui->autoToggle->setChecked(recording); m_autoRecord = recording; }
void setup::on_autoToggle_clicked(bool checked) { m_autoRecord = checked; emit autoRecordToggled(checked); }

/* ========== 配置持久化 ========== */

void setup::loadStorageConfig()
{
    QFile file(CONFIG_FILE);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty() || line.startsWith('#')) continue;
        int eq = line.indexOf('='); if (eq < 0) continue;
        QString key = line.left(eq).trimmed(), val = line.mid(eq + 1).trimmed();
        if (key == "storagePath")    m_storagePath = val;
        if (key == "autoRecord")     m_autoRecord = (val == "true");
        if (key == "retainDays")     m_retainDays = val.toInt();
        if (key == "diskFullPolicy") m_diskFullPolicy = val.toInt();
    }
    file.close();
    ui->pathInput->setText(m_storagePath);
    ui->autoToggle->setChecked(m_autoRecord);
    int ri = 2;
    switch (m_retainDays) { case 7:ri=0;break; case 15:ri=1;break; case 30:ri=2;break; case 60:ri=3;break; case 90:ri=4;break; }
    ui->retainCombo->setCurrentIndex(ri);
    ui->policyCombo->setCurrentIndex(m_diskFullPolicy);
}

void setup::saveStorageConfig()
{
    QFile file(CONFIG_FILE);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) return;
    QTextStream out(&file);
    out << "storagePath=" << m_storagePath << "\n"
        << "autoRecord=" << (m_autoRecord ? "true" : "false") << "\n"
        << "retainDays=" << m_retainDays << "\n"
        << "diskFullPolicy=" << m_diskFullPolicy << "\n";
    file.close();
}

void setup::on_saveBtRec_clicked()
{
    m_storagePath = ui->pathInput->text();
    m_autoRecord = ui->autoToggle->isChecked();
    QStringList rv; rv << "7" << "15" << "30" << "60" << "90";
    m_retainDays = rv[ui->retainCombo->currentIndex()].toInt();
    m_diskFullPolicy = ui->policyCombo->currentIndex();
    saveStorageConfig();
}

void setup::on_resetBtRec_clicked()
{
    m_storagePath = "/opt/aicTrain/sMonitor/video"; m_autoRecord = false;
    m_retainDays = 30; m_diskFullPolicy = 0;
    ui->pathInput->setText(m_storagePath); ui->autoToggle->setChecked(false);
    ui->retainCombo->setCurrentIndex(2); ui->policyCombo->setCurrentIndex(0);
}
