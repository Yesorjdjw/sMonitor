#ifndef MEDIAPREVIEW_H
#define MEDIAPREVIEW_H

#include <QDialog>
#include <QList>
#include "databasemanager.h"

namespace Ui { class MediaPreview; }
class QProcess;

class MediaPreview : public QDialog
{
    Q_OBJECT
public:
    explicit MediaPreview(QWidget *parent = nullptr);
    ~MediaPreview();

    void showFiles(const QList<MediaFile> &files, int startIndex = 0);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void showCurrent();
    void prevFile();
    void nextFile();
    void openSystem();

private:
    void updateDisplay();
    void playVideo(const QString &path);   // mplayer 嵌入播放录像
    void stopVideo();                      // 停止/清理 mplayer

    Ui::MediaPreview *ui;
    QList<MediaFile> m_files;
    int m_index = 0;
    QPixmap m_currentPixmap;
    QProcess *m_player = nullptr;
};

#endif // MEDIAPREVIEW_H
