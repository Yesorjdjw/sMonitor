#include "filemanagerwidget.h"
#include "ui_filemanagerwidget.h"
#include "mediapreview.h"

#include <QPainter>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QInputDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QVBoxLayout>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>

// ============================================================
// ThumbnailCard — 缩略图卡片
// QLabel 控件显示文字（避免 QPainter 中文渲染问题）
// ============================================================
class ThumbnailCard : public QWidget
{
    Q_OBJECT
public:
    ThumbnailCard(const MediaFile &file, QWidget *parent = nullptr)
        : QWidget(parent), m_file(file)
    {
        setObjectName("ThumbCard");
        setFixedSize(200, 175);
        setCursor(Qt::PointingHandCursor);
        setMouseTracking(true);

        QVBoxLayout *lay = new QVBoxLayout(this);
        lay->setContentsMargins(4, 4, 4, 4);
        lay->setSpacing(0);

        // ---- Thumbnail image ----
        m_thumbLabel = new QLabel();
        m_thumbLabel->setFixedSize(192, 120);
        m_thumbLabel->setAlignment(Qt::AlignCenter);
        m_thumbLabel->setStyleSheet("background-color: #e0e4ea; border-radius: 4px;");

        // Load thumbnail
        QString path = file.filepath;
        if (!file.thumbnail.isEmpty() && QFile::exists(file.thumbnail))
            path = file.thumbnail;

        QPixmap thumb;
        if (QFile::exists(path)) {
            thumb.load(path);
            if (!thumb.isNull())
                thumb = thumb.scaled(192, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }

        if (!thumb.isNull()) {
            m_thumbLabel->setPixmap(thumb);
            m_thumbLabel->setStyleSheet("background: transparent;");
        } else {
            // Placeholder icon
            QString icon = (file.type == "recording") ? ">>" : "[]";
            m_thumbLabel->setText(QString("<span style='font-size:36px; color:#a0a8b4;'>%1</span>").arg(icon));
            m_thumbLabel->setStyleSheet("background-color: #e0e4ea; border-radius: 4px;");
        }
        lay->addWidget(m_thumbLabel, 0, Qt::AlignCenter);

        // ---- Type badge (overlay on thumb, positioned with a small spacer) ----
        // Badge will be drawn in paintEvent

        // ---- Filename ----
        QString displayName = file.filename;
        if (displayName.length() > 18)
            displayName = displayName.left(16) + "...";

        m_nameLabel = new QLabel(displayName);
        m_nameLabel->setFixedHeight(20);
        m_nameLabel->setStyleSheet("font-size: 12px; font-weight: bold; color: #2c3e50; padding: 2px 4px; background: transparent;");
        m_nameLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        lay->addWidget(m_nameLabel);

        // ---- Info line ----
        QString info;
        if (file.size > 1024 * 1024)
            info = QString("%1 MB").arg(file.size / (1024.0 * 1024.0), 0, 'f', 1);
        else if (file.size > 1024)
            info = QString("%1 KB").arg(file.size / 1024);
        else if (file.size > 0)
            info = QString("%1 B").arg(file.size);
        if (!file.createdAt.isEmpty()) {
            if (!info.isEmpty()) info += "  |  ";
            info += file.createdAt.mid(5, 11);
        }

        m_infoLabel = new QLabel(info);
        m_infoLabel->setFixedHeight(18);
        m_infoLabel->setStyleSheet("font-size: 10px; color: #8a9aaa; padding: 2px 4px; background: transparent;");
        m_infoLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        lay->addWidget(m_infoLabel);

        lay->addStretch();
    }

    int fileId() const { return m_file.id; }
    MediaFile fileInfo() const { return m_file; }

signals:
    void clicked(int fileId);
    void doubleClicked(int fileId);
    void contextMenuRequested(int fileId, const QPoint &globalPos);

protected:
    void paintEvent(QPaintEvent *event) override
    {
        // Draw card border + hover effect
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        p.setBrush(m_hovered ? QColor("#f0f5ff") : Qt::white);
        p.setPen(QPen(m_hovered ? QColor("#4a90d9") : QColor("#dce0e6"), 1));
        p.drawRoundedRect(rect().adjusted(0, 0, -1, -1), 6, 6);

        // Type badge
        QRect badge(rect().right() - 48, rect().top() + 8, 40, 18);
        p.setBrush(m_file.type == "recording" ? QColor("#e74c3c") : QColor("#4a90d9"));
        p.setPen(Qt::NoPen);
        p.drawRoundedRect(badge, 3, 3);
        p.setPen(Qt::white);
        QFont bf = font();
        bf.setPixelSize(10);
        bf.setBold(true);
        p.setFont(bf);
        p.drawText(badge, Qt::AlignCenter, m_file.type == "recording" ? "AVI" : "JPG");
    }

    void mousePressEvent(QMouseEvent *e) override
    {
        if (e->button() == Qt::RightButton)
            emit contextMenuRequested(m_file.id, e->globalPos());
        else
            emit clicked(m_file.id);
    }

    void mouseDoubleClickEvent(QMouseEvent *e) override
    {
        Q_UNUSED(e)
        emit doubleClicked(m_file.id);
    }

    void enterEvent(QEvent *e) override { Q_UNUSED(e); m_hovered = true; update(); }
    void leaveEvent(QEvent *e) override { Q_UNUSED(e); m_hovered = false; update(); }

private:
    MediaFile m_file;
    QLabel *m_thumbLabel;
    QLabel *m_nameLabel;
    QLabel *m_infoLabel;
    bool m_hovered = false;
};

// ============================================================
// FileManagerWidget — 文件管理页面
// ============================================================
FileManagerWidget::FileManagerWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::FileManagerWidget)
{
    ui->setupUi(this);
    ui->rootLayout->setStretch(1, 1);

    // Create grid container programmatically
    m_gridContainer = new QWidget();
    m_gridContainer->setStyleSheet("background: transparent;");
    m_gridLayout = new QGridLayout(m_gridContainer);
    m_gridLayout->setContentsMargins(4, 4, 4, 4);
    m_gridLayout->setSpacing(12);
    m_gridLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    ui->scrollArea->setWidget(m_gridContainer);

    // Ensure page label is visible
    ui->pageLabel->setText("暂无文件");
    ui->pageLabel->setStyleSheet("color: #5a6a8a; font-size: 14px; padding: 0 8px; background: transparent;");

    setupConnections();

    m_net = new QNetworkAccessManager(this); // 删除操作调用中心 API

    // Default: show recordings
    ui->videoBtn->setChecked(true);
}

