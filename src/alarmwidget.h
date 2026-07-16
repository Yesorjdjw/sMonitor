#ifndef ALARMWIDGET_H
#define ALARMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QList>
#include "databasemanager.h"

class DatabaseManager;
namespace Ui { class AlarmWidget; }

class AlarmWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AlarmWidget(QWidget *parent = nullptr);
    ~AlarmWidget();
    void setDatabase(DatabaseManager *db);
    void refresh();

private slots:
    void onSearch();
    void onExport();
    void onPageChanged(int page);
    void onClearAll();

private:
    void updateCounts();
    void loadEvents(const QString &type);
    void applyFilters();
    void renderPage();
    void updatePagination();

    Ui::AlarmWidget *ui;
    DatabaseManager *m_db;
    QString m_currentType;

    // Full event list loaded from DB (before filtering)
    QList<EventRecord> m_allEvents;
    // Filtered list (after keyword/level/time)
    QList<EventRecord> m_filteredEvents;

    // Filter state
    QString m_keywordFilter;
    QString m_levelFilter;
    int m_timeFilter = 0;   // 0=全部, 1=今天, 2=近7天, 3=近30天

    // Pagination
    int m_currentPage = 1;
    int m_pageSize = 20;
};

#endif
