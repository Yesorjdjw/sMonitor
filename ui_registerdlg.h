/********************************************************************************
** Form generated from reading UI file 'registerdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTERDLG_H
#define UI_REGISTERDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_RegisterDlg
{
public:
    QFrame *cameraFrame;
    QLabel *cameraLabel;
    QLabel *label;
    QLineEdit *nameEdit;
    QPushButton *confirmBt;
    QLabel *progressLabel;
    QLabel *statusLabel;
    QPushButton *captureBt;
    QPushButton *doneBt;
    QPushButton *cancelBt;

    void setupUi(QDialog *RegisterDlg)
    {
        if (RegisterDlg->objectName().isEmpty())
            RegisterDlg->setObjectName(QStringLiteral("RegisterDlg"));
        RegisterDlg->resize(1024, 600);
        RegisterDlg->setStyleSheet(QLatin1String("\n"
"QDialog#RegisterDlg {\n"
"    background-color: #1a1a2e;\n"
"}\n"
"QFrame#cameraFrame {\n"
"    background-color: #000000;\n"
"    border: 2px solid #16213e;\n"
"    border-radius: 8px;\n"
"}\n"
"QLabel#label {\n"
"    color: #ffffff;\n"
"    font-size: 16px;\n"
"    background: transparent;\n"
"}\n"
"QLabel#statusLabel {\n"
"    color: #00ff88;\n"
"    font-size: 18px;\n"
"    font-weight: bold;\n"
"    background: transparent;\n"
"}\n"
"QLabel#progressLabel {\n"
"    color: #ffffff;\n"
"    font-size: 16px;\n"
"    background: transparent;\n"
"}\n"
"QLineEdit#nameEdit {\n"
"    background: #16213e;\n"
"    color: #ffffff;\n"
"    font-size: 18px;\n"
"    border: 2px solid #0f3460;\n"
"    border-radius: 8px;\n"
"    padding: 8px 16px;\n"
"}\n"
"QPushButton#captureBt {\n"
"    background: #e94560;\n"
"    color: #ffffff;\n"
"    font-size: 16px;\n"
"    font-weight: bold;\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    padding: 12px 24px;\n"
"}\n"
"QPushButton#captureBt:hover {\n"
"    backgrou"
                        "nd: #ff6b6b;\n"
"}\n"
"QPushButton#captureBt:disabled {\n"
"    background: #555555;\n"
"    color: #888888;\n"
"}\n"
"QPushButton#doneBt {\n"
"    background: #0f3460;\n"
"    color: #ffffff;\n"
"    font-size: 16px;\n"
"    font-weight: bold;\n"
"    border: 2px solid #16213e;\n"
"    border-radius: 8px;\n"
"    padding: 12px 24px;\n"
"}\n"
"QPushButton#doneBt:disabled {\n"
"    background: #555555;\n"
"    color: #888888;\n"
"}\n"
"QPushButton#cancelBt {\n"
"    background: #533483;\n"
"    color: #ffffff;\n"
"    font-size: 16px;\n"
"    border-radius: 8px;\n"
"    padding: 12px 24px;\n"
"}\n"
"QPushButton#confirmBt {\n"
"    background: #0f3460;\n"
"    color: #ffffff;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    padding: 8px 16px;\n"
"}\n"
"QPushButton#confirmBt:hover {\n"
"    background: #16213e;\n"
"}\n"
"QPushButton#confirmBt:disabled {\n"
"    background: #555555;\n"
"    color: #888888;\n"
"}\n"
"   "));
        cameraFrame = new QFrame(RegisterDlg);
        cameraFrame->setObjectName(QStringLiteral("cameraFrame"));
        cameraFrame->setGeometry(QRect(312, 30, 400, 300));
        cameraLabel = new QLabel(cameraFrame);
        cameraLabel->setObjectName(QStringLiteral("cameraLabel"));
        cameraLabel->setGeometry(QRect(0, 0, 400, 300));
        cameraLabel->setScaledContents(true);
        cameraLabel->setAlignment(Qt::AlignCenter);
        label = new QLabel(RegisterDlg);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(312, 350, 100, 30));
        nameEdit = new QLineEdit(RegisterDlg);
        nameEdit->setObjectName(QStringLiteral("nameEdit"));
        nameEdit->setGeometry(QRect(420, 346, 200, 36));
        confirmBt = new QPushButton(RegisterDlg);
        confirmBt->setObjectName(QStringLiteral("confirmBt"));
        confirmBt->setGeometry(QRect(628, 346, 80, 36));
        progressLabel = new QLabel(RegisterDlg);
        progressLabel->setObjectName(QStringLiteral("progressLabel"));
        progressLabel->setGeometry(QRect(312, 400, 400, 30));
        progressLabel->setAlignment(Qt::AlignCenter);
        statusLabel = new QLabel(RegisterDlg);
        statusLabel->setObjectName(QStringLiteral("statusLabel"));
        statusLabel->setGeometry(QRect(312, 430, 400, 30));
        statusLabel->setAlignment(Qt::AlignCenter);
        captureBt = new QPushButton(RegisterDlg);
        captureBt->setObjectName(QStringLiteral("captureBt"));
        captureBt->setGeometry(QRect(330, 480, 140, 44));
        doneBt = new QPushButton(RegisterDlg);
        doneBt->setObjectName(QStringLiteral("doneBt"));
        doneBt->setGeometry(QRect(490, 480, 100, 44));
        doneBt->setEnabled(false);
        cancelBt = new QPushButton(RegisterDlg);
        cancelBt->setObjectName(QStringLiteral("cancelBt"));
        cancelBt->setGeometry(QRect(610, 480, 100, 44));

        retranslateUi(RegisterDlg);

        QMetaObject::connectSlotsByName(RegisterDlg);
    } // setupUi

    void retranslateUi(QDialog *RegisterDlg)
    {
        RegisterDlg->setWindowTitle(QApplication::translate("RegisterDlg", "\344\272\272\350\204\270\346\263\250\345\206\214", 0));
        cameraLabel->setText(QString());
        label->setText(QApplication::translate("RegisterDlg", "\350\257\267\350\276\223\345\205\245\345\247\223\345\220\215\357\274\232", 0));
        nameEdit->setPlaceholderText(QApplication::translate("RegisterDlg", "\350\276\223\345\205\245\345\247\223\345\220\215\345\220\216\346\213\215\347\205\247", 0));
        confirmBt->setText(QApplication::translate("RegisterDlg", "\347\241\256\350\256\244", 0));
        progressLabel->setText(QApplication::translate("RegisterDlg", "0 / 10 \345\274\240", 0));
        statusLabel->setText(QApplication::translate("RegisterDlg", "\350\257\267\351\235\242\345\257\271\346\221\204\345\203\217\345\244\264", 0));
        captureBt->setText(QApplication::translate("RegisterDlg", "\346\213\215\347\205\247", 0));
        doneBt->setText(QApplication::translate("RegisterDlg", "\345\256\214\346\210\220", 0));
        cancelBt->setText(QApplication::translate("RegisterDlg", "\345\217\226\346\266\210", 0));
    } // retranslateUi

};

namespace Ui {
    class RegisterDlg: public Ui_RegisterDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTERDLG_H
