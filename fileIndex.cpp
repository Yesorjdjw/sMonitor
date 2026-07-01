#include "fileIndex.h"
#include "ui_fileIndex.h"
#include <QIcon>
#include <QScrollBar>
#include <QDateTime>
#include <QFileInfo>
#include <QImage>
#include <QApplication>

#include "keyboard.h"
static const QSize IMAGE_SIZE(238, 160);
static const QSize ITEM_SIZE(238, 190);

/* ========== ThumbnailWorker ========== */

void ThumbnailWorker::setJobs(const QList<Job> &jobs)
{
    QMutexLocker lock(&m_mutex);
    m_jobs = jobs;
}

void ThumbnailWorker::processJobs()
{
    QList<Job> jobs;
    { QMutexLocker lock(&m_mutex); jobs = m_jobs; m_jobs.clear(); }

    for (const auto &job : jobs) {
        if (!job.cache.isEmpty() && QFile::exists(job.cache)) {
            QPixmap cached(job.cache);
            if (!cached.isNull()) { emit thumbReady(job.idx, cached); continue; }
        }
        QPixmap thumb = grabFrame(job.path);
        if (!thumb.isNull() && !job.cache.isEmpty()) {
            QDir().mkpath(QFileInfo(job.cache).absolutePath());
            thumb.scaled(IMAGE_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation)
                .save(job.cache, "JPG", 85);
        }
        emit thumbReady(job.idx, thumb);
    }
}

QPixmap ThumbnailWorker::grabFrame(const QString &videoPath)
{
    cv::VideoCapture cap(videoPath.toStdString());
    if (!cap.isOpened()) return QPixmap();
    cv::Mat frame; cap >> frame; cap.release();
    if (frame.empty()) return QPixmap();
    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
    QImage img(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
    return QPixmap::fromImage(img.copy());
}

/* ========== fileIndex ========== */

fileIndex::fileIndex(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::fileIndex), m_fullScreen(nullptr), m_busy(false)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setGeometry(0, 0, 1024, 600);

    initWidget(ui->picWidget);
    initWidget(ui->vidWidget);
    ui->bottomBar->setVisible(true);
    ui->bottomBar->raise();
    ui->bottomBar->setAttribute(Qt::WA_AlwaysStackOnTop, true);
    ui->renameBt->setEnabled(false);
    ui->deleteBt->setEnabled(false);

    // 后台缩略图线程
    m_thumbWorker = new ThumbnailWorker;
    m_thumbThread = new QThread(this);
    m_thumbWorker->moveToThread(m_thumbThread);
    connect(m_thumbWorker, SIGNAL(thumbReady(int,QPixmap)),
            this, SLOT(onThumbReady(int,QPixmap)), Qt::QueuedConnection);
    connect(m_thumbThread, SIGNAL(finished()), m_thumbWorker, SLOT(deleteLater()));
    m_thumbThread->start();

    // 虚拟键盘（触摸屏）
    m_keyboard = new VirtualKeyboard(this);
    m_renameEdit = nullptr;
    connect(m_keyboard, &VirtualKeyboard::confirmed, this, &fileIndex::onKeyboardDone);
    connect(ui->searchEdit, &QLineEdit::textChanged, this, &fileIndex::on_searchEdit_textChanged);
    connect(qApp, &QApplication::focusChanged, this, [this](QWidget *old, QWidget *now) {
        Q_UNUSED(old);
        if (now == ui->searchEdit && !m_keyboard->isVisible()) {
            m_keyboard->setTarget(ui->searchEdit);
            m_keyboard->showKeyboard();
        }
    });

    // \xe8\x99\x9a\xe6\x8b\x9f\xe9\x94\xae\xe7\x9b\x98\xef\xbc\x88\xe8\xa7\xa6\xe6\x91\xb8\xe5\xb1\x8f\xef\xbc\x89
    connect(ui->exit_bt,     SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->refreshBt,   SIGNAL(clicked()), this, SLOT(on_refreshBt_clicked()));
    connect(ui->renameBt,    SIGNAL(clicked()), this, SLOT(on_renameBt_clicked()));
    connect(ui->deleteBt,    SIGNAL(clicked()), this, SLOT(on_deleteBt_clicked()));
    connect(ui->picWidget,   SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                             this, SLOT(on_picWidget_itemDoubleClicked(QListWidgetItem*)));
    connect(ui->vidWidget,   SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                             this, SLOT(on_vidWidget_itemDoubleClicked(QListWidgetItem*)));
    connect(ui->picWidget,   SIGNAL(itemClicked(QListWidgetItem*)),
                             this, SLOT(onItemClicked(QListWidgetItem*)));
    connect(ui->vidWidget,   SIGNAL(itemClicked(QListWidgetItem*)),
                             this, SLOT(onItemClicked(QListWidgetItem*)));
}

