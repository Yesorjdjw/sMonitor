#ifndef FOPS_H
#define FOPS_H

#include <QWidget>
#include <QListWidgetItem>
#include <QTimer>
#include <QIcon>
#include <QFutureWatcher>
#include <opencv2/opencv.hpp>

namespace Ui {
class fops;
}

struct ThumbnailData {
    QString path;
    QImage image; // Pass QImage across thread boundary instead of QIcon/QPixmap
    int type; // 1: Photo, 2: Video
    bool hasImage; // Indicates if thumbnail was successfully loaded
};

class fops : public QWidget
{
    Q_OBJECT

public:
    explicit fops(QWidget *parent = 0);
    ~fops();

private slots:
    void on_pushButton_clicked();
    void onFileClicked(QListWidgetItem *item);
    void onPlayVideo();
    void onDeleteFile();
    void onRefresh();
    void onFilterAll();
    void onFilterPhoto();
    void onFilterVideo();
    void onVideoTick();

    // New slots
    void onSearchTextChanged(const QString &text);
    void onRenameFile();
    void onThumbnailsLoaded();

private:
    Ui::fops *ui;

    QStringList m_photoPaths;
    QStringList m_videoPaths;
    int         m_filter;
    QString     m_curPath;

    QTimer           *m_vidTimer;
    cv::VideoCapture *m_vidCap;
    cv::Mat           m_vidFrame;
    bool              m_playing;

    QString           m_searchText;
    QFutureWatcher<QList<ThumbnailData> > m_thumbWatcher;

    void scanFiles();
    void showPreview(const QString &path);
    void stopVideo();
    void resetPreview();
};

#endif // FOPS_H
