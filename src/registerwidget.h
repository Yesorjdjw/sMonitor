#ifndef REGISTERWIDGET_H
#define REGISTERWIDGET_H

#include <QDialog>

namespace Ui { class RegisterWidget; }

class RegisterWidget : public QDialog
{
    Q_OBJECT
public:
    explicit RegisterWidget(QWidget *parent = nullptr);
    ~RegisterWidget();

signals:
    void registered(const QString &username);

private slots:
    void onRegisterClicked();
    void onCancelClicked();

private:
    Ui::RegisterWidget *ui;
    bool doRegister(const QString &user, const QString &pass, const QString &role);
};

#endif // REGISTERWIDGET_H
