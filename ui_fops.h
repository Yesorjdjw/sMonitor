/********************************************************************************
** Form generated from reading UI file 'fops.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FOPS_H
#define UI_FOPS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_fops
{
public:
    QWidget *titleBar;
    QLabel *titleText;
    QPushButton *tabAll;
    QPushButton *tabPhoto;
    QPushButton *tabVideo;
    QListWidget *fileList;
    QLabel *preview;
    QPushButton *playBtn;
    QPushButton *delBtn;
    QPushButton *refreshBtn;
    QPushButton *pushButton;
    QLabel *hintLabel;

    void setupUi(QWidget *fops)
    {
        if (fops->objectName().isEmpty())
            fops->setObjectName(QStringLiteral("fops"));
        fops->resize(1024, 600);
        fops->setStyleSheet(QLatin1String("\n"
"QWidget#fops {\n"
"    background-color: #d6e8f5;\n"
"    border-radius: 16px;\n"
"    font-family: \"Helvetica Neue\",\"PingFang SC\",\"Microsoft YaHei\",sans-serif;\n"
"}\n"
"QWidget#titleBar {\n"
"    background-color: #1a7bbd;\n"
"    border-top-left-radius: 14px;\n"
"    border-top-right-radius: 14px;\n"
"}\n"
"QLabel#titleText {\n"
"    color: #ffffff;\n"
"    font-size: 15px;\n"
"    font-weight: 600;\n"
"    background: transparent;\n"
"    letter-spacing: 1px;\n"
"}\n"
"QLabel#preview {\n"
"    background: #1e1e1e;\n"
"    border: 1px solid #ccc;\n"
"    border-radius: 8px;\n"
"    color: #999;\n"
"    font-size: 14px;\n"
"}\n"
"QListWidget#fileList {\n"
"    background: rgba(255,255,255,0.88);\n"
"    border: 1px solid #ddd;\n"
"    border-radius: 6px;\n"
"    font-size: 12px;\n"
"    outline: none;\n"
"}\n"
"QListWidget#fileList::item {\n"
"    padding: 8px 10px;\n"
"    border-bottom: 1px solid #eee;\n"
"}\n"
"QListWidget#fileList::item:selected {\n"
"    background: #d6e8f5;\n"
"    color: #1"
                        "a7bbd;\n"
"}\n"
"QPushButton#tabAll, QPushButton#tabPhoto, QPushButton#tabVideo {\n"
"    background: rgba(255,255,255,0.7);\n"
"    border: 1px solid #ccc;\n"
"    border-radius: 4px;\n"
"    font-size: 12px;\n"
"    color: #555;\n"
"}\n"
"QPushButton#tabAll:checked, QPushButton#tabPhoto:checked, QPushButton#tabVideo:checked {\n"
"    background: #1a7bbd;\n"
"    color: #fff;\n"
"    border: none;\n"
"}\n"
"QPushButton#playBtn {\n"
"    background: #1a7bbd;\n"
"    color: #fff;\n"
"    border: none;\n"
"    border-radius: 4px;\n"
"    font-size: 12px;\n"
"}\n"
"QPushButton#playBtn:disabled {\n"
"    background: #bbb;\n"
"}\n"
"QPushButton#delBtn {\n"
"    background: #e74c3c;\n"
"    color: #fff;\n"
"    border: none;\n"
"    border-radius: 4px;\n"
"    font-size: 12px;\n"
"}\n"
"QPushButton#refreshBtn {\n"
"    background: rgba(255,255,255,0.9);\n"
"    color: #1a7bbd;\n"
"    border: 1px solid #1a7bbd;\n"
"    border-radius: 4px;\n"
"    font-size: 12px;\n"
"}\n"
"QPushButton#pushButton {\n"
"    background"
                        ": rgba(255,255,255,0.9);\n"
"    border: 1px solid rgba(0,0,0,0.08);\n"
"    border-radius: 20px;\n"
"    color: #1a7bbd;\n"
"    font-size: 13px;\n"
"    font-weight: 500;\n"
"}\n"
"  "));
        titleBar = new QWidget(fops);
        titleBar->setObjectName(QStringLiteral("titleBar"));
        titleBar->setGeometry(QRect(0, 0, 1024, 52));
        titleText = new QLabel(titleBar);
        titleText->setObjectName(QStringLiteral("titleText"));
        titleText->setGeometry(QRect(24, 10, 200, 32));
        tabAll = new QPushButton(fops);
        tabAll->setObjectName(QStringLiteral("tabAll"));
        tabAll->setGeometry(QRect(24, 64, 72, 30));
        tabAll->setCheckable(true);
        tabAll->setChecked(true);
        tabPhoto = new QPushButton(fops);
        tabPhoto->setObjectName(QStringLiteral("tabPhoto"));
        tabPhoto->setGeometry(QRect(104, 64, 72, 30));
        tabPhoto->setCheckable(true);
        tabVideo = new QPushButton(fops);
        tabVideo->setObjectName(QStringLiteral("tabVideo"));
        tabVideo->setGeometry(QRect(184, 64, 72, 30));
        tabVideo->setCheckable(true);
        fileList = new QListWidget(fops);
        fileList->setObjectName(QStringLiteral("fileList"));
        fileList->setGeometry(QRect(24, 102, 260, 390));
        preview = new QLabel(fops);
        preview->setObjectName(QStringLiteral("preview"));
        preview->setGeometry(QRect(310, 70, 690, 410));
        preview->setAlignment(Qt::AlignCenter);
        playBtn = new QPushButton(fops);
        playBtn->setObjectName(QStringLiteral("playBtn"));
        playBtn->setGeometry(QRect(310, 490, 80, 28));
        playBtn->setEnabled(false);
        delBtn = new QPushButton(fops);
        delBtn->setObjectName(QStringLiteral("delBtn"));
        delBtn->setGeometry(QRect(24, 504, 100, 36));
        refreshBtn = new QPushButton(fops);
        refreshBtn->setObjectName(QStringLiteral("refreshBtn"));
        refreshBtn->setGeometry(QRect(132, 504, 100, 36));
        pushButton = new QPushButton(fops);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(810, 504, 170, 48));
        hintLabel = new QLabel(fops);
        hintLabel->setObjectName(QStringLiteral("hintLabel"));
        hintLabel->setGeometry(QRect(30, 80, 964, 360));

        retranslateUi(fops);

        QMetaObject::connectSlotsByName(fops);
    } // setupUi

    void retranslateUi(QWidget *fops)
    {
        fops->setWindowTitle(QApplication::translate("fops", "\346\226\207\344\273\266\347\256\241\347\220\206", 0));
        titleText->setText(QApplication::translate("fops", "\346\226\207\344\273\266\347\256\241\347\220\206", 0));
        tabAll->setText(QApplication::translate("fops", "\345\205\250\351\203\250", 0));
        tabPhoto->setText(QApplication::translate("fops", "\347\205\247\347\211\207", 0));
        tabVideo->setText(QApplication::translate("fops", "\350\247\206\351\242\221", 0));
        playBtn->setText(QApplication::translate("fops", "\346\222\255\346\224\276", 0));
        delBtn->setText(QApplication::translate("fops", "\345\210\240\351\231\244", 0));
        refreshBtn->setText(QApplication::translate("fops", "\345\210\267\346\226\260", 0));
        pushButton->setText(QApplication::translate("fops", "\350\277\224\345\233\236", 0));
        hintLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class fops: public Ui_fops {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FOPS_H