FileManagerWidget::~FileManagerWidget()
{
    delete ui;
}

void FileManagerWidget::setupConnections()
{
    // Sidebar category switching
    connect(ui->videoBtn, &QPushButton::clicked, [this]() { switchCategory("recording"); });
    connect(ui->photoBtn, &QPushButton::clicked, [this]() { switchCategory("photo"); });
    connect(ui->searchBtn, &QPushButton::clicked, [this]() { switchCategory(""); });

    // Pagination
    connect(ui->prevBtn, &QPushButton::clicked, [this]() {
        if (m_currentPage > 1) loadFiles(m_currentPage - 1);
    });
    connect(ui->nextBtn, &QPushButton::clicked, [this]() {
        if (m_currentPage < m_totalPages) loadFiles(m_currentPage + 1);
    });

    // Search button
    connect(ui->searchPageBtn, &QPushButton::clicked, [this]() {
        bool ok;
        QString keyword = QInputDialog::getText(this, "搜索文件",
            "输入文件名关键词：", QLineEdit::Normal, "", &ok);
        if (ok && !keyword.isEmpty() && m_db) {
            int total = m_db->getMediaFileCount(m_currentType);
            QList<MediaFile> all = m_db->getMediaFiles(m_currentType, 1, total);
            m_cachedFilter = keyword;
            m_cachedFiles.clear();
            for (const auto &f : all) {
                if (f.filename.contains(keyword, Qt::CaseInsensitive))
                    m_cachedFiles.append(f);
            }
            clearCards();
            int col = 0, row = 0;
            for (const auto &f : m_cachedFiles) {
                ThumbnailCard *card = new ThumbnailCard(f, this);
                connectCard(card);
                m_gridLayout->addWidget(card, row, col++, Qt::AlignLeft | Qt::AlignTop);
                m_cards.append(card);
                if (col >= 3) { col = 0; row++; }
            }
            ui->pageLabel->setText(QString("搜索: %1 条结果").arg(m_cachedFiles.size()));
            ui->prevBtn->setEnabled(false);
            ui->nextBtn->setEnabled(false);
        }
    });
}

void FileManagerWidget::connectCard(ThumbnailCard *card)
{
    connect(card, &ThumbnailCard::clicked, this, &FileManagerWidget::onCardClicked);
    connect(card, &ThumbnailCard::doubleClicked, this, &FileManagerWidget::onCardDoubleClicked);
    connect(card, &ThumbnailCard::contextMenuRequested, this, &FileManagerWidget::onContextMenu);
}

void FileManagerWidget::setDatabase(DatabaseManager *db)
{
    m_db = db;
    refreshFiles();
}

