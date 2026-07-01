/********************************************************************************
** Form generated from reading UI file 'unusual.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UNUSUAL_H
#define UI_UNUSUAL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_unusual
{
public:
    QFrame *leftCard;
    QWidget *leftTitleBar;
    QLabel *docIcon;
    QLabel *leftTitleText;
    QLabel *badgeTotal;
    QLabel *ind1;
    QLabel *catIcon1;
    QPushButton *catBtn1;
    QLabel *badge1;
    QLabel *ind2;
    QLabel *catIcon2;
    QPushButton *catBtn2;
    QLabel *badge2;
    QLabel *ind3;
    QLabel *catIcon3;
    QPushButton *catBtn3;
    QLabel *badge3;
    QLabel *ind4;
    QLabel *catIcon4;
    QPushButton *catBtn4;
    QLabel *badge4;
    QLabel *ind5;
    QLabel *catIcon5;
    QPushButton *catBtn5;
    QLabel *badge5;
    QFrame *rightCard;
    QWidget *rightTitleBar;
    QLabel *rightIcon;
    QLabel *rightTitleText;
    QLabel *countText;
    QTableWidget *eventTable;
    QPushButton *backBt;

    void setupUi(QWidget *unusual)
    {
        if (unusual->objectName().isEmpty())
            unusual->setObjectName(QStringLiteral("unusual"));
        unusual->resize(1024, 600);
        unusual->setStyleSheet(QString::fromUtf8("\n"
"QWidget#unusual {\n"
"    background-color: #d6e8f5;\n"
"    border-radius: 16px;\n"
"    font-family: \"Helvetica Neue\",\"PingFang SC\",\"Microsoft YaHei\",sans-serif;\n"
"}\n"
"QFrame#leftCard, QFrame#rightCard {\n"
"    background: #ffffff;\n"
"}\n"
"QWidget#leftTitleBar, QWidget#rightTitleBar {\n"
"    background-color: #1a7bbd;\n"
"}\n"
"QLabel#leftTitleText, QLabel#rightTitleText {\n"
"    color: #ffffff;\n"
"    font-size: 14px;\n"
"    font-weight: 600;\n"
"    background: transparent;\n"
"    letter-spacing: 1px;\n"
"}\n"
"QLabel#countText {\n"
"    color: rgba(255,255,255,0.8);\n"
"    font-size: 12px;\n"
"    font-weight: 400;\n"
"    background: transparent;\n"
"}\n"
"QLabel#badgeTotal {\n"
"    background: #ff3b30;\n"
"    color: #ffffff;\n"
"    font-size: 12px;\n"
"    font-weight: bold;\n"
"    border-radius: 12px;\n"
"    padding: 2px 8px;\n"
"}\n"
"/* \345\210\206\347\261\273\346\214\211\351\222\256 - \351\273\230\350\256\244\346\200\201 */\n"
"QPushButton#catBtn1, QPushButton#catBtn2, "
                        "QPushButton#catBtn3,\n"
"QPushButton#catBtn4, QPushButton#catBtn5 {\n"
"    background: transparent;\n"
"    border: none;\n"
"    border-radius: 0px;\n"
"    text-align: left;\n"
"    font-size: 13px;\n"
"    font-weight: 600;\n"
"    color: #333333;\n"
"    padding-left: 52px;\n"
"    padding-top: 6px;\n"
"}\n"
"QPushButton#catBtn1:hover, QPushButton#catBtn2:hover, QPushButton#catBtn3:hover,\n"
"QPushButton#catBtn4:hover, QPushButton#catBtn5:hover {\n"
"    background: #eef4fa;\n"
"}\n"
"    color: #888888;\n"
"    font-size: 11px;\n"
"    background: transparent;\n"
"}\n"
"/* \345\210\206\347\261\273\345\276\275\346\240\207 - \351\242\234\350\211\262\345\234\250\344\273\243\347\240\201\344\270\255\350\256\276\347\275\256\357\274\214\350\277\231\351\207\214\345\217\252\345\256\232\344\271\211\346\240\267\345\274\217\345\237\272\347\261\273 */\n"
"QLabel#badge1, QLabel#badge2 {\n"
"    background: #ff3b30;\n"
"    color: #ffffff;\n"
"    font-size: 11px;\n"
"    font-weight: bold;\n"
"    border-radius: 10px;"
                        "\n"
