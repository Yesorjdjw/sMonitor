/********************************************************************************
** Form generated from reading UI file 'fileIndex.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILEINDEX_H
#define UI_FILEINDEX_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_fileIndex
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QListWidget *picWidget;
    QListWidget *vidWidget;
    QPushButton *exit_bt;
    QWidget *bottomBar;
    QPushButton *refreshBt;
    QPushButton *renameBt;
    QPushButton *deleteBt;
    QLineEdit *searchEdit;

    void setupUi(QMainWindow *fileIndex)
    {
        if (fileIndex->objectName().isEmpty())
            fileIndex->setObjectName(QString::fromUtf8("fileIndex"));
        fileIndex->resize(1024, 600);
        fileIndex->setMinimumSize(QSize(1024, 600));
        fileIndex->setMaximumSize(QSize(1024, 600));
        centralwidget = new QWidget(fileIndex);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(0, 0, 1024, 555));
        tabWidget->setFocusPolicy(Qt::NoFocus);
        picWidget = new QListWidget(centralwidget);
        picWidget->setObjectName(QString::fromUtf8("picWidget"));
        picWidget->setGeometry(QRect(0, 0, 1024, 555));
        picWidget->setStyleSheet(QString::fromUtf8("background: skyblue;"));
        vidWidget = new QListWidget(centralwidget);
        vidWidget->setObjectName(QString::fromUtf8("vidWidget"));
        vidWidget->setGeometry(QRect(0, 0, 1024, 555));
        vidWidget->setStyleSheet(QString::fromUtf8("background: skyblue;"));
        exit_bt = new QPushButton(centralwidget);
        exit_bt->setObjectName(QString::fromUtf8("exit_bt"));
        exit_bt->setGeometry(QRect(970, 0, 50, 28));
        exit_bt->setStyleSheet(QString::fromUtf8("border-image: url(:/images/exit.png);"));
        bottomBar = new QWidget(centralwidget);
        bottomBar->setObjectName(QString::fromUtf8("bottomBar"));
        bottomBar->setGeometry(QRect(0, 555, 1024, 45));
        bottomBar->setStyleSheet(QString::fromUtf8("background-color: #1a7bbd;"));
        refreshBt = new QPushButton(bottomBar);
        refreshBt->setObjectName(QString::fromUtf8("refreshBt"));
        refreshBt->setGeometry(QRect(24, 6, 100, 33));
        refreshBt->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"  background: rgba(255,255,255,0.9);\n"
"  color: #1a7bbd;\n"
"  border: 1px solid #1a7bbd;\n"
"  border-radius: 4px;\n"
"  font-size: 13px;\n"
"  font-weight: bold;\n"
"}\n"
"QPushButton:disabled { background: #888; color: #fff; }"));
        renameBt = new QPushButton(bottomBar);
        renameBt->setObjectName(QString::fromUtf8("renameBt"));
        renameBt->setGeometry(QRect(370, 6, 120, 33));
        renameBt->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"  background: #f0ad4e;\n"
"  color: #fff;\n"
"  border: none;\n"
"  border-radius: 4px;\n"
"  font-size: 13px;\n"
"  font-weight: bold;\n"
"}\n"
"QPushButton:disabled { background: #888; }"));
        deleteBt = new QPushButton(bottomBar);
        deleteBt->setObjectName(QString::fromUtf8("deleteBt"));
        deleteBt->setGeometry(QRect(510, 6, 120, 33));
        deleteBt->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"  background: #e74c3c;\n"
"  color: #fff;\n"
"  border: none;\n"
"  border-radius: 4px;\n"
"  font-size: 13px;\n"
"  font-weight: bold;\n"
"}\n"
"QPushButton:disabled { background: #888; }"));
        searchEdit = new QLineEdit(bottomBar);
        searchEdit->setObjectName(QString::fromUtf8("searchEdit"));
        searchEdit->setGeometry(QRect(780, 8, 140, 28));
        searchEdit->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"  background: rgba(255,255,255,0.9);\n"
"  border: 1px solid #ccc;\n"
"  border-radius: 4px;\n"
"  padding: 2px 6px;\n"
"  font-size: 12px;\n"
"}"));
        fileIndex->setCentralWidget(centralwidget);

        retranslateUi(fileIndex);

        tabWidget->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(fileIndex);
    } // setupUi

    void retranslateUi(QMainWindow *fileIndex)
    {
        fileIndex->setWindowTitle(QCoreApplication::translate("fileIndex", "MainWindow", nullptr));
        exit_bt->setText(QString());
        refreshBt->setText(QCoreApplication::translate("fileIndex", "\345\210\267\346\226\260", nullptr));
        renameBt->setText(QCoreApplication::translate("fileIndex", "\351\207\215\345\221\275\345\220\215", nullptr));
        deleteBt->setText(QCoreApplication::translate("fileIndex", "\345\210\240\351\231\244", nullptr));
        searchEdit->setPlaceholderText(QCoreApplication::translate("fileIndex", "\346\220\234\347\264\242...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class fileIndex: public Ui_fileIndex {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILEINDEX_H
