/********************************************************************************
** Form generated from reading UI file 'menu.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MENU_H
#define UI_MENU_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_menu
{
public:
    QLabel *cap_Frame_lb;
    QLabel *timeWatermark_lb;
    QLabel *camLabel_lb;
    QLabel *noSignal_lb;
    QLabel *flashOverlay_lb;
    QStackedWidget *ctl_Panel_sw;
    QWidget *page;
    QPushButton *turn_up_bt;
    QPushButton *turn_left_bt;
    QPushButton *turn_reset_bt;
    QPushButton *turn_right_bt;
    QPushButton *turn_down_bt;
    QLabel *sepLine;
    QPushButton *get_photo_bt;
    QPushButton *get_vedio_bt;
    QLabel *recTime_lb;
    QWidget *page_2;
    QLabel *label_2;
    QPushButton *swt_Control_bt;
    QPushButton *exp_Check_bt;
    QPushButton *file_Manage_bt;
    QPushButton *mode_Setup_bt;

    void setupUi(QWidget *menu)
    {
        if (menu->objectName().isEmpty())
            menu->setObjectName(QStringLiteral("menu"));
        menu->resize(1024, 600);
        menu->setStyleSheet(QString::fromUtf8("\n"
"/* ===== \345\205\250\345\261\200 ===== */\n"
"QWidget#menu {\n"
"    font-family: \"Helvetica Neue\",\"PingFang SC\",\"Microsoft YaHei\",sans-serif;\n"
"}\n"
"\n"
"/* ===== \350\247\206\351\242\221\345\214\272 ===== */\n"
"QLabel#cap_Frame_lb {\n"
"    background-color: #000000;\n"
"    border: 1px solid rgba(255,255,255,0.15);\n"
"    border-radius: 18px;\n"
"}\n"
"\n"
"/* ===== \350\247\206\351\242\221\345\217\240\345\212\240\345\261\202 ===== */\n"
"QLabel#timeWatermark_lb {\n"
"    color: rgba(255,255,255,0.85);\n"
"    font-size: 12px;\n"
"    font-weight: 500;\n"
"    background: transparent;\n"
"}\n"
"QLabel#camLabel_lb {\n"
"    color: rgba(255,255,255,0.8);\n"
"    font-size: 11px;\n"
"    font-weight: 500;\n"
"    background: rgba(0,0,0,0.55);\n"
"    border-radius: 6px;\n"
"    padding: 2px 10px;\n"
"}\n"
"QLabel#noSignal_lb {\n"
"    color: rgba(255,255,255,0.5);\n"
"    font-size: 14px;\n"
"    font-weight: 400;\n"
"    background: transparent;\n"
"}\n"
"QLabel#flashOverlay_lb {\n"
"    back"
                        "ground: rgba(255,255,255,0.85);\n"
"    border-radius: 18px;\n"
"}\n"
"\n"
"/* ===== \346\216\247\345\210\266\351\235\242\346\235\277 - Apple\345\215\241\347\211\207\351\243\216\346\240\274 ===== */\n"
"QStackedWidget#ctl_Panel_sw {\n"
"    background: rgba(30,30,30,0.7);\n"
"    border: 1px solid rgba(255,255,255,0.1);\n"
"    border-radius: 20px;\n"
"}\n"
"\n"
"/* ===== PTZ\346\214\211\351\222\256 - \345\234\206\345\275\242\350\203\266\345\233\212 ===== */\n"
"QPushButton#turn_up_bt,\n"
"QPushButton#turn_down_bt,\n"
"QPushButton#turn_left_bt,\n"
"QPushButton#turn_right_bt {\n"
"    background: rgba(255,255,255,0.08);\n"
"    border: 1px solid rgba(255,255,255,0.15);\n"
"    border-radius: 14px;\n"
"}\n"
"QPushButton#turn_up_bt:hover,\n"
"QPushButton#turn_down_bt:hover,\n"
"QPushButton#turn_left_bt:hover,\n"
"QPushButton#turn_right_bt:hover {\n"
"    background: rgba(255,255,255,0.18);\n"
"    border-color: rgba(255,255,255,0.3);\n"
"}\n"
"\n"
"/* ===== \344\270\255\345\277\203\345\244\215\344\275\215 ===== *"
                        "/\n"
