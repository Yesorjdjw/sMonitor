#ifndef FULLSHOW_H
#define FULLSHOW_H

#include <QMainWindow>
#include <QDebug>
#include <QProcess>
#include <QFile>
#include <QMouseEvent>
#include <QKeyEvent>

namespace Ui {
class fullShow;
}

class fullShow : public QMainWindow
{
    Q_OBJECT

public:
    explicit fullShow(QWidget *parent = 0);
    ~fullShow();

    bool play_video(const QString &vid_name);

public slots:
    void show_picture(const QPixmap &pic_name);
    void show_video(const QString &vid_name);

private slots:
    void on_label_doubleclicked();
    void onProcessFinished();

protected:
    void keyPressEvent(QKeyEvent *event);
    void showEvent(QShowEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

private:
    Ui::fullShow *ui;
    QProcess *m_process;
    QString m_pendingVideo;
    bool m_isClosing;
    void closeShow();
};

#endif // FULLSHOW_H
