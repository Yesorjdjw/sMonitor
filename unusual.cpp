#include "unusual.h"
#include "ui_unusual.h"
#include "fullShow.h"
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QLabel>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QDebug>
#include <cstdlib>
#include <QCoreApplication>

unusual::unusual(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::unusual),
    m_currentIndex(0)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setGeometry(0, 0, 1024, 600);
    initTable();
    loadDefaultData();
    updateCategoryUI();
    if (!m_categories.isEmpty()) {
        selectCategory(0);
    }
}

unusual::~unusual()
{
    delete ui;
}

void unusual::on_backBt_clicked()
{
    this->close();
}

void unusual::updateCategoryUI()
{
    // Update left sidebar badges and button text
    QPushButton *btns[] = {
        findChild<QPushButton*>("catBtn1"),
        findChild<QPushButton*>("catBtn2"),
        findChild<QPushButton*>("catBtn3"),
        findChild<QPushButton*>("catBtn4"),
        findChild<QPushButton*>("catBtn5")
    };
    QLabel *badges[] = {ui->badge1, ui->badge2, ui->badge3, ui->badge4, ui->badge5};

    int total = 0;
    int maxItems = qMin(m_categories.size(), 5);

    for (int i = 0; i < 5; i++) {
        if (i < maxItems) {
            badges[i]->setText(QString::number(m_categories[i].count));
            badges[i]->show();
            if (btns[i]) btns[i]->setText(m_categories[i].name);
            total += m_categories[i].count;
        } else {
            badges[i]->hide();
            if (btns[i]) btns[i]->setText("");
        }
    }
    ui->badgeTotal->setText(QString::number(total));
}

void unusual::setData(const QList<CategoryData> &categories)
{
    m_categories = categories;
    updateCategoryUI();
    selectCategory(0);
}

void unusual::initTable()
{
    ui->eventTable->setColumnWidth(0, 130);
    ui->eventTable->setColumnWidth(1, 160);
    ui->eventTable->setColumnWidth(2, 70);
    ui->eventTable->setColumnWidth(3, 180);
    ui->eventTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
}

void unusual::loadDefaultData()
{
    m_categories.clear();

    QString eventsJsonPath = QCoreApplication::applicationDirPath() + "/unusual/unusual_events.json";
    QFile file(eventsJsonPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "No unusual events file yet";
        return;
    }

    QJsonArray events = QJsonDocument::fromJson(file.readAll()).array();
    file.close();
    if (events.isEmpty()) return;

    // Group events by type
    QMap<QString, QList<EventData>> grouped;
    for (const QJsonValue &val : events) {
        QJsonObject obj = val.toObject();
        EventData e;
        e.screenshot = obj["screenshot"].toString();
        e.type = obj["type"].toString();
        e.level = obj["level"].toString();
        e.time = obj["time"].toString();
        e.status = obj["status"].toString();
        e.videoPath = obj["videoPath"].toString();
        grouped[e.type].append(e);
    }

    // Build categories in fixed order
    QStringList typeOrder;
    typeOrder << QString::fromUtf8("人脸识别异常")
              << QString::fromUtf8("画面遮挡")
              << QString::fromUtf8("其他异常");

    QMap<QString, QString> iconMap;
    iconMap[QString::fromUtf8("人脸识别异常")] = ":/images/icon_alert.svg";
    iconMap[QString::fromUtf8("画面遮挡")] = ":/images/icon_warning.svg";
    iconMap[QString::fromUtf8("其他异常")] = ":/images/icon_info.svg";

    for (const QString &typeName : typeOrder) {
        if (!grouped.contains(typeName)) continue;
        CategoryData cat;
        cat.name = typeName;
        cat.count = grouped[typeName].size();
        cat.iconPath = iconMap.value(typeName, ":/images/icon_info.svg");
        cat.events = grouped[typeName];
        m_categories.append(cat);
    }
}

