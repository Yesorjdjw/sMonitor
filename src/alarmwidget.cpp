#include "alarmwidget.h"
#include "ui_alarmwidget.h"
#include "databasemanager.h"
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QColor>
#include <QDate>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QPixmap>
#include <QIcon>
#include <QMessageBox>
#include <QDebug>

AlarmWidget::AlarmWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::AlarmWidget), m_db(nullptr)
{
    ui->setupUi(this);

    ui->eventTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->eventTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->eventTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->eventTable->verticalHeader()->setVisible(false);
    ui->eventTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->eventTable->setIconSize(QSize(64, 48));
    ui->eventTable->verticalHeader()->setDefaultSectionSize(52);

    // Category selection with selected state
    auto selectCat = [this](const QString &type, QPushButton *btn) {
        QList<QPushButton*> btns = {ui->catBtn1, ui->catBtn2, ui->catBtn3, ui->catBtn4, ui->catBtn5};
        for (auto *b : btns) {
            b->setProperty("selected", false);
            b->style()->unpolish(b);
            b->style()->polish(b);
        }
        btn->setProperty("selected", true);
        btn->style()->unpolish(btn);
        btn->style()->polish(btn);
        m_currentType = type;
        m_currentPage = 1;
        loadEvents(type);
    };

    connect(ui->catBtn1, &QPushButton::clicked, [this, selectCat]{ selectCat("cover",   ui->catBtn1); });
    connect(ui->catBtn2, &QPushButton::clicked, [this, selectCat]{ selectCat("intrude", ui->catBtn2); });
    connect(ui->catBtn3, &QPushButton::clicked, [this, selectCat]{ selectCat("motion",  ui->catBtn3); });
    connect(ui->catBtn4, &QPushButton::clicked, [this, selectCat]{ selectCat("storage", ui->catBtn4); });
    connect(ui->catBtn5, &QPushButton::clicked, [this, selectCat]{ selectCat("face",    ui->catBtn5); });

    // Acknowledge
    connect(ui->ackBt, &QPushButton::clicked, [this]() {
        int row = ui->eventTable->currentRow();
        if (row < 0 || !m_db) return;
        QTableWidgetItem *it = ui->eventTable->item(row, 0);
        if (!it) return;
        int id = it->data(Qt::UserRole).toInt();
        m_db->acknowledgeEvent(id);
        refresh();
    });

    // 双击事件行 — 查看并自动标记已处理
    connect(ui->eventTable, &QTableWidget::cellDoubleClicked, [this](int row, int) {
        QTableWidgetItem *it = ui->eventTable->item(row, 0);
        if (!it || !m_db) return;
        int id = it->data(Qt::UserRole).toInt();
        m_db->acknowledgeEvent(id);
        refresh();
    });

    // Refresh
    connect(ui->refreshBt, &QPushButton::clicked, [this]{ refresh(); });

    // Search
    connect(ui->searchBt, &QPushButton::clicked, this, &AlarmWidget::onSearch);
    connect(ui->keywordEdit, &QLineEdit::returnPressed, this, &AlarmWidget::onSearch);
    connect(ui->timeCombo, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [this](int) { onSearch(); });
    connect(ui->levelCombo, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [this](int) { onSearch(); });

    // Export
    connect(ui->exportBt, &QPushButton::clicked, this, &AlarmWidget::onExport);

    // Pagination
    connect(ui->prevBt, &QPushButton::clicked, [this]{ onPageChanged(m_currentPage - 1); });
    connect(ui->nextBt, &QPushButton::clicked, [this]{ onPageChanged(m_currentPage + 1); });
    connect(ui->pageSizeCombo, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [this](int idx) {
                static const int sizes[] = {20, 50, 100};
                if (idx >= 0 && idx < 3) {
                    m_pageSize = sizes[idx];
                    m_currentPage = 1;
                    applyFilters();
                    renderPage();
                    updatePagination();
                }
            });

    // Clear all acknowledged
    connect(ui->clearBt, &QPushButton::clicked, this, &AlarmWidget::onClearAll);

    refresh();
}

AlarmWidget::~AlarmWidget() { delete ui; }

void AlarmWidget::setDatabase(DatabaseManager *db) { m_db = db; refresh(); }