fileIndex::~fileIndex()
{
    m_thumbThread->quit();
    m_thumbThread->wait();
    delete ui;
}

void fileIndex::initWidget(QListWidget *w)
{
    w->setIconSize(IMAGE_SIZE);
    w->setViewMode(QListView::IconMode);
    w->setMovement(QListView::Static);
    w->setResizeMode(QListView::Adjust);
    w->setSpacing(8);
    w->setFocusPolicy(Qt::NoFocus);
    w->setFrameShape(QListWidget::NoFrame);
    w->setStyleSheet(
        "QListWidget::Item{padding-top:10px; padding-bottom:4px;}"
        "QListWidget::Item:hover{background:skyblue;}"
        "QListWidget::item:selected{background:lightgreen; color:red;}"
        "QListWidget::item:selected:!active{border-width:0px; background:lightgreen;}"
        "QListWidget{background:skyblue;}");
}

/* ========== 加载照片 ========== */

void fileIndex::updatePicData(const QString &p_name)
{
    m_picPath = p_name;
    ui->picWidget->clear();

    QDir dir(m_picPath);
    if (!dir.exists()) return;
    QStringList filters; filters << "*.png" << "*.jpg" << "*.jpeg" << "*.bmp";
    dir.setNameFilters(filters);
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    picList = dir.entryList();
    if (picList.isEmpty()) return;

    QString cacheDir = m_picPath + "/.full_cache";
    QDir().mkpath(cacheDir);

    for (int i = 0; i < picList.count(); ++i) {
        QString fn = picList.at(i), fp = m_picPath + "/" + fn;
        QString cf = cacheDir + "/" + fn;
        if (!QFile::exists(cf)) {
            QImage img(fp);
            if (!img.isNull())
                img.scaled(1024,600,Qt::IgnoreAspectRatio,Qt::SmoothTransformation).save(cf,"JPG",85);
        }
        QPixmap thumb(fp);
        QListWidgetItem *item = new QListWidgetItem(
            QIcon(thumb.scaled(IMAGE_SIZE,Qt::KeepAspectRatio,Qt::SmoothTransformation)), fn);
        QFont f; f.setBold(true); item->setFont(f);
        item->setTextColor(QColor(255,255,255));
        item->setSizeHint(ITEM_SIZE);
        item->setData(Qt::UserRole, fp);
        item->setData(Qt::UserRole+1, cf);
        ui->picWidget->addItem(item);
    }
    if (ui->tabWidget->indexOf(ui->picWidget) < 0)
        ui->tabWidget->addTab(ui->picWidget, QString::fromUtf8("照片"));
}

/* ========== 加载视频 ========== */

void fileIndex::updataVidData(const QString &vp_name, const QString &v_name)
{
    m_picPath = vp_name; m_vidPath = v_name;

    QDir vDir(v_name);
    if (!vDir.exists()) return;
    QStringList vFilters; vFilters << "*.avi" << "*.mp4" << "*.mov";
    vDir.setNameFilters(vFilters);
    vDir.setFilter(QDir::Files | QDir::NoSymLinks);
    QStringList vList = vDir.entryList();
    if (vList.isEmpty()) return;

    vidList.clear();
    QList<ThumbnailWorker::Job> jobs;
    QString cacheDir = v_name + "/.thumb_cache";

    for (int i = 0; i < vList.count(); ++i) {
        QString vf = vList.at(i), fp = v_name + "/" + vf;
        QListWidgetItem *item = new QListWidgetItem(QIcon(":/images/icon_video.svg"), vf);
        QFont f; f.setBold(true); item->setFont(f);
        item->setTextColor(QColor(255,255,255));
        item->setSizeHint(ITEM_SIZE);
        item->setData(Qt::UserRole, fp);
        ui->vidWidget->addItem(item);
        vidList.append(vf);
        ThumbnailWorker::Job j; j.idx = i; j.path = fp;
        j.cache = cacheDir + "/" + QFileInfo(vf).baseName() + ".jpg";
        jobs.append(j);
    }
    m_thumbWorker->setJobs(jobs);
    QMetaObject::invokeMethod(m_thumbWorker, "processJobs", Qt::QueuedConnection);
    if (ui->tabWidget->indexOf(ui->vidWidget) < 0)
        ui->tabWidget->addTab(ui->vidWidget, QString::fromUtf8("视频"));
}