"    padding: 1px 7px;\n"
"}\n"
"QLabel#badge3, QLabel#badge4 {\n"
"    background: #ff9500;\n"
"    color: #ffffff;\n"
"    font-size: 11px;\n"
"    font-weight: bold;\n"
"    border-radius: 10px;\n"
"    padding: 1px 7px;\n"
"}\n"
"QLabel#badge5 {\n"
"    background: #007aff;\n"
"    color: #ffffff;\n"
"    font-size: 11px;\n"
"    font-weight: bold;\n"
"    border-radius: 10px;\n"
"    padding: 1px 7px;\n"
"}\n"
"/* \346\214\207\347\244\272\350\211\262\346\235\241 */\n"
"QLabel#ind1, QLabel#ind2 { background: #ff3b30; }\n"
"QLabel#ind3, QLabel#ind4 { background: #ff9500; }\n"
"QLabel#ind5 { background: #007aff; }\n"
"/* \350\241\250\346\240\274 */\n"
"QHeaderView::section {\n"
"    background: #f2f4f7;\n"
"    color: #555555;\n"
"    font-size: 12px;\n"
"    font-weight: 600;\n"
"    border: none;\n"
"    border-bottom: 1px solid #e0e0e0;\n"
"    padding: 10px 8px;\n"
"}\n"
"QTableWidget#eventTable {\n"
"    background: #ffffff;\n"
"    border: none;\n"
"    font-size: 12px;\n"
"    color: #333333;\n"
""
                        "    gridline-color: #f0f0f0;\n"