"QPushButton#turn_reset_bt {\n"
"    background: rgba(255,59,48,0.2);\n"
"    border: 1px solid rgba(255,59,48,0.4);\n"
"    border-radius: 14px;\n"
"}\n"
"QPushButton#turn_reset_bt:hover {\n"
"    background: rgba(255,59,48,0.35);\n"
"}\n"
"\n"
"/* ===== \346\213\215\347\205\247/\345\275\225\345\203\217 ===== */\n"
"QPushButton#get_photo_bt,\n"
"QPushButton#get_vedio_bt {\n"
"    background: rgba(255,255,255,0.08);\n"
"    border: 1px solid rgba(255,255,255,0.15);\n"
"    border-radius: 16px;\n"
"    font-size: 12px;\n"
"}\n"
"QPushButton#get_photo_bt:hover,\n"
"QPushButton#get_vedio_bt:hover {\n"
"    background: rgba(255,255,255,0.18);\n"
"    border-color: rgba(255,255,255,0.3);\n"
"}\n"
"\n"
"/* ===== \345\275\225\345\203\217\346\227\266\351\225\277 ===== */\n"
"QLabel#recTime_lb {\n"
"    color: rgba(255,255,255,0.7);\n"
"    font-size: 11px;\n"
"    font-weight: 400;\n"
"    background: transparent;\n"
"}\n"
"\n"
"/* ===== \345\272\225\351\203\250\345\257\274\350\210\252 - Apple\345\274\217\347\262"
                        "\227\345\234\206\350\247\222\345\215\241\347\211\207 ===== */\n"