void fileIndex::onThumbReady(int idx, const QPixmap &thumb)
{
    if (idx < 0 || idx >= ui->vidWidget->count()) return;
    QListWidgetItem *item = ui->vidWidget->item(idx);
    if (!item || thumb.isNull()) return;
    item->setIcon(QIcon(thumb.scaled(IMAGE_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
}

/* ========== 搜索过滤 ========== */

void fileIndex::on_searchEdit_textChanged(const QString &text)
{
    m_searchFilter = text;
    applyFilter();
    // \xe8\x99\x9a\xe6\x8b\x9f\xe9\x94\xae\xe7\x9b\x98\xef\xbc\x9a\xe7\x82\xb9\xe5\x87\xbb\xe6\x90\x9c\xe7\xb4\xa2\xe6\xa1\x86\xe6\x97\xb6\xe5\xbc\xb9\xe5\x87\xba
    if (!text.isEmpty() && !m_keyboard->isVisible()) {
        m_keyboard->setTarget(ui->searchEdit);
        m_keyboard->showKeyboard();
    }
}

void fileIndex::applyFilter()
{
    for (int t = 0; t < ui->tabWidget->count(); ++t) {
        QListWidget *w = qobject_cast<QListWidget*>(ui->tabWidget->widget(t));
        if (!w) continue;
        for (int i = 0; i < w->count(); ++i) {
            QListWidgetItem *item = w->item(i);
            if (!item) continue;
            bool match = m_searchFilter.isEmpty() ||
                         item->text().contains(m_searchFilter, Qt::CaseInsensitive);
            item->setHidden(!match);
        }
    }
}

/* ========== 刷新 ========== */

void fileIndex::on_refreshBt_clicked()
{
    refreshList();
}

/* ========== 工具 ========== */

void fileIndex::refreshList()
{
    int curIdx = ui->tabWidget->currentIndex();
    // 不清除tab，只清空列表项重新加载
    ui->picWidget->clear();
    ui->vidWidget->clear();
    vidList.clear();
    updatePicData(m_picPath);
    updataVidData(m_picPath, m_vidPath);
    if (curIdx >= 0 && curIdx < ui->tabWidget->count())
        ui->tabWidget->setCurrentIndex(curIdx);
    applyFilter();
}

void fileIndex::on_exit_bt_clicked()
{
    emit exitRequested();
    close();
}

QString fileIndex::selectedFilePath()
{
    QListWidgetItem *item = NULL;
    if (ui->tabWidget->currentIndex() == 0) item = ui->picWidget->currentItem();
    else if (ui->tabWidget->currentIndex() == 1) item = ui->vidWidget->currentItem();
    return item ? item->data(Qt::UserRole).toString() : QString();
}

bool fileIndex::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->searchEdit && event->type() == QEvent::MouseButtonPress) {
        m_keyboard->setTarget(ui->searchEdit);
        m_keyboard->showKeyboard();
    }
    return QMainWindow::eventFilter(obj, event);
}

void fileIndex::onKeyboardDone()
{
    if (m_renameEdit && m_renameEdit->isVisible()) {
        m_renameEdit->hide();
        QString nn = m_renameEdit->text().trimmed();
        QFileInfo fi(m_renamePath);
        if (!nn.isEmpty() && nn != m_renameOldName
            && !nn.contains('/') && !nn.contains('\\')
            && QFile::rename(m_renamePath, fi.absolutePath() + "/" + nn)) {
            refreshList();
        }
        m_renamePath.clear();
        m_renameOldName.clear();
    }
    m_busy.store(false);
    ui->renameBt->setEnabled(true);
    ui->deleteBt->setEnabled(true);
}

void fileIndex::onItemClicked(QListWidgetItem *)
{
    ui->bottomBar->show(); ui->bottomBar->raise();
    ui->renameBt->setEnabled(true); ui->deleteBt->setEnabled(true);
}

/* ========== 双击全屏 ========== */

void fileIndex::on_picWidget_itemDoubleClicked(QListWidgetItem *item)
{
    if (!item || m_busy.load()) return;
    m_busy.store(true);
    QString fp = item->data(Qt::UserRole).toString();
    if (fp.isEmpty()) { m_busy.store(false); return; }
    QString cf = item->data(Qt::UserRole+1).toString();
    QString lp = (!cf.isEmpty() && QFile::exists(cf)) ? cf : fp;
    m_fullScreen = new fullShow(this);
    m_fullScreen->setAttribute(Qt::WA_DeleteOnClose);
    m_fullScreen->show();
    QPixmap pix(lp);
    if (!pix.isNull()) m_fullScreen->show_picture(pix);
    m_busy.store(false);
}

void fileIndex::on_vidWidget_itemDoubleClicked(QListWidgetItem *item)
{
    if (!item || m_busy.load()) return;
    m_busy.store(true);
    QString fp = item->data(Qt::UserRole).toString();
    if (fp.isEmpty()) { m_busy.store(false); return; }

#ifdef __arm__
    // ARM: 隐藏列表→播放→播完恢复+刷新
    this->hide();
    QProcess *mp = new QProcess();
    QString cmd = "mplayer -quiet -geometry 0:0 -zoom -x 1024 -y 600 \"" + fp + "\"";
    mp->start(cmd);
    connect(mp, SIGNAL(finished(int)), this, SLOT(show()));
    connect(mp, SIGNAL(finished(int)), this, SLOT(on_refreshBt_clicked()));
    connect(mp, SIGNAL(finished(int)), mp, SLOT(deleteLater()));
#else
    m_fullScreen = new fullShow(this);
    m_fullScreen->setAttribute(Qt::WA_DeleteOnClose);
    m_fullScreen->show_video(fp);
    m_fullScreen->show();
#endif
    m_busy.store(false);
}

/* ========== 改名 ========== */

void fileIndex::on_renameBt_clicked()
{
    if (m_busy.load()) return;
    m_busy.store(true); ui->renameBt->setEnabled(false);
    QString fp = selectedFilePath();
    if (fp.isEmpty()) { m_busy.store(false); ui->renameBt->setEnabled(true); return; }
    QFileInfo fi(fp);

    // 触摸屏：使用虚拟键盘代替 QInputDialog
    if (!m_renameEdit) {
        m_renameEdit = new QLineEdit(this);
        m_renameEdit->setGeometry(260, 200, 504, 42);
        m_renameEdit->setStyleSheet(
            "QLineEdit { background: white; border: 2px solid #1a7bbd;"
            " border-radius: 5px; font-size: 20px; padding: 4px 10px; }");
        m_renameEdit->setFocusPolicy(Qt::NoFocus);
    }
    m_renamePath = fp;
    m_renameOldName = fi.fileName();
    m_renameEdit->setText(m_renameOldName);
    m_renameEdit->show();
    m_renameEdit->raise();
    m_renameEdit->selectAll();
    m_keyboard->setTarget(m_renameEdit);
    m_keyboard->showKeyboard();
    // 按下键盘"确定"后由 onKeyboardDone 处理
}

/* ========== 删除 ========== */

void fileIndex::on_deleteBt_clicked()
{
    if (m_busy.load()) return;
    m_busy.store(true); ui->deleteBt->setEnabled(false);
    QString fp = selectedFilePath();
    if (fp.isEmpty()) { m_busy.store(false); ui->deleteBt->setEnabled(true); return; }
    QFileInfo fi(fp);
    /* 直接删除，不加弹窗确认（触摸屏弹窗会触发虚拟键盘） */
    if (QFile::remove(fp)) {
        QDir d = fi.absoluteDir();
        QFile::remove(d.absolutePath()+"/.full_cache/"+fi.fileName());
        QFile::remove(d.absolutePath()+"/.thumb_cache/"+fi.completeBaseName()+".jpg");
        refreshList();
    }
    m_busy.store(false); ui->deleteBt->setEnabled(true);
    /* 清除焦点，防止虚拟键盘弹出 */
    ui->searchEdit->clearFocus();
    m_keyboard->hideKeyboard();
}
