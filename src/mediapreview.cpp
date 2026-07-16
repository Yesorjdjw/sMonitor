#include "mediapreview.h"
#include "ui_mediapreview.h"

#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QFileInfo>
#include <QKeyEvent>
#include <QProcess>
#include <QDebug>

MediaPreview::MediaPreview(QWidget *parent)
    : QDialog(parent), ui(new Ui::MediaPreview)
{
    ui->setupUi(this);
    ui->rootLayout->setStretch(1, 1); // preview label fills remaining space
    setWindowFlags(Qt::Dialog | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);

    connect(ui->closeBtn, &QPushButton::clicked, this, &QDialog::close);
    connect(ui->prevBtn, &QPushButton::clicked, this, &MediaPreview::prevFile);
    connect(ui->nextBtn, &QPushButton::clicked, this, &MediaPreview::nextFile);
    connect(ui->openSystemBtn, &QPushButton::clicked, this, &MediaPreview::openSystem);
}

MediaPreview::~MediaPreview()
{
    stopVideo();
    delete ui;
}

void MediaPreview::showFiles(const QList<MediaFile> &files, int startIndex)
{
    m_files = files;
    m_index = qBound(0, startIndex, files.size() - 1);

    if (files.isEmpty()) return;

    showCurrent();
    exec();
}

void MediaPreview::showCurrent()
{
    if (m_index < 0 || m_index >= m_files.size()) return;

    stopVideo(); // 切换/重播前先停掉上一个 mplayer

    const MediaFile &mf = m_files.at(m_index);
    QString path = mf.filepath;

    // Update info label
    QString typeLabel = (mf.type == "recording") ? "录像" : "照片";
    QString sizeLabel;
    if (mf.size > 1024 * 1024)
        sizeLabel = QString("%1 MB").arg(mf.size / (1024.0 * 1024.0), 0, 'f', 1);
    else
        sizeLabel = QString("%1 KB").arg(mf.size / 1024.0, 0, 'f', 1);

    setWindowTitle(QString("媒体预览 - %1").arg(mf.filename));
    ui->fileInfoLabel->setText(
        QString("[%1/%2]  %3  |  %4  |  %5")
        .arg(m_index + 1).arg(m_files.size())
        .arg(mf.filename).arg(typeLabel).arg(sizeLabel));

    // Update nav buttons
    ui->prevBtn->setEnabled(m_index > 0);
    ui->nextBtn->setEnabled(m_index < m_files.size() - 1);

    // 录像(视频)→ mplayer 嵌入播放;照片 → QPixmap 显示
    bool isVideo = (mf.type == "recording") || path.endsWith(".avi", Qt::CaseInsensitive);
    if (isVideo) {
        m_currentPixmap = QPixmap();
        if (!QFile::exists(path)) {
            ui->previewLabel->setText("文件不存在:\n" + path);
            ui->previewLabel->setStyleSheet("color: #8a8a9a; font-size: 16px; background-color: #1a1a1e;");
            return;
        }
        playVideo(path);
        return;
    }

    // Load and display (image)
    m_currentPixmap = QPixmap(path);
    if (m_currentPixmap.isNull()) {
        ui->previewLabel->setText("无法加载文件:\n" + path);
        ui->previewLabel->setStyleSheet("color: #8a8a9a; font-size: 16px; background-color: #1a1a1e;");
        return;
    }

    updateDisplay();
}

void MediaPreview::playVideo(const QString &path)
{
    // 清掉图片/文字,把 previewLabel 变成 mplayer 的嵌入窗口
    ui->previewLabel->setText("");
    ui->previewLabel->setPixmap(QPixmap());
    ui->previewLabel->setStyleSheet("background-color: #000000;");
    ui->previewLabel->winId(); // 强制创建 native 窗口(mplayer -wid 需要)

    m_player = new QProcess(this);
    QStringList args;
    args << "-quiet" << "-zoom"
         << "-vo" << "x11"
         << "-wid" << QString::number((unsigned long)ui->previewLabel->winId())
         << path;
    m_player->start("mplayer", args);
}

void MediaPreview::stopVideo()
{
    if (m_player) {
        if (m_player->state() != QProcess::NotRunning) {
            m_player->kill();
            m_player->waitForFinished(500);
        }
        delete m_player;
        m_player = nullptr;
    }
}

void MediaPreview::updateDisplay()
{
    if (m_currentPixmap.isNull()) return;

    QSize labelSize = ui->previewLabel->size();
    if (labelSize.width() < 10 || labelSize.height() < 10) return;

    QPixmap scaled = m_currentPixmap.scaled(labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->previewLabel->setPixmap(scaled);
}

void MediaPreview::prevFile()
{
    if (m_index > 0) {
        m_index--;
        showCurrent();
    }
}

void MediaPreview::nextFile()
{
    if (m_index < m_files.size() - 1) {
        m_index++;
        showCurrent();
    }
}

void MediaPreview::openSystem()
{
    if (m_index >= 0 && m_index < m_files.size()) {
        QString path = m_files.at(m_index).filepath;
        if (QFile::exists(path))
            QDesktopServices::openUrl(QUrl::fromLocalFile(path));
    }
}

void MediaPreview::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Left:  case Qt::Key_A: prevFile(); return;
    case Qt::Key_Right: case Qt::Key_D: nextFile(); return;
    case Qt::Key_Escape: close(); return;
    case Qt::Key_F11:
        isFullScreen() ? showNormal() : showFullScreen();
        return;
    }
    QDialog::keyPressEvent(event);
}

void MediaPreview::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
    if (!m_currentPixmap.isNull())
        updateDisplay();
}