static QString typeToName(const QString &type) {
    if (type == "cover")   return "画面遮挡";
    if (type == "intrude") return "区域入侵";
    if (type == "motion")  return "运动检测";
    if (type == "storage") return "存储告警";
    if (type == "face")    return "人脸告警";
    return type;
}

// ========== DATA ==========

void AlarmWidget::refresh() {
    updateCounts();
    loadEvents(m_currentType);
}

void AlarmWidget::updateCounts() {
    if (!m_db) return;
    QStringList types = {"cover", "intrude", "motion", "storage", "face"};
    QList<QLabel*> badges = {ui->badge1, ui->badge2, ui->badge3, ui->badge4, ui->badge5};
    int total = 0;
    for (int i = 0; i < types.size(); i++) {
        int c = m_db->getEventCount(types[i]);
        badges[i]->setText(QString::number(c));
        total += c;
    }
    ui->badgeTotal->setText(QString::number(total));
    ui->countText->setText(QString("共 %1 条记录").arg(total));
}

void AlarmWidget::loadEvents(const QString &type) {
    m_currentType = type;
    if (!m_db) return;
    m_allEvents = m_db->getEvents(type.isEmpty() ? "" : type, 2000);

    // Reset filters when switching category
    ui->timeCombo->setCurrentIndex(0);
    ui->levelCombo->setCurrentIndex(0);
    ui->keywordEdit->clear();
    m_timeFilter = 0;
    m_levelFilter.clear();
    m_keywordFilter.clear();
    m_currentPage = 1;

    applyFilters();
    renderPage();
    updatePagination();
}

// ========== FILTER ==========

void AlarmWidget::onSearch() {
    m_timeFilter = ui->timeCombo->currentIndex();
    m_levelFilter = (ui->levelCombo->currentIndex() == 0) ? QString() : ui->levelCombo->currentText();
    m_keywordFilter = ui->keywordEdit->text().trimmed();
    m_currentPage = 1;
    applyFilters();
    renderPage();
    updatePagination();
}

void AlarmWidget::applyFilters() {
    m_filteredEvents.clear();
    for (const EventRecord &e : m_allEvents) {
        // Time filter
        if (m_timeFilter > 0) {
            QDate evtDate = QDate::fromString(e.createdAt.left(10), "yyyy-MM-dd");
            if (!evtDate.isValid()) continue;
            QDate today = QDate::currentDate();
            if (m_timeFilter == 1 && evtDate != today) continue;
            if (m_timeFilter == 2 && evtDate < today.addDays(-7)) continue;
            if (m_timeFilter == 3 && evtDate < today.addDays(-30)) continue;
        }
        // Level filter
        if (!m_levelFilter.isEmpty() && e.level != m_levelFilter) continue;
        // Keyword
        if (!m_keywordFilter.isEmpty()) {
            QString haystack = typeToName(e.type) + " " + e.description + " " + e.level + " " + e.createdAt;
            if (!haystack.contains(m_keywordFilter, Qt::CaseInsensitive))
                continue;
        }
        m_filteredEvents.append(e);
    }
}

// ========== RENDER ==========

