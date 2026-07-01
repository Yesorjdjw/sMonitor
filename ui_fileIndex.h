/********************************************************************************
** Form generated from reading UI file 'fileIndex.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILEINDEX_H
#define UI_FILEINDEX_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
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
            fileIndex->setObjectName(QStringLiteral("fileIndex"));
        fileIndex->resize(1024, 600);
        fileIndex->setMinimumSize(QSize(1024, 600));
        fileIndex->setMaximumSize(QSize(1024, 600));
        centralwidget = new QWidget(fileIndex);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(0, 0, 1024, 555));
        tabWidget->setFocusPolicy(Qt::NoFocus);
        picWidget = new QListWidget(centralwidget);
        picWidget->setObjectName(QStringLiteral("picWidget"));
        picWidget->setGeometry(QRect(0, 0, 1024, 555));
        picWidget->setStyleSheet(QStringLiteral("background: skyblue;"));
        vidWidget = new QListWidget(centralwidget);
        vidWidget->setObjectName(QStringLiteral("vidWidget"));
        vidWidget->setGeometry(QRect(0, 0, 1024, 555));
        vidWidget->setStyleSheet(QStringLiteral("background: skyblue;"));
        exit_bt = new QPushButton(centralwidget);
        exit_bt->setObjectName(QStringLiteral("exit_bt"));
        exit_bt->setGeometry(QRect(970, 0, 50, 28));
        exit_bt->setStyleSheet(QStringLiteral("border-image: url(:/images/exit.png);"));
        bottomBar = new QWidget(centralwidget);
        bottomBar->setObjectName(QStringLiteral("bottomBar"));
        bottomBar->setGeometry(QRect(0, 555, 1024, 45));
        bottomBar->setStyleSheet(QStringLiteral("background-color: #1a7bbd;"));
        refreshBt = new QPushButton(bottomBar);
        refreshBt->setObjectName(QStringLiteral("refreshBt"));
        refreshBt->setGeometry(QRect(24, 6, 100, 33));
        refreshBt->setStyleSheet(QLatin1String("QPushButton {\n"
"  background: rgba(255,255,255,0.9);\n"
"  color: #1a7bbd;\n"
"  border: 1px solid #1a7bbd;\n"
"  border-radius: 4px;\n"
"  font-size: 13px;\n"
"  font-weight: bold;\n"
"}\n"
"QPushButton:disabled { background: #888; color: #fff; }"));
        renameBt = new QPushButton(bottomBar);
        renameBt->setObjectName(QStringLiteral("renameBt"));
        renameBt->setGeometry(QRect(370, 6, 120, 33));
        renameBt->setStyleSheet(QLatin1String("QPushButton {\n"
"  background: #f0ad4e;\n"
"  color: #fff;\n"
"  border: none;\n"
"  border-radius: 4px;\n"
"  font-size: 13px;\n"
"  font-weight: bold;\n"
"}\n"
"QPushButton:disabled { background: #888; }"));
        deleteBt = new QPushButton(bottomBar);
        deleteBt->setObjectName(QStringLiteral("deleteBt"));
        deleteBt->setGeometry(QRect(510, 6, 120, 33));
        deleteBt->setStyleSheet(QLatin1String("QPushButton {\n"
"  background: #e74c3c;\n"
"  color: #fff;\n"
"  border: none;\n"
"  border-radius: 4px;\n"
"  font-size: 13px;\n"
"  font-weight: bold;\n"
"}\n"
"QPushButton:disabled { background: #888; }"));
        searchEdit = new QLineEdit(bottomBar);
        searchEdit->setObjectName(QStringLiteral("searchEdit"));
        searchEdit->setGeometry(QRect(780, 8, 140, 28));
        searchEdit->setStyleSheet(QLatin1String("QLineEdit {\n"
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
        fileIndex->setWindowTitle(QApplication::translate("fileIndex", "MainWindow", 0));
        exit_bt->setText(QString());
        refreshBt->setText(QApplication::translate("fileIndex", "\345\210\267\346\226\260", 0));
        renameBt->setText(QApplication::translate("fileIndex", "\351\207\215\345\221\275\345\220\215", 0));
        deleteBt->setText(QApplication::translate("fileIndex", "\345\210\240\351\231\244", 0));
        searchEdit->setPlaceholderText(QApplication::translate("fileIndex", "\346\220\234\347\264\242...", 0));
    } // retranslateUi

};

namespace Ui {
    class fileIndex: public Ui_fileIndex {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILEINDEX_H
