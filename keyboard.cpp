#include "keyboard.h"
#include <QApplication>

VirtualKeyboard::VirtualKeyboard(QWidget *parent)
    : QWidget(parent), m_target(nullptr)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint
                   | Qt::WindowStaysOnTopHint);
    setFixedSize(1024, 220);
    setStyleSheet("background-color: #2c3e50;");

    auto *mainLay = new QVBoxLayout(this);
    mainLay->setSpacing(4);
    mainLay->setContentsMargins(8, 6, 8, 6);

    // ── Row 1: 数字 ──
    auto *row1 = new QHBoxLayout;
    row1->setSpacing(4);
    for (const auto &ch : QString("1234567890.")) {
        QString k(ch);
        auto *btn = makeBtn(k, "#444");
        connect(btn, &QPushButton::clicked, [this, k]() { onKeyPressed(k); });
        row1->addWidget(btn);
    }
    // 退格
    auto *bs = makeBtn("←", "#c0392b", 90);
    connect(bs, &QPushButton::clicked, this, &VirtualKeyboard::onBackspace);
    row1->addWidget(bs);
    mainLay->addLayout(row1);

    // ── Row 2: Q-P ──
    auto *row2 = new QHBoxLayout;
    row2->setSpacing(4);
    for (const auto &ch : QString("QWERTYUIOP")) {
        QString k(ch);
        auto *btn = makeBtn(k, "#555");
        connect(btn, &QPushButton::clicked, [this, k]() { onKeyPressed(k); });
        row2->addWidget(btn);
    }
    mainLay->addLayout(row2);

    // ── Row 3: A-L + 清空 ──
    auto *row3 = new QHBoxLayout;
    row3->setSpacing(4);
    for (const auto &ch : QString("ASDFGHJKL")) {
        QString k(ch);
        auto *btn = makeBtn(k, "#555");
        connect(btn, &QPushButton::clicked, [this, k]() { onKeyPressed(k); });
        row3->addWidget(btn);
    }
    auto *clr = makeBtn("清空", "#e67e22", 80);
    connect(clr, &QPushButton::clicked, this, &VirtualKeyboard::onClear);
    row3->addWidget(clr);
    mainLay->addLayout(row3);

    // ── Row 4: Z-M + 空格 + 确定 ──
    auto *row4 = new QHBoxLayout;
    row4->setSpacing(4);
    for (const auto &ch : QString("ZXCVBNM")) {
        QString k(ch);
        auto *btn = makeBtn(k, "#555");
        connect(btn, &QPushButton::clicked, [this, k]() { onKeyPressed(k); });
        row4->addWidget(btn);
    }
    auto *sp = makeBtn("空格", "#666", 120);
    connect(sp, &QPushButton::clicked, [this]() { onKeyPressed(" "); });
    row4->addWidget(sp);
    auto *ok = makeBtn("确定", "#27ae60", 80);
    ok->setStyleSheet("QPushButton { background: #27ae60; color: white;"
                       " border-radius: 6px; font-size: 16px; font-weight: bold; }"
                       " QPushButton:pressed { background: #1e8449; }");
    connect(ok, &QPushButton::clicked, this, &VirtualKeyboard::onConfirm);
    row4->addWidget(ok);
    mainLay->addLayout(row4);
}

QPushButton *VirtualKeyboard::makeBtn(const QString &text, const QString &color, int w)
{
    auto *btn = new QPushButton(text, this);
    btn->setFixedSize(w, 46);
    btn->setStyleSheet(QString(
        "QPushButton { background: %1; color: white; border-radius: 5px;"
        " font-size: 17px; } QPushButton:pressed { background: #888; }").arg(color));
    btn->setFocusPolicy(Qt::NoFocus);
    return btn;
}

void VirtualKeyboard::setTarget(QLineEdit *edit) { m_target = edit; }

void VirtualKeyboard::showKeyboard()
{
    if (parentWidget()) {
        auto gp = parentWidget()->mapToGlobal(QPoint(0, 0));
        move(gp.x(), gp.y() + parentWidget()->height() - height());
    }
    raise();
    QWidget::show();
}

void VirtualKeyboard::hideKeyboard()
{
    QWidget::hide();
    if (m_target) m_target->clearFocus();
}

void VirtualKeyboard::onKeyPressed(const QString &text)
{
    if (m_target) m_target->insert(text);
}

void VirtualKeyboard::onBackspace()
{
    if (m_target) {
        QString t = m_target->text();
        if (!t.isEmpty()) m_target->setText(t.left(t.size() - 1));
    }
}

void VirtualKeyboard::onClear()
{
    if (m_target) m_target->clear();
}

void VirtualKeyboard::onConfirm()
{
    hideKeyboard();
    emit confirmed();
}
