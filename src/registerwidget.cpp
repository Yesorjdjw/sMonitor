#include "registerwidget.h"
#include "ui_registerwidget.h"
#include "databasemanager.h"

#include <QMessageBox>
#include <QDebug>
#include <QPushButton>

RegisterWidget::RegisterWidget(QWidget *parent)
    : QDialog(parent), ui(new Ui::RegisterWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    setFixedSize(420, 420);
    setModal(true);

    connect(ui->registerBtn, &QPushButton::clicked, this, &RegisterWidget::onRegisterClicked);
    connect(ui->cancelBtn, &QPushButton::clicked, this, &RegisterWidget::onCancelClicked);

    // Allow Enter to submit
    connect(ui->confirmInput, &QLineEdit::returnPressed, this, &RegisterWidget::onRegisterClicked);

    ui->statusLabel->hide();

    // Tab order
    setTabOrder(ui->usernameInput, ui->passwordInput);
    setTabOrder(ui->passwordInput, ui->confirmInput);
    setTabOrder(ui->confirmInput, ui->roleCombo);
    setTabOrder(ui->roleCombo, ui->registerBtn);
    setTabOrder(ui->registerBtn, ui->cancelBtn);

    ui->usernameInput->setFocus();
}

RegisterWidget::~RegisterWidget()
{
    delete ui;
}

void RegisterWidget::onRegisterClicked()
{
    QString user = ui->usernameInput->text().trimmed();
    QString pass = ui->passwordInput->text();
    QString conf = ui->confirmInput->text();
    QString role = ui->roleCombo->currentIndex() == 1 ? "administrator" : "operator";

    // Validate
    if (user.isEmpty()) {
        ui->statusLabel->setText("请输入账号");
        ui->statusLabel->show();
        ui->usernameInput->setFocus();
        return;
    }
    if (user.length() < 4) {
        ui->statusLabel->setText("账号至少 4 位");
        ui->statusLabel->show();
        return;
    }
    if (pass.isEmpty()) {
        ui->statusLabel->setText("请输入密码");
        ui->statusLabel->show();
        ui->passwordInput->setFocus();
        return;
    }
    if (pass.length() < 6) {
        ui->statusLabel->setText("密码至少 6 位");
        ui->statusLabel->show();
        return;
    }
    if (pass != conf) {
        ui->statusLabel->setText("两次密码输入不一致");
        ui->statusLabel->show();
        ui->confirmInput->clear();
        ui->confirmInput->setFocus();
        return;
    }

    if (doRegister(user, pass, role)) {
        QMessageBox::information(this, "注册成功",
            QString("账号「%1」注册成功，请登录").arg(user));
        emit registered(user);
        accept();
    } else {
        ui->statusLabel->setText("注册失败，账号可能已存在");
        ui->statusLabel->show();
    }
}

void RegisterWidget::onCancelClicked()
{
    reject();
}

bool RegisterWidget::doRegister(const QString &user, const QString &pass, const QString &role)
{
    DatabaseManager db("/opt/aicTrain/sMonitor_server/dat/server.db");
    if (!db.initDatabase()) return false;
    return db.registerUser(user, pass, role);
}
