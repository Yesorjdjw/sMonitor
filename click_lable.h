#ifndef CLICK_LABLE_H
#define CLICK_LABLE_H

#include <QLabel>

class click_lable : public QLabel
{
    Q_OBJECT

public:
    explicit click_lable(QWidget *parent = 0, Qt::WindowFlags f = 0);
    explicit click_lable(const QString &text, QWidget *parent = 0, Qt::WindowFlags f = 0);

signals:
    void clicked();
    void doubleclicked();

protected:
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
};

#endif // CLICK_LABLE_H
