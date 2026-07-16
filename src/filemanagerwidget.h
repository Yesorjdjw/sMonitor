#ifndef FILEMANAGERWIDGET_H
#define FILEMANAGERWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QList>
#include "databasemanager.h"

namespace Ui { class FileManagerWidget; }

class ThumbnailCard;
class QNetworkAccessManager;

class FileManagerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FileManagerWidget(QWidget *parent = nullptr);
    ~FileManagerWidget();

    void setDatabase(DatabaseManager *db);
    void refreshFiles();
    void showCategory(const QString &type);   // 切到指定分类并刷新(拍照/录像后定位新文件)

private slots:
    void switchCategory(const QString &type);
    void loadFiles(int page = 1);
    void onCardClicked(int fileId);
    void onCardDoubleClicked(int fileId);
    void onContextMenu(int fileId, const QPoint &globalPos);

private:
    void setupConnections();
    void connectCard(ThumbnailCard *card);
    void clearCards();

    Ui::FileManagerWidget *ui;
    QWidget *m_gridContainer;
    QGridLayout *m_gridLayout;
    QList<ThumbnailCard*> m_cards;
    DatabaseManager *m_db = nullptr;
    QNetworkAccessManager *m_net = nullptr;   // 删除调用中心 API
    QString m_currentType = "recording";
    int m_currentPage = 1;
    int m_totalPages = 1;
    int m_pageSize = 9;

    // For search
    QString m_cachedFilter;
    QList<MediaFile> m_cachedFiles;
};

#endif // FILEMANAGERWIDGET_H
