#ifndef FILEINDEX_H
#define FILEINDEX_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QInputDialog>
#include <QMessageBox>
#include <QProcess>
#include <QThread>
#include <QMutex>
#include <QTimer>
#include <QLineEdit>
#include <atomic>
#include <opencv2/opencv.hpp>
#include "fullShow.h"

class VirtualKeyboard;

namespace Ui { class fileIndex; }

/* 后台缩略图生成器 */
class ThumbnailWorker : public QObject
{
    Q_OBJECT
public:
    struct Job { int idx; QString path; QString cache; };
    void setJobs(const QList<Job> &jobs);

public slots:
    void processJobs();

signals:
    void thumbReady(int index, const QPixmap &thumb);

private:
    QList<Job> m_jobs;
    QMutex m_mutex;
    static QPixmap grabFrame(const QString &videoPath);
};

class fileIndex : public QMainWindow
{
    Q_OBJECT

signals:
    void exitRequested();

public:
    explicit fileIndex(QWidget *parent = 0);
    ~fileIndex();
    void updatePicData(const QString &p_name);
    void updataVidData(const QString &vp_name, const QString &v_name);

private slots:
    void on_exit_bt_clicked();
    void on_refreshBt_clicked();
    void on_renameBt_clicked();
    void on_deleteBt_clicked();
    void on_searchEdit_textChanged(const QString &text);
    void onItemClicked(QListWidgetItem *item);
    void on_picWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_vidWidget_itemDoubleClicked(QListWidgetItem *item);
    void onThumbReady(int index, const QPixmap &thumb);
    void onKeyboardDone();
    bool eventFilter(QObject *obj, QEvent *event);

private:
    Ui::fileIndex *ui;
    QString m_picPath, m_vidPath;
    QStringList picList, vidList;
    fullShow *m_fullScreen;
    std::atomic<bool> m_busy;
    QString m_searchFilter;

    QThread *m_thumbThread;
    ThumbnailWorker *m_thumbWorker;

    /* 虚拟键盘（触摸屏） */
    VirtualKeyboard *m_keyboard;
    QLineEdit *m_renameEdit;
    QString m_renamePath, m_renameOldName;

    void initWidget(QListWidget *w);
    void refreshList();
    void applyFilter();
    QString selectedFilePath();
};

#endif // FILEINDEX_H