"    selection-background-color: #e8f0fe;\n"
"}\n"
"QTableWidget#eventTable::item {\n"
"    padding: 8px;\n"
"    border-bottom: 1px solid #f0f0f0;\n"
"}\n"
"/* \350\277\224\345\233\236\346\214\211\351\222\256 - \347\256\200\347\272\246\347\272\277\346\235\241\351\243\216\346\240\274 */\n"
"QPushButton#backBt {\n"
"    background: transparent;\n"
"    border: 1px solid rgba(26,123,189,0.25);\n"
"    border-radius: 14px;\n"
"    color: #1a7bbd;\n"
"    font-size: 12px;\n"
"    font-weight: 500;\n"
"    text-align: center;\n"
"}\n"
"QPushButton#backBt:hover {\n"
"    background: rgba(26,123,189,0.06);\n"
"    border-color: rgba(26,123,189,0.5);\n"
"}\n"
"   "));
        leftCard = new QFrame(unusual);
        leftCard->setObjectName(QStringLiteral("leftCard"));
        leftCard->setGeometry(QRect(15, 15, 238, 542));
        leftTitleBar = new QWidget(leftCard);
        leftTitleBar->setObjectName(QStringLiteral("leftTitleBar"));
        leftTitleBar->setGeometry(QRect(0, 0, 238, 44));
        docIcon = new QLabel(leftTitleBar);
        docIcon->setObjectName(QStringLiteral("docIcon"));
        docIcon->setGeometry(QRect(12, 10, 24, 24));
        docIcon->setPixmap(QPixmap(QString::fromUtf8(":/images/icon_doc.svg")));
        docIcon->setScaledContents(true);
        leftTitleText = new QLabel(leftTitleBar);
        leftTitleText->setObjectName(QStringLiteral("leftTitleText"));
        leftTitleText->setGeometry(QRect(42, 6, 140, 32));
        badgeTotal = new QLabel(leftTitleBar);
        badgeTotal->setObjectName(QStringLiteral("badgeTotal"));
        badgeTotal->setGeometry(QRect(194, 10, 30, 24));
        badgeTotal->setAlignment(Qt::AlignCenter);
        ind1 = new QLabel(leftCard);
        ind1->setObjectName(QStringLiteral("ind1"));
        ind1->setGeometry(QRect(0, 68, 4, 52));
        catIcon1 = new QLabel(leftCard);
        catIcon1->setObjectName(QStringLiteral("catIcon1"));
        catIcon1->setGeometry(QRect(16, 82, 28, 28));
        catIcon1->setPixmap(QPixmap(QString::fromUtf8(":/images/icon_alert.svg")));
        catIcon1->setScaledContents(true);
        catBtn1 = new QPushButton(leftCard);
        catBtn1->setObjectName(QStringLiteral("catBtn1"));
        catBtn1->setGeometry(QRect(0, 68, 238, 52));
        badge1 = new QLabel(leftCard);
        badge1->setObjectName(QStringLiteral("badge1"));
        badge1->setGeometry(QRect(192, 84, 28, 22));
        badge1->setAlignment(Qt::AlignCenter);
        ind2 = new QLabel(leftCard);
        ind2->setObjectName(QStringLiteral("ind2"));
        ind2->setGeometry(QRect(0, 124, 4, 52));
        catIcon2 = new QLabel(leftCard);
        catIcon2->setObjectName(QStringLiteral("catIcon2"));
        catIcon2->setGeometry(QRect(16, 138, 28, 28));
        catIcon2->setPixmap(QPixmap(QString::fromUtf8(":/images/icon_alert.svg")));
        catIcon2->setScaledContents(true);
        catBtn2 = new QPushButton(leftCard);
        catBtn2->setObjectName(QStringLiteral("catBtn2"));
        catBtn2->setGeometry(QRect(0, 124, 238, 52));
        badge2 = new QLabel(leftCard);
        badge2->setObjectName(QStringLiteral("badge2"));
        badge2->setGeometry(QRect(192, 140, 28, 22));
        badge2->setAlignment(Qt::AlignCenter);
        ind3 = new QLabel(leftCard);
        ind3->setObjectName(QStringLiteral("ind3"));
        ind3->setGeometry(QRect(0, 180, 4, 52));
        catIcon3 = new QLabel(leftCard);
        catIcon3->setObjectName(QStringLiteral("catIcon3"));
        catIcon3->setGeometry(QRect(16, 194, 28, 28));
        catIcon3->setPixmap(QPixmap(QString::fromUtf8(":/images/icon_warning.svg")));
        catIcon3->setScaledContents(true);
        catBtn3 = new QPushButton(leftCard);
        catBtn3->setObjectName(QStringLiteral("catBtn3"));
        catBtn3->setGeometry(QRect(0, 180, 238, 52));
        badge3 = new QLabel(leftCard);
        badge3->setObjectName(QStringLiteral("badge3"));
        badge3->setGeometry(QRect(192, 196, 28, 22));
        badge3->setAlignment(Qt::AlignCenter);
        ind4 = new QLabel(leftCard);
        ind4->setObjectName(QStringLiteral("ind4"));
        ind4->setGeometry(QRect(0, 236, 4, 52));
        catIcon4 = new QLabel(leftCard);
        catIcon4->setObjectName(QStringLiteral("catIcon4"));
        catIcon4->setGeometry(QRect(16, 250, 28, 28));
        catIcon4->setPixmap(QPixmap(QString::fromUtf8(":/images/icon_warning.svg")));
        catIcon4->setScaledContents(true);
        catBtn4 = new QPushButton(leftCard);
        catBtn4->setObjectName(QStringLiteral("catBtn4"));
        catBtn4->setGeometry(QRect(0, 236, 238, 52));
        badge4 = new QLabel(leftCard);
        badge4->setObjectName(QStringLiteral("badge4"));
        badge4->setGeometry(QRect(192, 252, 28, 22));
        badge4->setAlignment(Qt::AlignCenter);
        ind5 = new QLabel(leftCard);
        ind5->setObjectName(QStringLiteral("ind5"));
        ind5->setGeometry(QRect(0, 292, 4, 52));
        catIcon5 = new QLabel(leftCard);
        catIcon5->setObjectName(QStringLiteral("catIcon5"));
        catIcon5->setGeometry(QRect(16, 306, 28, 28));
        catIcon5->setPixmap(QPixmap(QString::fromUtf8(":/images/icon_info.svg")));
        catIcon5->setScaledContents(true);
        catBtn5 = new QPushButton(leftCard);
        catBtn5->setObjectName(QStringLiteral("catBtn5"));
        catBtn5->setGeometry(QRect(0, 292, 238, 52));
        badge5 = new QLabel(leftCard);
        badge5->setObjectName(QStringLiteral("badge5"));
        badge5->setGeometry(QRect(192, 308, 28, 22));
        badge5->setAlignment(Qt::AlignCenter);
        rightCard = new QFrame(unusual);
        rightCard->setObjectName(QStringLiteral("rightCard"));
        rightCard->setGeometry(QRect(268, 15, 741, 542));
        rightTitleBar = new QWidget(rightCard);
        rightTitleBar->setObjectName(QStringLiteral("rightTitleBar"));
        rightTitleBar->setGeometry(QRect(0, 0, 741, 44));
        rightIcon = new QLabel(rightTitleBar);
        rightIcon->setObjectName(QStringLiteral("rightIcon"));
        rightIcon->setGeometry(QRect(14, 10, 24, 24));
        rightIcon->setPixmap(QPixmap(QString::fromUtf8(":/images/icon_warning.svg")));
        rightIcon->setScaledContents(true);
        rightTitleText = new QLabel(rightTitleBar);
        rightTitleText->setObjectName(QStringLiteral("rightTitleText"));
        rightTitleText->setGeometry(QRect(44, 6, 240, 32));
        countText = new QLabel(rightTitleBar);
        countText->setObjectName(QStringLiteral("countText"));
        countText->setGeometry(QRect(660, 10, 70, 24));
        countText->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        eventTable = new QTableWidget(rightCard);
        if (eventTable->columnCount() < 5)
            eventTable->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        eventTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        eventTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        eventTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        eventTable->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        eventTable->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        eventTable->setObjectName(QStringLiteral("eventTable"));
        eventTable->setGeometry(QRect(0, 44, 741, 498));
        eventTable->setColumnCount(5);
        eventTable->horizontalHeader()->setDefaultSectionSize(140);
        eventTable->horizontalHeader()->setMinimumSectionSize(40);
        eventTable->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        eventTable->horizontalHeader()->setStretchLastSection(false);
        backBt = new QPushButton(unusual);
        backBt->setObjectName(QStringLiteral("backBt"));
        backBt->setGeometry(QRect(53, 490, 151, 34));

        retranslateUi(unusual);

        QMetaObject::connectSlotsByName(unusual);
    } // setupUi

    void retranslateUi(QWidget *unusual)
    {
        unusual->setWindowTitle(QApplication::translate("unusual", "\345\274\202\345\270\270\344\272\213\344\273\266\345\244\204\347\220\206", 0));
        docIcon->setText(QString());
        leftTitleText->setText(QApplication::translate("unusual", "\345\276\205\345\244\204\347\220\206\345\274\202\345\270\270\344\272\213\344\273\266", 0));
        badgeTotal->setText(QString());
        ind1->setText(QString());
        catIcon1->setText(QString());
        catBtn1->setText(QApplication::translate("unusual", "\345\214\272\345\237\237\345\205\245\344\276\265\346\243\200\346\265\213", 0));
        badge1->setText(QString());
        ind2->setText(QString());
        catIcon2->setText(QString());
        catBtn2->setText(QApplication::translate("unusual", "\344\272\272\350\204\270\350\257\206\345\210\253\345\274\202\345\270\270", 0));
        badge2->setText(QString());
        ind3->setText(QString());
        catIcon3->setText(QString());
        catBtn3->setText(QApplication::translate("unusual", "\347\224\273\351\235\242\351\201\256\346\214\241\345\221\212\350\255\246", 0));
        badge3->setText(QString());
        ind4->setText(QString());
        catIcon4->setText(QString());
        catBtn4->setText(QApplication::translate("unusual", "\347\247\273\345\212\250\344\276\246\346\265\213\345\221\212\350\255\246", 0));
        badge4->setText(QString());
        ind5->setText(QString());
        catIcon5->setText(QString());
        catBtn5->setText(QApplication::translate("unusual", "\345\255\230\345\202\250\347\251\272\351\227\264\344\270\215\350\266\263", 0));
        badge5->setText(QString());
        rightIcon->setText(QString());
        rightTitleText->setText(QString());
        countText->setText(QString());
        QTableWidgetItem *___qtablewidgetitem = eventTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("unusual", "\346\210\252\345\233\276", 0));
        QTableWidgetItem *___qtablewidgetitem1 = eventTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("unusual", "\344\272\213\344\273\266\347\261\273\345\236\213", 0));
        QTableWidgetItem *___qtablewidgetitem2 = eventTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("unusual", "\347\255\211\347\272\247", 0));
        QTableWidgetItem *___qtablewidgetitem3 = eventTable->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("unusual", "\350\247\246\345\217\221\346\227\266\351\227\264", 0));
        QTableWidgetItem *___qtablewidgetitem4 = eventTable->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("unusual", "\347\212\266\346\200\201", 0));
        backBt->setText(QApplication::translate("unusual", "  < \350\277\224\345\233\236\344\270\273\351\241\265", 0));
    } // retranslateUi

};

namespace Ui {
    class unusual: public Ui_unusual {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UNUSUAL_H