void unusual::selectCategory(int index)
{
    if (m_categories.isEmpty() || index < 0 || index >= m_categories.size()) return;
    m_currentIndex = index;
    const CategoryData &cat = m_categories[index];

    clearSelection();
    const QString selStyle =
        "background: rgba(0,180,255,0.2); border: none; border-radius: 0px;"
        "text-align: left; font-size: 13px; font-weight: 600;"
        "color: #333333; padding-left: 52px;";
    QPushButton *btn = findChild<QPushButton*>(QString("catBtn%1").arg(index + 1));
    if (btn) {
        btn->setStyleSheet(selStyle);
    }

    ui->rightTitleText->setText(cat.name);
    ui->countText->setText(QString::fromUtf8("共 %1 条").arg(cat.count));
    if (!cat.iconPath.isEmpty() && QFile::exists(cat.iconPath)) {
        ui->rightIcon->setPixmap(QPixmap(cat.iconPath));
    }

    // Setup table - also connect double-click for video playback
    ui->eventTable->clearContents();
    ui->eventTable->setRowCount(cat.events.size());

    for (int i = 0; i < cat.events.size(); i++) {
        ui->eventTable->setRowHeight(i, 60);
        const EventData &e = cat.events[i];

        // Screenshot column
        QLabel *thumbLabel = new QLabel();
        thumbLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
        if (!e.screenshot.isEmpty() && QFile::exists(e.screenshot)) {
            QPixmap thumb(e.screenshot);
            thumbLabel->setPixmap(thumb.scaled(100, 56, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            thumbLabel->setText(QString::fromUtf8("无截图"));
            thumbLabel->setStyleSheet("color: #999; font-size: 11px;");
        }
        thumbLabel->setAlignment(Qt::AlignCenter);
        thumbLabel->setStyleSheet(thumbLabel->styleSheet() + "background: #000; border-radius: 4px;");
        ui->eventTable->setCellWidget(i, 0, thumbLabel);

        // Type column - store videoPath in UserRole
        QTableWidgetItem *typeItem = new QTableWidgetItem(e.type);
        typeItem->setFlags(typeItem->flags() & ~Qt::ItemIsEditable);
        if (!e.videoPath.isEmpty()) {
            typeItem->setData(Qt::UserRole, e.videoPath);
            typeItem->setData(Qt::UserRole + 1, e.screenshot);  // also store screenshot path
        }
        ui->eventTable->setItem(i, 1, typeItem);

        // Level
        QTableWidgetItem *levelItem = new QTableWidgetItem(e.level);
        levelItem->setFlags(levelItem->flags() & ~Qt::ItemIsEditable);
        if (e.level == QString::fromUtf8("紧急"))
            levelItem->setForeground(QColor("#ff3b30"));
        else if (e.level == QString::fromUtf8("警告"))
            levelItem->setForeground(QColor("#ff9500"));
        else if (e.level == QString::fromUtf8("通知"))
            levelItem->setForeground(QColor("#007aff"));
        ui->eventTable->setItem(i, 2, levelItem);

        // Time
        QTableWidgetItem *timeItem = new QTableWidgetItem(e.time);
        timeItem->setFlags(timeItem->flags() & ~Qt::ItemIsEditable);
        ui->eventTable->setItem(i, 3, timeItem);

        // Status
        QTableWidgetItem *statusItem = new QTableWidgetItem(e.status);
        statusItem->setFlags(statusItem->flags() & ~Qt::ItemIsEditable);
        if (e.status == QString::fromUtf8("待处理"))
            statusItem->setForeground(QColor("#ff9500"));
        else if (e.status == QString::fromUtf8("处理中"))
            statusItem->setForeground(QColor("#1a7bbd"));
        ui->eventTable->setItem(i, 4, statusItem);
    }

    // Connect double-click for video playback
    QObject::disconnect(ui->eventTable, nullptr, nullptr, nullptr);
    QObject::connect(ui->eventTable, &QTableWidget::cellDoubleClicked, [this](int row, int) {
        QTableWidgetItem *item = ui->eventTable->item(row, 1);
        if (!item) return;
        QString videoPath = item->data(Qt::UserRole).toString();
        if (!videoPath.isEmpty() && QFile::exists(videoPath)) {
            qDebug() << "Playing:" << videoPath;
            fullShow *fs = new fullShow(this);
            fs->setAttribute(Qt::WA_DeleteOnClose);
            fs->show_video(videoPath);
            fs->show();
        }
    });
}

void unusual::clearSelection()
{
    const QString normalStyle =
        "background: transparent; border: none; border-radius: 0px;"
        "text-align: left; font-size: 13px; font-weight: 600;"
        "color: #333333; padding-left: 52px;";
    for (int i = 1; i <= 5; i++) {
        QPushButton *btn = findChild<QPushButton*>(QString("catBtn%1").arg(i));
        if (btn) btn->setStyleSheet(normalStyle);
    }
}

void unusual::on_catBtn1_clicked() { selectCategory(0); }
void unusual::on_catBtn2_clicked() { selectCategory(1); }
void unusual::on_catBtn3_clicked() { selectCategory(2); }
void unusual::on_catBtn4_clicked() { selectCategory(3); }
void unusual::on_catBtn5_clicked() { selectCategory(4); }
