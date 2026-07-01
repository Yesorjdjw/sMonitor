#include "fops.h"
#include "ui_fops.h"
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QFile>

#define PHOTO_DIR  "/opt/aicTrain/sMonitor/photo"
#define VIDEO_DIR  "/opt/aicTrain/sMonitor/video"

fops::fops(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fops),
    m_filter(0),
    m_vidTimer(0),
    m_vidCap(0),
    m_playing(false)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setGeometry(0, 0, 1024, 600);

    connect(ui->tabAll, SIGNAL(clicked()), this, SLOT(onFilterAll()));
    connect(ui->tabPhoto, SIGNAL(clicked()), this, SLOT(onFilterPhoto()));
    connect(ui->tabVideo, SIGNAL(clicked()), this, SLOT(onFilterVideo()));
    connect(ui->fileList, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(onFileClicked(QListWidgetItem*)));
    connect(ui->playBtn, SIGNAL(clicked()), this, SLOT(onPlayVideo()));
    connect(ui->delBtn, SIGNAL(clicked()), this, SLOT(onDeleteFile()));
    connect(ui->refreshBtn, SIGNAL(clicked()), this, SLOT(onRefresh()));

    /* 底部三个按钮对齐文件列表宽度 (24~284 = 260px) */
    ui->delBtn->setGeometry(24, 504, 84, 36);
    ui->refreshBtn->setGeometry(200, 504, 84, 36);

    /* 改名按钮 */
    m_renameBtn = new QPushButton(QString::fromUtf8("改名"), this);
    m_renameBtn->setObjectName(QStringLiteral("renameBtn"));
    m_renameBtn->setGeometry(112, 504, 84, 36);
    m_renameBtn->setStyleSheet(
        "QPushButton {"
        "  background: #f0ad4e;"
        "  color: #fff;"
        "  border: none;"
        "  border-radius: 4px;"
        "  font-size: 12px;"
        "}"
        "QPushButton:disabled {"
        "  background: #bbb;"
        "}");
    m_renameBtn->setEnabled(false);
    connect(m_renameBtn, SIGNAL(clicked()), this, SLOT(onRenameFile()));

    ui->hintLabel->hide();
    resetPreview();
    scanFiles();
}

fops::~fops()
{
    stopVideo();
    delete ui;
}

void fops::resetPreview()
{
    ui->preview->setText(QString::fromUtf8("← 选择文件预览"));
    ui->playBtn->setEnabled(false);
    m_renameBtn->setEnabled(false);
}

/* ========== 文件扫描 ========== */

void fops::scanFiles()
{
    m_photoPaths.clear();
    m_videoPaths.clear();

    QDir photoDir(PHOTO_DIR);
    if (photoDir.exists()) {
        QFileInfoList list = photoDir.entryInfoList(
            QStringList() << "*.jpg" << "*.jpeg" << "*.png" << "*.bmp",
            QDir::Files, QDir::Time);
        foreach (const QFileInfo &fi, list)
            m_photoPaths.append(fi.absoluteFilePath());
    }

    QDir videoDir(VIDEO_DIR);
    if (videoDir.exists()) {
        QFileInfoList list = videoDir.entryInfoList(
            QStringList() << "*.avi" << "*.mp4" << "*.mov",
            QDir::Files, QDir::Time);
        foreach (const QFileInfo &fi, list)
            m_videoPaths.append(fi.absoluteFilePath());
    }

    ui->fileList->clear();

    if (m_filter == 0 || m_filter == 1) {
        QIcon photoIcon(":/images/icon_camera.svg");
        foreach (const QString &p, m_photoPaths) {
            QFileInfo fi(p);
            QListWidgetItem *it = new QListWidgetItem(photoIcon, fi.fileName());
            it->setData(Qt::UserRole, p);
            it->setData(Qt::UserRole + 1, 1);
            ui->fileList->addItem(it);
        }
    }

    if (m_filter == 0 || m_filter == 2) {
        QIcon videoIcon(":/images/icon_video.svg");
        foreach (const QString &p, m_videoPaths) {
            QFileInfo fi(p);
            QListWidgetItem *it = new QListWidgetItem(videoIcon, fi.fileName());
            it->setData(Qt::UserRole, p);
            it->setData(Qt::UserRole + 1, 2);
            ui->fileList->addItem(it);
        }
    }

    if (ui->fileList->count() == 0) {
        QListWidgetItem *empty = new QListWidgetItem(
            QString::fromUtf8("暂无文件"));
        empty->setFlags(Qt::NoItemFlags);
        empty->setTextAlignment(Qt::AlignCenter);
        ui->fileList->addItem(empty);
    }
}