void FileManagerWidget::refreshFiles()
{
    m_cachedFilter.clear();
    m_cachedFiles.clear();
    loadFiles(1);
}

void FileManagerWidget::showCategory(const QString &type)
{
    switchCategory(type);
}

void FileManagerWidget::clearCards()
{
    for (auto *c : m_cards) {
        m_gridLayout->removeWidget(c);
        c->deleteLater();
    }
    m_cards.clear();
}

void FileManagerWidget::switchCategory(const QString &type)
{
    m_currentType = type;
    ui->videoBtn->setChecked(type == "recording");
    ui->photoBtn->setChecked(type == "photo");
    ui->searchBtn->setChecked(type.isEmpty());
    m_cachedFilter.clear();
    m_cachedFiles.clear();
    loadFiles(1);
}

void FileManagerWidget::loadFiles(int page)
{
    clearCards();
    if (!m_db) {
        ui->pageLabel->setText("数据库未连接");
        return;
    }

    m_currentPage = page;
    int total = m_db->getMediaFileCount(m_currentType);
    m_totalPages = qMax(1, (total + m_pageSize - 1) / m_pageSize);

    if (total == 0) {
        ui->pageLabel->setText(
            QString("暂无%1文件").arg(
                m_currentType == "recording" ? "录像" :
                m_currentType == "photo" ? "照片" : ""));
    } else {
        ui->pageLabel->setText(QString("第 %1 / %2 页（共 %3 个文件）")
                               .arg(m_currentPage).arg(m_totalPages).arg(total));
    }

    QList<MediaFile> files = m_db->getMediaFiles(m_currentType, m_currentPage, m_pageSize);

    int col = 0, row = 0;
    for (const auto &f : files) {
        ThumbnailCard *card = new ThumbnailCard(f, this);
        connectCard(card);
        m_gridLayout->addWidget(card, row, col, Qt::AlignLeft | Qt::AlignTop);
        m_cards.append(card);
        if (++col >= 3) { col = 0; row++; }
    }

    m_gridLayout->setRowStretch(row + 1, 1);
    ui->prevBtn->setEnabled(m_currentPage > 1);
    ui->nextBtn->setEnabled(m_currentPage < m_totalPages);
    qDebug() << "Files loaded:" << files.size() << "total:" << total;
}

void FileManagerWidget::onCardClicked(int fileId)
{
    qDebug() << "Card clicked:" << fileId;
}

void FileManagerWidget::onCardDoubleClicked(int fileId)
{
    // Collect all files for navigation
    QList<MediaFile> allFiles;
    if (!m_cachedFiles.isEmpty()) {
        allFiles = m_cachedFiles;
    } else {
        if (m_db) {
            int total = m_db->getMediaFileCount(m_currentType);
            allFiles = m_db->getMediaFiles(m_currentType, 1, total);
        }
    }

    int idx = 0;
    for (int i = 0; i < allFiles.size(); i++) {
        if (allFiles[i].id == fileId) { idx = i; break; }
    }

    MediaPreview *preview = new MediaPreview(this->window());
    preview->resize(900, 650);
    preview->showFiles(allFiles, idx);
    delete preview;
}

void FileManagerWidget::onContextMenu(int fileId, const QPoint &globalPos)
{
    MediaFile target;
    bool found = false;
    for (auto *c : m_cards) {
        if (c->fileId() == fileId) {
            target = c->fileInfo();
            found = true;
            break;
        }
    }
    if (!found) return;

    QMenu menu;
    QAction *actOpen = menu.addAction("打开");
    QAction *actDelete = menu.addAction("删除");
    QAction *actShow = menu.addAction("在文件夹中显示");

    QAction *chosen = menu.exec(globalPos);
    if (!chosen) return;

    if (chosen == actOpen) {
        onCardDoubleClicked(fileId);
    } else if (chosen == actDelete) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "确认删除",
            QString("确定要删除「%1」吗？").arg(target.filename),
            QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            // 删除走中心 API:中心服务删文件+库并广播 /MEDIA_DELETE,各端(含本端)据此刷新同步
            QNetworkRequest req(QUrl("http://127.0.0.1:8090/api/media/delete"));
            req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
            QByteArray body = "{\"id\":" + QByteArray::number(fileId) + "}";
            QNetworkReply *rep = m_net->post(req, body);
            connect(rep, &QNetworkReply::finished, rep, &QNetworkReply::deleteLater);
        }
    } else if (chosen == actShow) {
        QFileInfo fi(target.filepath);
        QDesktopServices::openUrl(QUrl::fromLocalFile(fi.dir().absolutePath()));
    }
}

#include "filemanagerwidget.moc"
