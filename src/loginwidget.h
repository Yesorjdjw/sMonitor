#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>

namespace Ui { class LoginWidget; }

class LoginWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();

signals:
    void loginSuccess(const QString &username);
    void registerRequested();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void onRegistered(const QString &username);
    void onFaceLoginClicked();

private:
    void drawBackground(QPainter &p);
    Ui::LoginWidget *ui;
};

#endif // LOGINWIDGET_H