/* ========== 预览 ========== */

void fops::onFileClicked(QListWidgetItem *item)
{
    if (!item || item->flags() == Qt::NoItemFlags)
        return;

    stopVideo();
    QString path = item->data(Qt::UserRole).toString();
    if (path.isEmpty()) return;

    m_curPath = path;
    m_renameBtn->setEnabled(true);
    showPreview(path);
}

void fops::showPreview(const QString &path)
{
    QString lower = path.toLower();

    if (lower.endsWith(".avi") || lower.endsWith(".mp4") || lower.endsWith(".mov")) {
        ui->playBtn->setEnabled(true);
        ui->playBtn->setText(QString::fromUtf8("▶ 播放"));

        try {
            cv::VideoCapture cap(path.toStdString());
            if (!cap.isOpened()) {
                ui->preview->setText(QString::fromUtf8("无法打开视频"));
                ui->playBtn->setEnabled(false);
                return;
            }
            cv::Mat frame;
            cap >> frame;
            cap.release();

            if (frame.empty()) {
                ui->preview->setText(QString::fromUtf8("视频无帧"));
                ui->playBtn->setEnabled(false);
                return;
            }
            cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
            QImage img(frame.data, frame.cols, frame.rows,
                       frame.step, QImage::Format_RGB888);
            ui->preview->setPixmap(
                QPixmap::fromImage(img).scaled(ui->preview->size(),
                                               Qt::IgnoreAspectRatio,
                                               Qt::SmoothTransformation));
        } catch (const cv::Exception &e) {
            ui->preview->setText(QString::fromUtf8("视频格式错误"));
            ui->playBtn->setEnabled(false);
            qWarning("fops: preview error %s", e.what());
        }
        return;
    }

    if (lower.endsWith(".jpg") || lower.endsWith(".jpeg") ||
        lower.endsWith(".png") || lower.endsWith(".bmp")) {
        ui->playBtn->setEnabled(false);
        QPixmap pm(path);
        if (pm.isNull()) {
            ui->preview->setText(QString::fromUtf8("无法加载图片"));
            return;
        }
        ui->preview->setPixmap(pm.scaled(ui->preview->size(),
                                         Qt::IgnoreAspectRatio,
                                         Qt::SmoothTransformation));
    }
}

/* ========== 视频播放 ========== */

void fops::onPlayVideo()
{
    if (m_playing) {
        stopVideo();
        ui->playBtn->setText(QString::fromUtf8("▶ 播放"));
        showPreview(m_curPath);
        return;
    }

    if (m_curPath.isEmpty()) return;

    m_vidCap = new cv::VideoCapture(m_curPath.toStdString());
    if (!m_vidCap->isOpened()) {
        delete m_vidCap;
        m_vidCap = 0;
        return;
    }

    m_playing = true;
    ui->playBtn->setText(QString::fromUtf8("■ 停止"));

    m_vidTimer = new QTimer(this);
    connect(m_vidTimer, SIGNAL(timeout()), this, SLOT(onVideoTick()));
    m_vidTimer->start(33);
}

