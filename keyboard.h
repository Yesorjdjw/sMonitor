#ifndef VIRTUALKEYBOARD_H
#define VIRTUALKEYBOARD_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

/**
 * 轻量虚拟键盘 — 专为嵌入式触摸屏设计
 * 点击输入框自动弹出，按"确定"隐藏
 */
class VirtualKeyboard : public QWidget
{
    Q_OBJECT

public:
    explicit VirtualKeyboard(QWidget *parent = nullptr);
    void setTarget(QLineEdit *edit);

signals:
    /** 用户按了"确定"（隐藏键盘 + 通知调用方） */
    void confirmed();

public slots:
    void showKeyboard();
    void hideKeyboard();

private slots:
    void onKeyPressed(const QString &text);
    void onBackspace();
    void onClear();
    void onConfirm();

private:
    QLineEdit *m_target;
    QPushButton *makeBtn(const QString &text, const QString &color, int w = 64);
};

#endif // VIRTUALKEYBOARD_H