"QPushButton#swt_Control_bt,\n"
"QPushButton#exp_Check_bt,\n"
"QPushButton#file_Manage_bt,\n"
"QPushButton#mode_Setup_bt {\n"
"    background: rgba(28,28,30,0.75);\n"
"    border: 1px solid rgba(255,255,255,0.1);\n"
"    border-radius: 22px;\n"
"    color: #ffffff;\n"
"    font-size: 14px;\n"
"    font-weight: 500;\n"
"    padding-top: 50px;\n"
"}\n"
"QPushButton#swt_Control_bt:hover,\n"
"QPushButton#exp_Check_bt:hover,\n"
"QPushButton#file_Manage_bt:hover,\n"
"QPushButton#mode_Setup_bt:hover {\n"
"    background: rgba(44,44,46,0.85);\n"
"    border-color: rgba(255,255,255,0.2);\n"
"}\n"
"   "));
        cap_Frame_lb = new QLabel(menu);
        cap_Frame_lb->setObjectName(QStringLiteral("cap_Frame_lb"));
        cap_Frame_lb->setGeometry(QRect(20, 68, 654, 362));
        timeWatermark_lb = new QLabel(menu);
        timeWatermark_lb->setObjectName(QStringLiteral("timeWatermark_lb"));
        timeWatermark_lb->setGeometry(QRect(344, 76, 330, 28));
        timeWatermark_lb->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        camLabel_lb = new QLabel(menu);
        camLabel_lb->setObjectName(QStringLiteral("camLabel_lb"));
        camLabel_lb->setGeometry(QRect(32, 396, 64, 24));
        camLabel_lb->setAlignment(Qt::AlignCenter);
        noSignal_lb = new QLabel(menu);
        noSignal_lb->setObjectName(QStringLiteral("noSignal_lb"));
        noSignal_lb->setGeometry(QRect(20, 68, 654, 362));
        noSignal_lb->setAlignment(Qt::AlignCenter);
        flashOverlay_lb = new QLabel(menu);
        flashOverlay_lb->setObjectName(QStringLiteral("flashOverlay_lb"));
        flashOverlay_lb->setGeometry(QRect(20, 68, 654, 362));
        ctl_Panel_sw = new QStackedWidget(menu);
        ctl_Panel_sw->setObjectName(QStringLiteral("ctl_Panel_sw"));
        ctl_Panel_sw->setGeometry(QRect(700, 70, 314, 362));
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        turn_up_bt = new QPushButton(page);
        turn_up_bt->setObjectName(QStringLiteral("turn_up_bt"));
        turn_up_bt->setGeometry(QRect(120, 22, 74, 54));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/icon_up.svg"), QSize(), QIcon::Normal, QIcon::Off);
        turn_up_bt->setIcon(icon);
        turn_up_bt->setIconSize(QSize(20, 20));
        turn_left_bt = new QPushButton(page);
        turn_left_bt->setObjectName(QStringLiteral("turn_left_bt"));
        turn_left_bt->setGeometry(QRect(36, 94, 74, 54));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/icon_left.svg"), QSize(), QIcon::Normal, QIcon::Off);
        turn_left_bt->setIcon(icon1);
        turn_left_bt->setIconSize(QSize(20, 20));
        turn_reset_bt = new QPushButton(page);
        turn_reset_bt->setObjectName(QStringLiteral("turn_reset_bt"));
        turn_reset_bt->setGeometry(QRect(120, 94, 74, 54));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/icon_center.svg"), QSize(), QIcon::Normal, QIcon::Off);
        turn_reset_bt->setIcon(icon2);
        turn_reset_bt->setIconSize(QSize(20, 20));
        turn_right_bt = new QPushButton(page);
        turn_right_bt->setObjectName(QStringLiteral("turn_right_bt"));
        turn_right_bt->setGeometry(QRect(204, 94, 74, 54));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/images/icon_right.svg"), QSize(), QIcon::Normal, QIcon::Off);
        turn_right_bt->setIcon(icon3);
        turn_right_bt->setIconSize(QSize(20, 20));
        turn_down_bt = new QPushButton(page);
        turn_down_bt->setObjectName(QStringLiteral("turn_down_bt"));
        turn_down_bt->setGeometry(QRect(120, 166, 74, 54));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/images/icon_down.svg"), QSize(), QIcon::Normal, QIcon::Off);
        turn_down_bt->setIcon(icon4);
        turn_down_bt->setIconSize(QSize(20, 20));
        sepLine = new QLabel(page);
        sepLine->setObjectName(QStringLiteral("sepLine"));
        sepLine->setGeometry(QRect(24, 240, 266, 1));
        sepLine->setStyleSheet(QStringLiteral("background: rgba(255,255,255,0.08);"));
        get_photo_bt = new QPushButton(page);
        get_photo_bt->setObjectName(QStringLiteral("get_photo_bt"));
        get_photo_bt->setGeometry(QRect(46, 264, 94, 72));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/images/icon_camera.svg"), QSize(), QIcon::Normal, QIcon::Off);
        get_photo_bt->setIcon(icon5);
        get_photo_bt->setIconSize(QSize(24, 24));
        get_vedio_bt = new QPushButton(page);
        get_vedio_bt->setObjectName(QStringLiteral("get_vedio_bt"));
        get_vedio_bt->setGeometry(QRect(164, 264, 110, 72));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/images/icon_video.svg"), QSize(), QIcon::Normal, QIcon::Off);
        get_vedio_bt->setIcon(icon6);
        get_vedio_bt->setIconSize(QSize(24, 24));
        recTime_lb = new QLabel(page);
        recTime_lb->setObjectName(QStringLiteral("recTime_lb"));
        recTime_lb->setGeometry(QRect(174, 338, 94, 22));
        recTime_lb->setAlignment(Qt::AlignCenter);
        ctl_Panel_sw->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        label_2 = new QLabel(page_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(82, 140, 150, 50));
        label_2->setStyleSheet(QStringLiteral("color: rgba(255,255,255,0.6); font-size: 14px; font-weight: 400; background: transparent;"));
        label_2->setAlignment(Qt::AlignCenter);
        ctl_Panel_sw->addWidget(page_2);
        swt_Control_bt = new QPushButton(menu);
        swt_Control_bt->setObjectName(QStringLiteral("swt_Control_bt"));
        swt_Control_bt->setGeometry(QRect(20, 450, 231, 128));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/images/icon_switch.svg"), QSize(), QIcon::Normal, QIcon::Off);
        swt_Control_bt->setIcon(icon7);
        swt_Control_bt->setIconSize(QSize(24, 24));
        exp_Check_bt = new QPushButton(menu);
        exp_Check_bt->setObjectName(QStringLiteral("exp_Check_bt"));
        exp_Check_bt->setGeometry(QRect(267, 450, 231, 128));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/images/icon_alarm.svg"), QSize(), QIcon::Normal, QIcon::Off);
        exp_Check_bt->setIcon(icon8);
        exp_Check_bt->setIconSize(QSize(24, 24));
        file_Manage_bt = new QPushButton(menu);
        file_Manage_bt->setObjectName(QStringLiteral("file_Manage_bt"));
        file_Manage_bt->setGeometry(QRect(514, 450, 231, 128));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/images/icon_folder.svg"), QSize(), QIcon::Normal, QIcon::Off);
        file_Manage_bt->setIcon(icon9);
        file_Manage_bt->setIconSize(QSize(24, 24));
        mode_Setup_bt = new QPushButton(menu);
        mode_Setup_bt->setObjectName(QStringLiteral("mode_Setup_bt"));
        mode_Setup_bt->setGeometry(QRect(760, 450, 243, 128));
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/images/icon_settings.svg"), QSize(), QIcon::Normal, QIcon::Off);
        mode_Setup_bt->setIcon(icon10);
        mode_Setup_bt->setIconSize(QSize(24, 24));

        retranslateUi(menu);

        ctl_Panel_sw->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(menu);
    } // setupUi

    void retranslateUi(QWidget *menu)
    {
        menu->setWindowTitle(QApplication::translate("menu", "\345\260\217\350\232\201\346\231\272\350\203\275\345\256\211\351\230\262\347\263\273\347\273\237", 0));
        cap_Frame_lb->setText(QString());
        timeWatermark_lb->setText(QApplication::translate("menu", "2026-06-29 \346\230\237\346\234\237\344\270\200 13:53:44", 0));
        camLabel_lb->setText(QApplication::translate("menu", "CAM 01", 0));
        noSignal_lb->setText(QApplication::translate("menu", "\346\232\202\346\227\240\350\247\206\351\242\221\344\277\241\345\217\267", 0));
        flashOverlay_lb->setText(QString());
        turn_up_bt->setText(QString());
        turn_left_bt->setText(QString());
        turn_reset_bt->setText(QString());
        turn_right_bt->setText(QString());
        turn_down_bt->setText(QString());
        sepLine->setText(QString());
        get_photo_bt->setText(QString());
        get_vedio_bt->setText(QString());
        recTime_lb->setText(QApplication::translate("menu", "00:00", 0));
        label_2->setText(QApplication::translate("menu", "\346\211\213\345\212\250\346\216\247\345\210\266", 0));
        swt_Control_bt->setText(QApplication::translate("menu", "\345\210\207\346\215\242\350\207\252\345\212\250", 0));
        exp_Check_bt->setText(QApplication::translate("menu", "\345\274\202\345\270\270\346\237\245\347\234\213", 0));
        file_Manage_bt->setText(QApplication::translate("menu", "\346\226\207\344\273\266\347\256\241\347\220\206", 0));
        mode_Setup_bt->setText(QApplication::translate("menu", "\346\250\241\345\274\217\350\256\276\347\275\256", 0));
    } // retranslateUi

};

namespace Ui {
    class menu: public Ui_menu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MENU_H