void fops::onVideoTick()
{
    if (!m_vidCap || !m_vidCap->isOpened()) {
        stopVideo();
        return;
    }

    try {
        *m_vidCap >> m_vidFrame;
        if (m_vidFrame.empty()) {
            stopVideo();
            ui->playBtn->setText(QString::fromUtf8("▶ 播放"));
            showPreview(m_curPath);
            return;
        }
        cv::cvtColor(m_vidFrame, m_vidFrame, cv::COLOR_BGR2RGB);
        QImage img(m_vidFrame.data, m_vidFrame.cols, m_vidFrame.rows,
                   m_vidFrame.step, QImage::Format_RGB888);
        ui->preview->setPixmap(
            QPixmap::fromImage(img).scaled(ui->preview->size(),
                                           Qt::IgnoreAspectRatio,
                                           Qt::SmoothTransformation));
    } catch (const cv::Exception &e) {
        stopVideo();
        ui->playBtn->setText(QString::fromUtf8("▶ 播放"));
        ui->preview->setText(QString::fromUtf8("播放出错"));
        qWarning("fops: playback error %s", e.what());
    }
}

void fops::stopVideo()
{
    m_playing = false;
    if (m_vidTimer) {
        m_vidTimer->stop();
        delete m_vidTimer;
        m_vidTimer = 0;
    }
    if (m_vidCap) {
        if (m_vidCap->isOpened())
            m_vidCap->release();
        delete m_vidCap;
        m_vidCap = 0;
    }
}

/* ========== 操作按钮 ========== */

void fops::onDeleteFile()
{
    if (m_curPath.isEmpty()) return;
    stopVideo();

    if (QFile::remove(m_curPath))
        qDebug() << "Deleted:" << m_curPath;
    else
        qWarning("fops: failed to delete %s", qPrintable(m_curPath));

    m_curPath.clear();
    resetPreview();
    scanFiles();
}

/* ========== 改名 ========== */

void fops::onRenameFile()
{
    if (m_curPath.isEmpty()) return;
    stopVideo();

    QFileInfo fi(m_curPath);
    QString oldName = fi.fileName();
    QString dirPath = fi.absolutePath();

    bool ok;
    QString newName = QInputDialog::getText(
        this,
        QString::fromUtf8("重命名文件"),
        QString::fromUtf8("新文件名:"),
        QLineEdit::Normal,
        oldName,
        &ok);

    if (!ok || newName.isEmpty() || newName == oldName) return;

    // 禁止非法字符
    if (newName.contains('/') || newName.contains('\\')) {
        qWarning("fops: invalid chars in new name");
        return;
    }

    QString newPath = dirPath + "/" + newName;

    if (QFile::rename(m_curPath, newPath)) {
        qDebug() << "Renamed:" << m_curPath << "->" << newPath;
        m_curPath = newPath;
    } else {
        qWarning("fops: failed to rename %s", qPrintable(m_curPath));
    }

    m_renameBtn->setEnabled(true);
    scanFiles();

    // 选中改名后的文件
    QFileInfo newFi(newPath);
    for (int i = 0; i < ui->fileList->count(); i++) {
        QListWidgetItem *item = ui->fileList->item(i);
        if (item->data(Qt::UserRole).toString() == newPath) {
            ui->fileList->setCurrentItem(item);
            break;
        }
    }
}

void fops::onRefresh()
{
    stopVideo();
    m_curPath.clear();
    resetPreview();
    scanFiles();
}

/* ========== 筛选 ========== */

void fops::onFilterAll()
{
    stopVideo();
    m_filter = 0;
    ui->tabAll->setChecked(true);
    ui->tabPhoto->setChecked(false);
    ui->tabVideo->setChecked(false);
    m_curPath.clear();
    resetPreview();
    scanFiles();
}

void fops::onFilterPhoto()
{
    stopVideo();
    m_filter = 1;
    ui->tabAll->setChecked(false);
    ui->tabPhoto->setChecked(true);
    ui->tabVideo->setChecked(false);
    m_curPath.clear();
    resetPreview();
    scanFiles();
}

void fops::onFilterVideo()
{
    stopVideo();
    m_filter = 2;
    ui->tabAll->setChecked(false);
    ui->tabPhoto->setChecked(false);
    ui->tabVideo->setChecked(true);
    m_curPath.clear();
    resetPreview();
    scanFiles();
}

/* ========== 返回 ========== */

void fops::on_pushButton_clicked()
{
    stopVideo();
    this->close();
}