void AlarmWidget::renderPage() {
    bool empty = m_filteredEvents.isEmpty();
    ui->contentStack->setCurrentIndex(empty ? 1 : 0);
    ui->pageBar->setVisible(!empty);

    if (empty) {
        ui->eventTable->setRowCount(0);
        return;
    }

    int total = m_filteredEvents.size();
    int start = (m_currentPage - 1) * m_pageSize;
    int end = qMin(start + m_pageSize, total);

    ui->eventTable->setRowCount(end - start);
    for (int i = start; i < end; i++) {
        const EventRecord &e = m_filteredEvents[i];
        int row = i - start;

        // Col 0: type
        QTableWidgetItem *tType = new QTableWidgetItem(typeToName(e.type));
        tType->setData(Qt::UserRole, e.id);
        ui->eventTable->setItem(row, 0, tType);

        // Col 1: level
        QTableWidgetItem *tLevel = new QTableWidgetItem(e.level);
        bool critical = (e.level == "紧急");
        bool warning  = (e.level == "重要");
        QColor rowBg = critical ? QColor(254, 240, 240) : (warning ? QColor(255, 248, 240) : Qt::white);
        tLevel->setForeground(critical ? QColor("#D93025") : (warning ? QColor("#F29927") : QColor("#666666")));
        ui->eventTable->setItem(row, 1, tLevel);

        // Col 2: description
        ui->eventTable->setItem(row, 2, new QTableWidgetItem(e.description));

        // Col 3: time
        ui->eventTable->setItem(row, 3, new QTableWidgetItem(e.createdAt));

        // Col 4: status
        QTableWidgetItem *tStat = new QTableWidgetItem(e.acknowledged ? "已处理" : "待处理");
        tStat->setForeground(e.acknowledged ? QColor("#27ae60") : QColor("#F29927"));
        ui->eventTable->setItem(row, 4, tStat);

        // Col 5: thumbnail
        QTableWidgetItem *tThumb = new QTableWidgetItem();
        if (!e.thumbnail.isEmpty()) {
            QString thumbPath = e.thumbnail;
            if (!thumbPath.startsWith('/')) {
                // 先查告警目录，再查照片目录
                QString alarmPath = "/usr/local/nginx/html/media/alarms/" + thumbPath;
                QString photoPath = "/usr/local/nginx/html/media/photos/" + thumbPath;
                if (QFile::exists(alarmPath))
                    thumbPath = alarmPath;
                else
                    thumbPath = photoPath;
            }
            if (QFile::exists(thumbPath)) {
                QPixmap pm(thumbPath);
                if (!pm.isNull())
                    tThumb->setIcon(QIcon(pm.scaled(64, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
            }
        }
        ui->eventTable->setItem(row, 5, tThumb);

        // Row background for critical/warning
        if (critical || warning) {
            for (int c = 0; c < 6; c++)
                ui->eventTable->item(row, c)->setBackground(rowBg);
        }
    }
}

// ========== PAGINATION ==========

void AlarmWidget::onPageChanged(int page) {
    int totalPages = qMax(1, (m_filteredEvents.size() + m_pageSize - 1) / m_pageSize);
    if (page < 1 || page > totalPages) return;
    m_currentPage = page;
    renderPage();
    updatePagination();
}

void AlarmWidget::updatePagination() {
    int totalPages = qMax(1, (m_filteredEvents.size() + m_pageSize - 1) / m_pageSize);
    ui->pageLabel->setText(QString("第 %1/%2 页").arg(m_currentPage).arg(totalPages));
    ui->prevBt->setEnabled(m_currentPage > 1);
    ui->nextBt->setEnabled(m_currentPage < totalPages);
}

// ========== EXPORT ==========

void AlarmWidget::onExport() {
    if (m_filteredEvents.isEmpty()) {
        QMessageBox::information(this, "导出", "没有可导出的告警记录。");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "导出告警记录",
        QString("alarm_%1.csv").arg(QDate::currentDate().toString("yyyyMMdd")),
        "CSV 文件 (*.csv)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "导出失败", "无法写入文件: " + fileName);
        return;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << "\xEF\xBB\xBF";  // BOM for Excel
    out << "\"事件类型\",\"等级\",\"描述\",\"触发时间\",\"状态\"\n";

    for (const EventRecord &e : m_filteredEvents) {
        auto csv = [](const QString &s) { return "\"" + QString(s).replace("\"", "\"\"") + "\""; };
        out << csv(typeToName(e.type)) << ","
            << csv(e.level) << ","
            << csv(e.description) << ","
            << csv(e.createdAt) << ","
            << csv(e.acknowledged ? "已处理" : "待处理") << "\n";
    }
    file.close();
    QMessageBox::information(this, "导出成功",
        QString("已导出 %1 条告警记录到:\n%2").arg(m_filteredEvents.size()).arg(fileName));
}

// ========== CLEAR ==========

void AlarmWidget::onClearAll() {
    if (!m_db) return;
    int acked = 0;
    for (const EventRecord &e : m_allEvents) {
        if (e.acknowledged) acked++;
    }
    if (acked == 0) {
        QMessageBox::information(this, "清除告警", "没有已处理的告警可清除。");
        return;
    }
    QMessageBox::StandardButton reply = QMessageBox::question(this, "清除告警",
        QString("确定清除所有已处理的告警记录吗？\n\n共 %1 条已处理记录将被删除。").arg(acked),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        int deleted = m_db->deleteAcknowledgedEvents();
        QMessageBox::information(this, "清除完成",
            QString("已删除 %1 条已处理的告警记录。").arg(deleted));
        refresh();
    }
}
