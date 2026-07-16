#include "loginwidget.h"
#include "ui_loginwidget.h"
#include "registerwidget.h"
#include "databasemanager.h"
#include "faceEngine.h"
#include "facelogindialog.h"

#include <QPainter>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QDebug>

LoginWidget::LoginWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
    setWindowTitle("安防系统 - 登录");
    resize(1024, 768);
    show();

    QPixmap logo(48, 48);
    logo.fill(Qt::transparent);
    {
        QPainter lp(&logo);
        lp.setRenderHint(QPainter::Antialiasing);
        lp.setBrush(QColor("#4a90d9"));
        lp.setPen(Qt::NoPen);
        lp.drawEllipse(0, 0, 48, 48);
        lp.setBrush(Qt::white);
        QPolygonF tri;
        tri << QPointF(16, 34) << QPointF(16, 14) << QPointF(32, 24);
        lp.drawPolygon(tri);
        lp.drawEllipse(QPointF(24, 24), 5, 5);
    }
    ui->logoIcon->setPixmap(logo);

    connect(ui->loginBtn, &QPushButton::clicked, this, &LoginWidget::onLoginClicked);
    connect(ui->registerBtn, &QPushButton::clicked, this, &LoginWidget::onRegisterClicked);
    connect(ui->passwordInput, &QLineEdit::returnPressed, this, &LoginWidget::onLoginClicked);
    connect(ui->usernameInput, &QLineEdit::returnPressed, [this]() {
        ui->passwordInput->setFocus();
    });

    ui->statusLabel->hide();

    // 人脸登录按钮
    QPushButton *faceBtn = new QPushButton("人脸登录", ui->formContainer);
    faceBtn->setFixedHeight(44);
    faceBtn->setCursor(Qt::PointingHandCursor);
    faceBtn->setStyleSheet(
        "QPushButton { background-color: #27ae60; color: white; border: none; border-radius: 6px; font-size: 16px; font-weight: bold; }"
        "QPushButton:hover { background-color: #219a52; }");
    connect(faceBtn, &QPushButton::clicked, this, &LoginWidget::onFaceLoginClicked);

    QLayout *formLayout = ui->formContainer->layout();
    QHBoxLayout *btnLayout = nullptr;
    for (int i = 0; i < formLayout->count(); i++) {
        QLayoutItem *item = formLayout->itemAt(i);
        if (item && item->layout()) {
            QHBoxLayout *hbl = qobject_cast<QHBoxLayout*>(item->layout());
            if (hbl && hbl->indexOf(ui->registerBtn) >= 0) {
                btnLayout = hbl;
                break;
            }
        }
    }
    if (btnLayout) {
        int idx = btnLayout->indexOf(ui->registerBtn);
        if (idx >= 0) btnLayout->insertWidget(idx, faceBtn);
    }
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::onLoginClicked()
{
    QString user = ui->usernameInput->text().trimmed();
    QString pass = ui->passwordInput->text();
    if (user.isEmpty() || pass.isEmpty()) {
        ui->statusLabel->setText("请输入账号和密码");
        ui->statusLabel->show();
        return;
    }
    DatabaseManager db("/opt/aicTrain/central/media.db");
    if (!db.initDatabase()) {
        ui->statusLabel->setText("数据库初始化失败");
        ui->statusLabel->show();
        return;
    }
    if (db.checkLogin(user, pass)) {
        qDebug() << "Login success:" << user;
        emit loginSuccess(user);
    } else {
        ui->statusLabel->setText("账号或密码错误");
        ui->statusLabel->show();
        ui->passwordInput->clear();
        ui->passwordInput->setFocus();
    }
}

void LoginWidget::onRegisterClicked()
{
    RegisterWidget *reg = new RegisterWidget(this);
    connect(reg, &RegisterWidget::registered, this, &LoginWidget::onRegistered);
    reg->exec();
    delete reg;
}

void LoginWidget::onRegistered(const QString &username)
{
    ui->usernameInput->setText(username);
    ui->passwordInput->setFocus();
    ui->statusLabel->setStyleSheet("color: #27ae60; font-size: 13px; background: transparent;");
    ui->statusLabel->setText("Registered, please login");
    ui->statusLabel->show();
}

void LoginWidget::onFaceLoginClicked()
{
    FaceEngine engine;
    engine.loadCascades("/opt/aicTrain/sMonitor_server");
    engine.loadModel("/opt/aicTrain/sMonitor_server/facedata/MyFacePCAModel.xml");
    engine.loadNames("/opt/aicTrain/sMonitor_server/facedata/Faces/name.txt");

    FaceLoginDialog dlg(&engine, this);
    if (dlg.exec() == QDialog::Accepted) {
        QString user = dlg.loggedInUser();
        if (user.isEmpty()) user = "admin";
        emit loginSuccess(user);
    }
}

void LoginWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    drawBackground(p);
}

void LoginWidget::drawBackground(QPainter &p)
{
    int w = width(), h = height();
    QLinearGradient grad(0, 0, 0, h);
    grad.setColorAt(0.0,  QColor("#4a90d9"));
    grad.setColorAt(0.35, QColor("#6bb3f0"));
    grad.setColorAt(0.45, QColor("#8ac4f8"));
    grad.setColorAt(0.55, QColor("#5a9ec8"));
    grad.setColorAt(0.75, QColor("#2a6a9a"));
    grad.setColorAt(1.0,  QColor("#1a3a5a"));
    p.fillRect(rect(), grad);
    p.setBrush(QColor(255, 255, 255, 40));
    p.setPen(Qt::NoPen);
    p.drawEllipse(100, 60, 120, 40);
    p.drawEllipse(140, 45, 90, 35);
    p.drawEllipse(80, 55, 80, 30);
    p.drawEllipse(w - 250, 80, 100, 35);
    p.drawEllipse(w - 200, 65, 80, 30);
    QRadialGradient sunGrad(w * 0.65, h * 0.45, 120);
    sunGrad.setColorAt(0.0, QColor(255, 220, 150, 80));
    sunGrad.setColorAt(0.5, QColor(255, 200, 100, 30));
    sunGrad.setColorAt(1.0, QColor(255, 200, 100, 0));
    p.setBrush(sunGrad);
    p.drawEllipse(QPointF(w * 0.65, h * 0.45), 120, 80);
    p.fillRect(rect(), QColor(0, 0, 0, 60));
    p.setBrush(QColor(255, 255, 255, 8));
    for (int i = 0; i < 20; i++) {
        int y = h * 0.70 + i * 12;
        int x = 40 + (i * 17) % 200;
        p.drawRect(x, y, 80 + (i * 7) % 60, 2);
        p.drawRect(w - x - 100, y, 60 + (i * 11) % 40, 2);
    }
}
