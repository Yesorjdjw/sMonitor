/********************************************************************************
** Form generated from reading UI file 'logindlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINDLG_H
#define UI_LOGINDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_LoginDlg
{
public:
    QFrame *cameraFrame;
    QLabel *cameraLabel;
    QLabel *statusLabel;
    QLabel *nameLabel;
    QPushButton *registerBt;
    QPushButton *retryBt;

    void setupUi(QDialog *LoginDlg)
    {
        if (LoginDlg->objectName().isEmpty())
            LoginDlg->setObjectName(QString::fromUtf8("LoginDlg"));
        LoginDlg->resize(1024, 600);
        LoginDlg->setStyleSheet(QString::fromUtf8("\n"
"QDialog#LoginDlg {\n"
"    background-color: #1a1a2e;\n"
"}\n"
"QFrame#cameraFrame {\n"
"    background-color: #000000;\n"
"    border: 2px solid #16213e;\n"
"    border-radius: 8px;\n"
"}\n"
"QLabel {\n"
"    color: #ffffff;\n"
"    font-size: 16px;\n"
"    background: transparent;\n"
"}\n"
"QPushButton {\n"
"    color: #ffffff;\n"
"    font-size: 16px;\n"
"    font-weight: bold;\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    padding: 12px 24px;\n"
"}\n"
"QPushButton#registerBt {\n"
"    background: #0f3460;\n"
"    border: 2px solid #16213e;\n"
"}\n"
"QPushButton#registerBt:hover {\n"
"    background: #16213e;\n"
"}\n"
"QPushButton#retryBt {\n"
"    background: #e94560;\n"
"}\n"
"QPushButton#retryBt:hover {\n"
"    background: #ff6b6b;\n"
"}\n"
"   "));
        cameraFrame = new QFrame(LoginDlg);
        cameraFrame->setObjectName(QString::fromUtf8("cameraFrame"));
        cameraFrame->setGeometry(QRect(312, 30, 400, 300));
        cameraLabel = new QLabel(cameraFrame);
        cameraLabel->setObjectName(QString::fromUtf8("cameraLabel"));
        cameraLabel->setGeometry(QRect(0, 0, 400, 300));
        cameraLabel->setScaledContents(true);
        cameraLabel->setAlignment(Qt::AlignCenter);
        statusLabel = new QLabel(LoginDlg);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        statusLabel->setGeometry(QRect(312, 360, 400, 40));
        statusLabel->setAlignment(Qt::AlignCenter);
        statusLabel->setStyleSheet(QString::fromUtf8("font-size: 20px; font-weight: bold; color: #00ff88;"));
        nameLabel = new QLabel(LoginDlg);
        nameLabel->setObjectName(QString::fromUtf8("nameLabel"));
        nameLabel->setGeometry(QRect(312, 410, 400, 40));
        nameLabel->setAlignment(Qt::AlignCenter);
        nameLabel->setStyleSheet(QString::fromUtf8("font-size: 18px; color: #ffffff;"));
        registerBt = new QPushButton(LoginDlg);
        registerBt->setObjectName(QString::fromUtf8("registerBt"));
        registerBt->setGeometry(QRect(330, 480, 140, 44));
        retryBt = new QPushButton(LoginDlg);
        retryBt->setObjectName(QString::fromUtf8("retryBt"));
        retryBt->setGeometry(QRect(490, 480, 100, 44));

        retranslateUi(LoginDlg);

        QMetaObject::connectSlotsByName(LoginDlg);
    } // setupUi

    void retranslateUi(QDialog *LoginDlg)
    {
        LoginDlg->setWindowTitle(QCoreApplication::translate("LoginDlg", "\344\272\272\350\204\270\350\257\206\345\210\253\347\231\273\345\275\225", nullptr));
        cameraLabel->setText(QString());
        statusLabel->setText(QCoreApplication::translate("LoginDlg", "\346\255\243\345\234\250\346\211\253\346\217\217\344\272\272\350\204\270...", nullptr));
        nameLabel->setText(QString());
        registerBt->setText(QCoreApplication::translate("LoginDlg", "\346\263\250\345\206\214\346\226\260\347\224\250\346\210\267", nullptr));
        retryBt->setText(QCoreApplication::translate("LoginDlg", "\351\207\215\350\257\225", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginDlg: public Ui_LoginDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINDLG_H
