#include "unusual.h"
#include "ui_unusual.h"
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QLabel>

unusual::unusual(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::unusual),
    m_currentIndex(0)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setGeometry(0, 0, 1024, 600);

    // Replace DOM traversal with QButtonGroup
    m_catGroup = new QButtonGroup(this);
    m_catGroup->addButton(ui->catBtn1, 0);
    m_catGroup->addButton(ui->catBtn2, 1);
    m_catGroup->addButton(ui->catBtn3, 2);
    m_catGroup->addButton(ui->catBtn4, 3);
    m_catGroup->addButton(ui->catBtn5, 4);

    initTable();
    loadDefaultData();
    selectCategory(0);
}

unusual::~unusual()
{
    delete ui;
}

void unusual::on_backBt_clicked()
{
    this->close();
}

void unusual::setData(const QList<CategoryData> &categories)
{
    m_categories = categories;
    QLabel *badges[] = {ui->badge1, ui->badge2, ui->badge3, ui->badge4, ui->badge5};
    int total = 0;
    int maxItems = qMin(m_categories.size(), 5);
    for (int i = 0; i < maxItems; i++) {
        badges[i]->setText(QString::number(m_categories[i].count));
        badges[i]->show();
        total += m_categories[i].count;
    }
    for (int i = maxItems; i < 5; i++) {
        badges[i]->hide();
    }
    ui->badgeTotal->setText(QString::number(total));
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
}

void unusual::selectCategory(int index)
{
    if (m_categories.isEmpty() || index < 0 || index >= m_categories.size()) return;
    m_currentIndex = index;
    const CategoryData &cat = m_categories[index];

    clearSelection();

    QAbstractButton *btn = m_catGroup->button(index);
    if (btn) {
        btn->setProperty("catSelected", "true");
        btn->style()->unpolish(btn);
        btn->style()->polish(btn);
    }

    // Manual icon handling (hardcoded to avoid findChild completely)
    QLabel* icons[] = {ui->catIcon1, ui->catIcon2, ui->catIcon3, ui->catIcon4, ui->catIcon5};
    if (index >= 0 && index < 5) {
        icons[index]->raise();
    }

    ui->rightTitleText->setText(cat.name);
    ui->countText->setText(QString("共 %1 条").arg(cat.count));
    ui->rightIcon->setPixmap(QPixmap(cat.iconPath));
    setupTable(cat.events);
}

void unusual::setupTable(const QList<EventData> &events)
{
    ui->eventTable->clearContents();
    ui->eventTable->setRowCount(events.size());

    for (int i = 0; i < events.size(); i++) {
        ui->eventTable->setRowHeight(i, 54);
        const EventData &e = events[i];
        QStringList cols;
        cols << e.screenshot << e.type << e.level << e.time << e.status;
        for (int j = 0; j < 5; j++) {
            QTableWidgetItem *item = new QTableWidgetItem(cols[j]);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            ui->eventTable->setItem(i, j, item);
        }

        // 等级列颜色
        QTableWidgetItem *levelItem = ui->eventTable->item(i, 2);
        if (levelItem) {
            if (e.level == QString::fromUtf8("紧急"))
                levelItem->setForeground(QColor("#ff3b30"));
            else if (e.level == QString::fromUtf8("警告"))
                levelItem->setForeground(QColor("#ff9500"));
            else if (e.level == QString::fromUtf8("通知"))
                levelItem->setForeground(QColor("#007aff"));
        }

        // 状态列颜色
        QTableWidgetItem *statusItem = ui->eventTable->item(i, 4);
        if (statusItem) {
            if (e.status == QString::fromUtf8("待处理"))
                statusItem->setForeground(QColor("#ff9500"));
            else if (e.status == QString::fromUtf8("处理中"))
                statusItem->setForeground(QColor("#1a7bbd"));
        }
    }
}

void unusual::clearSelection()
{
    for (QAbstractButton *btn : m_catGroup->buttons()) {
        btn->setProperty("catSelected", "false");
        btn->style()->unpolish(btn);
        btn->style()->polish(btn);
    }
}

void unusual::on_catBtn1_clicked() { selectCategory(0); }
void unusual::on_catBtn2_clicked() { selectCategory(1); }
void unusual::on_catBtn3_clicked() { selectCategory(2); }
void unusual::on_catBtn4_clicked() { selectCategory(3); }
void unusual::on_catBtn5_clicked() { selectCategory(4); }
