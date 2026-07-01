#ifndef FOPS_H
#define FOPS_H

#include <QWidget>
#include <QListWidgetItem>
#include <QTimer>
#include <QPushButton>
#include <QInputDialog>
#include <opencv2/opencv.hpp>

namespace Ui {
class fops;
}

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
    void onRenameFile();
    void onRefresh();
    void onFilterAll();
    void onFilterPhoto();
    void onFilterVideo();
    void onVideoTick();

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

    QPushButton *m_renameBtn;

    void scanFiles();
    void showPreview(const QString &path);
    void stopVideo();
    void resetPreview();
};

#endif // FOPS_H
