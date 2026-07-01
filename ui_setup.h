/********************************************************************************
** Form generated from reading UI file 'setup.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETUP_H
#define UI_SETUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_setup
{
public:
    QFrame *leftCard;
    QWidget *leftTitleBar;
    QLabel *leftIcon;
    QLabel *leftTitleText;
    QPushButton *nav1;
    QPushButton *nav2;
    QPushButton *nav3;
    QPushButton *nav4;
    QPushButton *nav5;
    QPushButton *backBt;
    QFrame *rightCard;
    QWidget *rightTitleBar;
    QLabel *rightIcon;
    QLabel *rightTitleText;
    QStackedWidget *contentStack;
    QWidget *pageCruise;
    QFrame *cruiseCard;
    QWidget *cruiseTitleBar;
    QLabel *cruiseTitle;
    QLabel *cruiseIcon;
    QLabel *cruiseLabel;
    QCheckBox *toggleCruise;
    QLabel *presetLabel;
    QCheckBox *ckDoor;
    QCheckBox *ckWarehouse;
    QCheckBox *ckParking;
    QCheckBox *ckWall;
    QLabel *speedIcon;
    QLabel *speedLabel;
    QSlider *speedSlider;
    QLabel *speedVal;
    QWidget *pageAlarm;
    QFrame *alarmCard;
    QWidget *alarmTitleBar;
    QLabel *alarmTitle;
    QCheckBox *ckMotion;
    QCheckBox *ckCover;
    QCheckBox *ckIntrude;
    QCheckBox *ckStorage;
    QCheckBox *ckFace;
    QLabel *linkIcon;
    QLabel *linkLabel;
    QComboBox *linkCombo;
    QLabel *timeIcon;
    QLabel *timeLabel;
    QTimeEdit *timeStart;
    QLabel *toLabel;
    QTimeEdit *timeEnd;
    QPushButton *resetBt;
    QPushButton *saveBt;
    QWidget *pageRecord;
    QFrame *storageCard;
    QWidget *storageTitleBar;
    QLabel *storageTitle;
    QLabel *totalLabel;
    QLabel *storageBarBg;
    QLabel *videoSeg;
    QLabel *photoSeg;
    QLabel *otherSeg;
    QLabel *freeSeg;
    QLabel *legendVideoDot;
    QLabel *legendVideo;
    QLabel *legendPhotoDot;
    QLabel *legendPhoto;
    QLabel *legendOtherDot;
    QLabel *legendOther;
    QLabel *legendFreeDot;
    QLabel *legendFree;
    QLabel *estimateLabel;
    QFrame *pathCard;
    QWidget *pathTitleBar;
    QLabel *pathTitle;
    QLabel *pathLabel;
    QLineEdit *pathInput;
    QPushButton *browseBt;
    QFrame *autoCard;
    QWidget *autoTitleBar;
    QLabel *autoTitle;
    QLabel *autoLabel;
    QCheckBox *autoToggle;
    QLabel *retainLabel;
    QComboBox *retainCombo;
    QLabel *policyLabel;
    QComboBox *policyCombo;
    QPushButton *resetBtRec;
    QPushButton *saveBtRec;
    QWidget *pageNetwork;
    QLabel *netPlaceholder;
    QWidget *pageUser;
    QLabel *userPlaceholder;

    void setupUi(QWidget *setup)
    {
        if (setup->objectName().isEmpty())
            setup->setObjectName(QStringLiteral("setup"));
        setup->resize(1024, 600);
        setup->setStyleSheet(QLatin1String("\n"
"QWidget#setup { background-color: #d6e8f5; border-radius: 16px; font-family: \"Helvetica Neue\",\"PingFang SC\",\"Microsoft YaHei\",sans-serif; }\n"
"QFrame#leftCard, QFrame#rightCard { background: #ffffff; border: none; border-radius: 14px; }\n"
"QWidget#leftTitleBar, QWidget#rightTitleBar { background-color: #1a7bbd; border-top-left-radius: 14px; border-top-right-radius: 14px; }\n"
"QLabel#leftTitleText { color: #ffffff; font-size: 14px; font-weight: 600; background: transparent; }\n"
"QPushButton#nav1, QPushButton#nav2, QPushButton#nav3, QPushButton#nav4, QPushButton#nav5 { background: transparent; border: none; text-align: left; font-size: 13px; font-weight: 500; color: #333333; padding-left: 44px; }\n"
"QPushButton#nav1:hover, QPushButton#nav2:hover, QPushButton#nav3:hover, QPushButton#nav4:hover, QPushButton#nav5:hover { background: #eef4fa; }\n"
"QCheckBox { spacing: 8px; font-size: 12px; color: #333333; }\n"
"QCheckBox::indicator { width: 18px; height: 18px; border-radius: 4px; border: 2px solid #"
                        "cccccc; background: #ffffff; }\n"
"QCheckBox::indicator:checked { border: 2px solid #1a7bbd; background: #1a7bbd; image: url(:/images/icon_check.svg); }\n"
"QCheckBox#toggleCruise, QCheckBox#autoToggle { spacing: 10px; font-size: 13px; }\n"
"QCheckBox#toggleCruise::indicator, QCheckBox#autoToggle::indicator { width: 44px; height: 24px; border-radius: 12px; border: none; background: #e0e0e0; }\n"
"QCheckBox#toggleCruise::indicator:checked, QCheckBox#autoToggle::indicator:checked { background: #34c759; image: url(:/images/icon_toggle_on.svg); }\n"
"QSlider#speedSlider::groove:horizontal { height: 6px; border-radius: 3px; background: #e0e0e0; }\n"
"QSlider#speedSlider::handle:horizontal { width: 20px; height: 20px; margin: -7px 0; border-radius: 10px; background: #1a7bbd; border: none; }\n"
"QSlider#speedSlider::sub-page:horizontal { background: #1a7bbd; border-radius: 3px; }\n"
"QComboBox { background: #f5f5f7; border: 1px solid #e0e0e0; border-radius: 6px; padding: 6px 10px; font-size: 12px; color: #333333; min"
                        "-width: 140px; }\n"
"QComboBox:hover { border-color: #1a7bbd; } QComboBox::drop-down { border: none; width: 20px; }\n"
"QComboBox QAbstractItemView { background: #ffffff; color: #333333; selection-background-color: #1a7bbd; selection-color: #ffffff; border: 1px solid #e0e0e0; outline: none; }\n"
"QTimeEdit { background: #f5f5f7; border: 1px solid #e0e0e0; border-radius: 6px; padding: 6px 10px; font-size: 12px; color: #333333; min-width: 90px; } QTimeEdit:hover { border-color: #1a7bbd; }\n"
"QLineEdit { background: #f5f5f7; border: 1px solid #e0e0e0; border-radius: 6px; padding: 6px 10px; font-size: 12px; color: #333333; } QLineEdit:hover { border-color: #1a7bbd; }\n"
"QPushButton#saveBt, QPushButton#saveBtRec { background: #1a7bbd; border: none; border-radius: 8px; color: #ffffff; font-size: 13px; padding: 10px 28px; }\n"
"QPushButton#saveBt:hover, QPushButton#saveBtRec:hover { background: #155d94; }\n"
"QPushButton#resetBt, QPushButton#resetBtRec { background: #ffffff; border: 1px solid #dddddd; border-radius"
                        ": 8px; color: #888888; font-size: 13px; padding: 10px 20px; }\n"
"QPushButton#resetBt:hover, QPushButton#resetBtRec:hover { border-color: #bbbbbb; color: #555555; }\n"
"QPushButton#backBt { background: transparent; border: 1px solid rgba(26,123,189,0.25); border-radius: 14px; color: #1a7bbd; font-size: 12px; font-weight: 500; text-align: center; }\n"
"QPushButton#backBt:hover { background: rgba(26,123,189,0.06); border-color: rgba(26,123,189,0.5); }\n"
"QFrame#storageCard, QFrame#pathCard, QFrame#autoCard { background: #fafbfc; border: 1px solid #eeeeee; border-radius: 10px; }\n"
"QPushButton#browseBt { background: #f5f5f7; border: 1px solid #e0e0e0; border-radius: 6px; color: #333; font-size: 17px; }\n"
"QPushButton#browseBt:hover { background: #e8e8ea; border-color: #1a7bbd; }\n"
"QLabel#storageBarBg { background: #e8e8e8; border-radius: 7px; }\n"
"QLabel#videoSeg { background: #1a7bbd; border-top-left-radius: 7px; border-bottom-left-radius: 7px; }\n"
"QLabel#photoSeg { background: #34c759; }\n"
"QLabel#othe"
                        "rSeg { background: #ff9500; }\n"
"QLabel#freeSeg { background: #e0e0e0; border-top-right-radius: 7px; border-bottom-right-radius: 7px; }\n"
"QLabel#legendVideoDot, QLabel#legendPhotoDot, QLabel#legendOtherDot, QLabel#legendFreeDot { border-radius: 4px; min-width: 12px; max-width: 12px; min-height: 12px; max-height: 12px; }\n"
"QLabel#legendVideoDot { background: #1a7bbd; } QLabel#legendPhotoDot { background: #34c759; }\n"
"QLabel#legendOtherDot { background: #ff9500; } QLabel#legendFreeDot { background: #d0d0d0; }\n"
"  "));
        leftCard = new QFrame(setup);
        leftCard->setObjectName(QStringLiteral("leftCard"));
        leftCard->setGeometry(QRect(15, 15, 210, 542));
        leftTitleBar = new QWidget(leftCard);
        leftTitleBar->setObjectName(QStringLiteral("leftTitleBar"));
        leftTitleBar->setGeometry(QRect(0, 0, 210, 44));
        leftIcon = new QLabel(leftTitleBar);
        leftIcon->setObjectName(QStringLiteral("leftIcon"));
        leftIcon->setGeometry(QRect(12, 10, 24, 24));
        leftIcon->setPixmap(QPixmap(QString::fromUtf8(":/images/icon_settings.svg")));
        leftIcon->setScaledContents(true);
        leftTitleText = new QLabel(leftTitleBar);
        leftTitleText->setObjectName(QStringLiteral("leftTitleText"));
        leftTitleText->setGeometry(QRect(40, 6, 160, 32));
        nav1 = new QPushButton(leftCard);
        nav1->setObjectName(QStringLiteral("nav1"));
        nav1->setGeometry(QRect(0, 52, 210, 42));
        nav2 = new QPushButton(leftCard);
        nav2->setObjectName(QStringLiteral("nav2"));
        nav2->setGeometry(QRect(0, 98, 210, 42));
        nav3 = new QPushButton(leftCard);
        nav3->setObjectName(QStringLiteral("nav3"));
        nav3->setGeometry(QRect(0, 144, 210, 42));
        nav4 = new QPushButton(leftCard);
        nav4->setObjectName(QStringLiteral("nav4"));
        nav4->setGeometry(QRect(0, 190, 210, 42));
        nav5 = new QPushButton(leftCard);
        nav5->setObjectName(QStringLiteral("nav5"));
        nav5->setGeometry(QRect(0, 236, 210, 42));
        backBt = new QPushButton(leftCard);
        backBt->setObjectName(QStringLiteral("backBt"));
        backBt->setGeometry(QRect(40, 473, 131, 41));
        rightCard = new QFrame(setup);
        rightCard->setObjectName(QStringLiteral("rightCard"));
        rightCard->setGeometry(QRect(240, 15, 769, 542));
        rightTitleBar = new QWidget(rightCard);
        rightTitleBar->setObjectName(QStringLiteral("rightTitleBar"));
        rightTitleBar->setGeometry(QRect(0, 0, 769, 44));
        rightIcon = new QLabel(rightTitleBar);
        rightIcon->setObjectName(QStringLiteral("rightIcon"));
        rightIcon->setGeometry(QRect(12, 10, 24, 24));
        rightIcon->setPixmap(QPixmap(QString::fromUtf8(":/images/icon_settings.svg")));
        rightIcon->setScaledContents(true);
        rightTitleText = new QLabel(rightTitleBar);
        rightTitleText->setObjectName(QStringLiteral("rightTitleText"));
        rightTitleText->setGeometry(QRect(40, 6, 200, 32));
        contentStack = new QStackedWidget(rightCard);
        contentStack->setObjectName(QStringLiteral("contentStack"));
        contentStack->setGeometry(QRect(0, 44, 769, 498));
        pageCruise = new QWidget();
        pageCruise->setObjectName(QStringLiteral("pageCruise"));
        cruiseCard = new QFrame(pageCruise);
        cruiseCard->setObjectName(QStringLiteral("cruiseCard"));
        cruiseCard->setGeometry(QRect(16, 14, 737, 204));
        cruiseCard->setStyleSheet(QStringLiteral("background: #fafbfc; border: 1px solid #eeeeee; border-radius: 10px;"));
        cruiseTitleBar = new QWidget(cruiseCard);
        cruiseTitleBar->setObjectName(QStringLiteral("cruiseTitleBar"));
        cruiseTitleBar->setGeometry(QRect(0, 0, 737, 36));
        cruiseTitleBar->setStyleSheet(QStringLiteral("background-color: #1a7bbd; border-top-left-radius: 10px; border-top-right-radius: 10px;"));
        cruiseTitle = new QLabel(cruiseTitleBar);
        cruiseTitle->setObjectName(QStringLiteral("cruiseTitle"));
        cruiseTitle->setGeometry(QRect(14, 6, 120, 24));
        cruiseTitle->setStyleSheet(QStringLiteral("color: #ffffff; font-size: 13px; font-weight: 600; background: transparent;"));
        cruiseIcon = new QLabel(cruiseCard);
        cruiseIcon->setObjectName(QStringLiteral("cruiseIcon"));
        cruiseIcon->setGeometry(QRect(16, 52, 20, 20));
        cruiseIcon->setPixmap(QPixmap(QString::fromUtf8(":/images/icon_target.svg")));
        cruiseIcon->setScaledContents(true);
        cruiseLabel = new QLabel(cruiseCard);
        cruiseLabel->setObjectName(QStringLiteral("cruiseLabel"));
        cruiseLabel->setGeometry(QRect(38, 54, 70, 24));
        cruiseLabel->setStyleSheet(QStringLiteral("color: #333; font-size: 13px; background: transparent;"));
        toggleCruise = new QCheckBox(cruiseCard);
        toggleCruise->setObjectName(QStringLiteral("toggleCruise"));
        toggleCruise->setGeometry(QRect(100, 48, 60, 30));
        presetLabel = new QLabel(cruiseCard);
        presetLabel->setObjectName(QStringLiteral("presetLabel"));
        presetLabel->setGeometry(QRect(180, 54, 80, 24));
        presetLabel->setStyleSheet(QStringLiteral("color: #333; font-size: 13px; background: transparent;"));
        ckDoor = new QCheckBox(cruiseCard);
        ckDoor->setObjectName(QStringLiteral("ckDoor"));
        ckDoor->setEnabled(true);
        ckDoor->setGeometry(QRect(275, 52, 60, 24));
        ckWarehouse = new QCheckBox(cruiseCard);
        ckWarehouse->setObjectName(QStringLiteral("ckWarehouse"));
        ckWarehouse->setEnabled(true);
        ckWarehouse->setGeometry(QRect(350, 52, 70, 24));
        ckParking = new QCheckBox(cruiseCard);
        ckParking->setObjectName(QStringLiteral("ckParking"));
        ckParking->setGeometry(QRect(430, 52, 70, 24));
        ckWall = new QCheckBox(cruiseCard);
        ckWall->setObjectName(QStringLiteral("ckWall"));
        ckWall->setGeometry(QRect(510, 52, 60, 24));
        speedIcon = new QLabel(cruiseCard);
        speedIcon->setObjectName(QStringLiteral("speedIcon"));
        speedIcon->setGeometry(QRect(16, 108, 20, 20));
        speedIcon->setPixmap(QPixmap(QString::fromUtf8(":/images/icon_speed.svg")));
        speedIcon->setScaledContents(true);
        speedLabel = new QLabel(cruiseCard);
        speedLabel->setObjectName(QStringLiteral("speedLabel"));
        speedLabel->setGeometry(QRect(38, 110, 70, 24));
        speedLabel->setStyleSheet(QStringLiteral("color: #333; font-size: 13px; background: transparent;"));
        speedSlider = new QSlider(cruiseCard);
        speedSlider->setObjectName(QStringLiteral("speedSlider"));
        speedSlider->setGeometry(QRect(100, 110, 240, 20));
        speedSlider->setMinimum(1);
        speedSlider->setMaximum(10);
        speedSlider->setValue(5);
        speedSlider->setOrientation(Qt::Horizontal);
        speedVal = new QLabel(cruiseCard);
        speedVal->setObjectName(QStringLiteral("speedVal"));
        speedVal->setGeometry(QRect(348, 108, 30, 24));
        speedVal->setStyleSheet(QStringLiteral("color: #1a7bbd; font-size: 16px; font-weight: bold; background: transparent;"));
        speedVal->setAlignment(Qt::AlignCenter);
        contentStack->addWidget(pageCruise);
        pageAlarm = new QWidget();
        pageAlarm->setObjectName(QStringLiteral("pageAlarm"));
        alarmCard = new QFrame(pageAlarm);
        alarmCard->setObjectName(QStringLiteral("alarmCard"));
        alarmCard->setGeometry(QRect(16, 14, 737, 270));
        alarmCard->setStyleSheet(QStringLiteral("background: #fafbfc; border: 1px solid #eeeeee; border-radius: 10px;"));
        alarmTitleBar = new QWidget(alarmCard);
        alarmTitleBar->setObjectName(QStringLiteral("alarmTitleBar"));
        alarmTitleBar->setGeometry(QRect(0, 0, 737, 36));
        alarmTitleBar->setStyleSheet(QStringLiteral("background-color: #1a7bbd; border-top-left-radius: 10px; border-top-right-radius: 10px;"));
        alarmTitle = new QLabel(alarmTitleBar);
        alarmTitle->setObjectName(QStringLiteral("alarmTitle"));
        alarmTitle->setGeometry(QRect(14, 6, 160, 24));
        alarmTitle->setStyleSheet(QStringLiteral("color: #ffffff; font-size: 13px; font-weight: 600; background: transparent;"));
        ckMotion = new QCheckBox(alarmCard);
        ckMotion->setObjectName(QStringLiteral("ckMotion"));
        ckMotion->setGeometry(QRect(24, 52, 100, 24));
        ckMotion->setChecked(true);
        ckCover = new QCheckBox(alarmCard);
        ckCover->setObjectName(QStringLiteral("ckCover"));
        ckCover->setGeometry(QRect(24, 84, 100, 24));
        ckCover->setChecked(true);
        ckIntrude = new QCheckBox(alarmCard);
        ckIntrude->setObjectName(QStringLiteral("ckIntrude"));
        ckIntrude->setGeometry(QRect(260, 52, 100, 24));
        ckIntrude->setChecked(true);
        ckStorage = new QCheckBox(alarmCard);
        ckStorage->setObjectName(QStringLiteral("ckStorage"));
        ckStorage->setGeometry(QRect(260, 84, 130, 24));
        ckStorage->setChecked(true);
        ckFace = new QCheckBox(alarmCard);
        ckFace->setObjectName(QStringLiteral("ckFace"));
        ckFace->setGeometry(QRect(500, 52, 120, 24));
        ckFace->setChecked(true);
        linkIcon = new QLabel(alarmCard);
        linkIcon->setObjectName(QStringLiteral("linkIcon"));
        linkIcon->setGeometry(QRect(20, 130, 20, 20));
        linkIcon->setPixmap(QPixmap(QString::fromUtf8(":/images/icon_bell.svg")));
        linkIcon->setScaledContents(true);
        linkLabel = new QLabel(alarmCard);
        linkLabel->setObjectName(QStringLiteral("linkLabel"));
        linkLabel->setGeometry(QRect(42, 132, 80, 24));
        linkLabel->setStyleSheet(QStringLiteral("color: #333; font-size: 13px; background: transparent;"));
        linkCombo = new QComboBox(alarmCard);
        linkCombo->setObjectName(QStringLiteral("linkCombo"));
        linkCombo->setGeometry(QRect(120, 128, 162, 30));
        timeIcon = new QLabel(alarmCard);
        timeIcon->setObjectName(QStringLiteral("timeIcon"));
        timeIcon->setGeometry(QRect(336, 130, 20, 20));
        timeIcon->setPixmap(QPixmap(QString::fromUtf8(":/images/icon_clock.svg")));
        timeIcon->setScaledContents(true);
        timeLabel = new QLabel(alarmCard);
        timeLabel->setObjectName(QStringLiteral("timeLabel"));
        timeLabel->setGeometry(QRect(358, 132, 100, 24));
        timeLabel->setStyleSheet(QStringLiteral("color: #333; font-size: 13px; background: transparent;"));
        timeStart = new QTimeEdit(alarmCard);
        timeStart->setObjectName(QStringLiteral("timeStart"));
        timeStart->setGeometry(QRect(450, 126, 112, 30));
        timeStart->setTime(QTime(0, 0, 0));
        toLabel = new QLabel(alarmCard);
        toLabel->setObjectName(QStringLiteral("toLabel"));
        toLabel->setGeometry(QRect(556, 130, 20, 24));
        toLabel->setStyleSheet(QStringLiteral("color: #888; font-size: 12px; background: transparent;"));
        toLabel->setAlignment(Qt::AlignCenter);
        timeEnd = new QTimeEdit(alarmCard);
        timeEnd->setObjectName(QStringLiteral("timeEnd"));
        timeEnd->setGeometry(QRect(580, 126, 112, 30));
        timeEnd->setTime(QTime(0, 0, 0));
        resetBt = new QPushButton(pageAlarm);
        resetBt->setObjectName(QStringLiteral("resetBt"));
        resetBt->setGeometry(QRect(510, 496, 110, 34));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/icon_undo.svg"), QSize(), QIcon::Normal, QIcon::Off);
        resetBt->setIcon(icon);
        resetBt->setIconSize(QSize(16, 16));
        saveBt = new QPushButton(pageAlarm);
        saveBt->setObjectName(QStringLiteral("saveBt"));
        saveBt->setGeometry(QRect(628, 496, 110, 34));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/icon_save.svg"), QSize(), QIcon::Normal, QIcon::Off);
        saveBt->setIcon(icon1);
        saveBt->setIconSize(QSize(16, 16));
        contentStack->addWidget(pageAlarm);
        pageRecord = new QWidget();
        pageRecord->setObjectName(QStringLiteral("pageRecord"));
        storageCard = new QFrame(pageRecord);
        storageCard->setObjectName(QStringLiteral("storageCard"));
        storageCard->setGeometry(QRect(16, 6, 737, 155));
        storageTitleBar = new QWidget(storageCard);
        storageTitleBar->setObjectName(QStringLiteral("storageTitleBar"));
        storageTitleBar->setGeometry(QRect(0, 0, 737, 36));
        storageTitleBar->setStyleSheet(QStringLiteral("background-color: #1a7bbd; border-top-left-radius: 10px; border-top-right-radius: 10px;"));
        storageTitle = new QLabel(storageTitleBar);
        storageTitle->setObjectName(QStringLiteral("storageTitle"));
        storageTitle->setGeometry(QRect(14, 6, 160, 24));
        storageTitle->setStyleSheet(QStringLiteral("color: #ffffff; font-size: 13px; font-weight: 600; background: transparent;"));
        totalLabel = new QLabel(storageCard);
        totalLabel->setObjectName(QStringLiteral("totalLabel"));
        totalLabel->setGeometry(QRect(24, 44, 690, 18));
        totalLabel->setStyleSheet(QStringLiteral("color: #555; font-size: 11px; background: transparent;"));
        storageBarBg = new QLabel(storageCard);
        storageBarBg->setObjectName(QStringLiteral("storageBarBg"));
        storageBarBg->setGeometry(QRect(24, 66, 690, 14));
        videoSeg = new QLabel(storageCard);
        videoSeg->setObjectName(QStringLiteral("videoSeg"));
        videoSeg->setGeometry(QRect(24, 66, 0, 14));
        photoSeg = new QLabel(storageCard);
        photoSeg->setObjectName(QStringLiteral("photoSeg"));
        photoSeg->setGeometry(QRect(24, 66, 0, 14));
        otherSeg = new QLabel(storageCard);
        otherSeg->setObjectName(QStringLiteral("otherSeg"));
        otherSeg->setGeometry(QRect(24, 66, 0, 14));
        freeSeg = new QLabel(storageCard);
        freeSeg->setObjectName(QStringLiteral("freeSeg"));
        freeSeg->setGeometry(QRect(24, 66, 690, 14));
        legendVideoDot = new QLabel(storageCard);
        legendVideoDot->setObjectName(QStringLiteral("legendVideoDot"));
        legendVideoDot->setGeometry(QRect(24, 88, 12, 12));
        legendVideo = new QLabel(storageCard);
        legendVideo->setObjectName(QStringLiteral("legendVideo"));
        legendVideo->setGeometry(QRect(40, 86, 150, 18));
        legendVideo->setStyleSheet(QStringLiteral("color: #555; font-size: 11px; background: transparent;"));
        legendPhotoDot = new QLabel(storageCard);
        legendPhotoDot->setObjectName(QStringLiteral("legendPhotoDot"));
        legendPhotoDot->setGeometry(QRect(200, 88, 12, 12));
        legendPhoto = new QLabel(storageCard);
        legendPhoto->setObjectName(QStringLiteral("legendPhoto"));
        legendPhoto->setGeometry(QRect(216, 86, 130, 18));
        legendPhoto->setStyleSheet(QStringLiteral("color: #555; font-size: 11px; background: transparent;"));
        legendOtherDot = new QLabel(storageCard);
        legendOtherDot->setObjectName(QStringLiteral("legendOtherDot"));
        legendOtherDot->setGeometry(QRect(360, 88, 12, 12));
        legendOther = new QLabel(storageCard);
        legendOther->setObjectName(QStringLiteral("legendOther"));
        legendOther->setGeometry(QRect(376, 86, 130, 18));
        legendOther->setStyleSheet(QStringLiteral("color: #555; font-size: 11px; background: transparent;"));
        legendFreeDot = new QLabel(storageCard);
        legendFreeDot->setObjectName(QStringLiteral("legendFreeDot"));
        legendFreeDot->setGeometry(QRect(520, 88, 12, 12));
        legendFree = new QLabel(storageCard);
        legendFree->setObjectName(QStringLiteral("legendFree"));
        legendFree->setGeometry(QRect(536, 86, 170, 18));
        legendFree->setStyleSheet(QStringLiteral("color: #555; font-size: 11px; background: transparent;"));
        estimateLabel = new QLabel(storageCard);
        estimateLabel->setObjectName(QStringLiteral("estimateLabel"));
        estimateLabel->setGeometry(QRect(24, 118, 400, 24));
        estimateLabel->setStyleSheet(QStringLiteral("color: #888; font-size: 12px; background: transparent;"));
        pathCard = new QFrame(pageRecord);
        pathCard->setObjectName(QStringLiteral("pathCard"));
        pathCard->setGeometry(QRect(16, 170, 737, 95));
        pathTitleBar = new QWidget(pathCard);
        pathTitleBar->setObjectName(QStringLiteral("pathTitleBar"));
        pathTitleBar->setGeometry(QRect(0, 0, 737, 36));
        pathTitleBar->setStyleSheet(QStringLiteral("background-color: #1a7bbd; border-top-left-radius: 10px; border-top-right-radius: 10px;"));
        pathTitle = new QLabel(pathTitleBar);
        pathTitle->setObjectName(QStringLiteral("pathTitle"));
        pathTitle->setGeometry(QRect(14, 6, 160, 24));
        pathTitle->setStyleSheet(QStringLiteral("color: #ffffff; font-size: 13px; font-weight: 600; background: transparent;"));
        pathLabel = new QLabel(pathCard);
        pathLabel->setObjectName(QStringLiteral("pathLabel"));
        pathLabel->setGeometry(QRect(24, 50, 70, 30));
        pathLabel->setStyleSheet(QStringLiteral("color: #333; font-size: 13px; background: transparent;"));
        pathInput = new QLineEdit(pathCard);
        pathInput->setObjectName(QStringLiteral("pathInput"));
        pathInput->setGeometry(QRect(100, 48, 535, 32));
        pathInput->setReadOnly(true);
        browseBt = new QPushButton(pathCard);
        browseBt->setObjectName(QStringLiteral("browseBt"));
        browseBt->setGeometry(QRect(645, 46, 64, 34));
        autoCard = new QFrame(pageRecord);
        autoCard->setObjectName(QStringLiteral("autoCard"));
        autoCard->setGeometry(QRect(16, 275, 737, 110));
        autoTitleBar = new QWidget(autoCard);
        autoTitleBar->setObjectName(QStringLiteral("autoTitleBar"));
        autoTitleBar->setGeometry(QRect(0, 0, 737, 36));
        autoTitleBar->setStyleSheet(QStringLiteral("background-color: #1a7bbd; border-top-left-radius: 10px; border-top-right-radius: 10px;"));
        autoTitle = new QLabel(autoTitleBar);
        autoTitle->setObjectName(QStringLiteral("autoTitle"));
        autoTitle->setGeometry(QRect(14, 6, 200, 24));
        autoTitle->setStyleSheet(QStringLiteral("color: #ffffff; font-size: 13px; font-weight: 600; background: transparent;"));
        autoLabel = new QLabel(autoCard);
        autoLabel->setObjectName(QStringLiteral("autoLabel"));
        autoLabel->setGeometry(QRect(24, 52, 70, 24));
        autoLabel->setStyleSheet(QStringLiteral("color: #333; font-size: 13px; background: transparent;"));
        autoToggle = new QCheckBox(autoCard);
        autoToggle->setObjectName(QStringLiteral("autoToggle"));
        autoToggle->setGeometry(QRect(100, 48, 60, 30));
        retainLabel = new QLabel(autoCard);
        retainLabel->setObjectName(QStringLiteral("retainLabel"));
        retainLabel->setGeometry(QRect(210, 52, 100, 24));
        retainLabel->setStyleSheet(QStringLiteral("color: #333; font-size: 13px; background: transparent;"));
        retainCombo = new QComboBox(autoCard);
        retainCombo->setObjectName(QStringLiteral("retainCombo"));
        retainCombo->setGeometry(QRect(300, 48, 120, 30));
        policyLabel = new QLabel(autoCard);
        policyLabel->setObjectName(QStringLiteral("policyLabel"));
        policyLabel->setGeometry(QRect(450, 52, 90, 24));
        policyLabel->setStyleSheet(QStringLiteral("color: #333; font-size: 13px; background: transparent;"));
        policyCombo = new QComboBox(autoCard);
        policyCombo->setObjectName(QStringLiteral("policyCombo"));
        policyCombo->setGeometry(QRect(540, 48, 180, 30));
        resetBtRec = new QPushButton(pageRecord);
        resetBtRec->setObjectName(QStringLiteral("resetBtRec"));
        resetBtRec->setGeometry(QRect(477, 440, 130, 34));
        resetBtRec->setIcon(icon);
        resetBtRec->setIconSize(QSize(16, 16));
        saveBtRec = new QPushButton(pageRecord);
        saveBtRec->setObjectName(QStringLiteral("saveBtRec"));
        saveBtRec->setGeometry(QRect(617, 440, 130, 34));
        saveBtRec->setIcon(icon1);
        saveBtRec->setIconSize(QSize(16, 16));
        contentStack->addWidget(pageRecord);
        pageNetwork = new QWidget();
        pageNetwork->setObjectName(QStringLiteral("pageNetwork"));
        netPlaceholder = new QLabel(pageNetwork);
        netPlaceholder->setObjectName(QStringLiteral("netPlaceholder"));
        netPlaceholder->setGeometry(QRect(30, 200, 709, 60));
        netPlaceholder->setStyleSheet(QStringLiteral("color: #999999; font-size: 14px; background: transparent;"));
        netPlaceholder->setAlignment(Qt::AlignCenter);
        contentStack->addWidget(pageNetwork);
        pageUser = new QWidget();
        pageUser->setObjectName(QStringLiteral("pageUser"));
        userPlaceholder = new QLabel(pageUser);
        userPlaceholder->setObjectName(QStringLiteral("userPlaceholder"));
        userPlaceholder->setGeometry(QRect(30, 200, 709, 60));
        userPlaceholder->setStyleSheet(QStringLiteral("color: #999999; font-size: 14px; background: transparent;"));
        userPlaceholder->setAlignment(Qt::AlignCenter);
        contentStack->addWidget(pageUser);

        retranslateUi(setup);

        contentStack->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(setup);
    } // setupUi

    void retranslateUi(QWidget *setup)
    {
        setup->setWindowTitle(QApplication::translate("setup", "\346\250\241\345\274\217\350\256\276\347\275\256", 0));
        leftIcon->setText(QString());
        leftTitleText->setText(QApplication::translate("setup", "\350\256\276\347\275\256\351\200\211\351\241\271", 0));
        nav1->setText(QApplication::translate("setup", "\345\267\241\350\210\252\350\256\276\347\275\256", 0));
        nav2->setText(QApplication::translate("setup", "\345\221\212\350\255\246\347\255\226\347\225\245\351\205\215\347\275\256", 0));
        nav3->setText(QApplication::translate("setup", "\345\275\225\345\203\217\345\255\230\345\202\250\350\256\276\347\275\256", 0));
        nav4->setText(QApplication::translate("setup", "\347\275\221\347\273\234\344\270\216\350\256\276\345\244\207\347\256\241\347\220\206", 0));
        nav5->setText(QApplication::translate("setup", "\347\224\250\346\210\267\346\235\203\351\231\220\347\256\241\347\220\206", 0));
        backBt->setText(QApplication::translate("setup", "  < \350\277\224\345\233\236\344\270\273\351\241\265", 0));
        rightIcon->setText(QString());
        rightTitleText->setText(QApplication::translate("setup", "\345\267\241\350\210\252\350\256\276\347\275\256", 0));
        cruiseTitle->setText(QApplication::translate("setup", "\345\267\241\350\210\252\350\256\276\347\275\256", 0));
        cruiseIcon->setText(QString());
        cruiseLabel->setText(QApplication::translate("setup", "\350\207\252\345\212\250\345\267\241\350\210\252", 0));
        toggleCruise->setText(QString());
        presetLabel->setText(QApplication::translate("setup", "\345\267\241\350\210\252\351\242\204\347\275\256\344\275\215", 0));
        ckDoor->setText(QApplication::translate("setup", "\345\244\247\351\227\250", 0));
        ckWarehouse->setText(QApplication::translate("setup", "\344\273\223\345\272\223", 0));
        ckParking->setText(QApplication::translate("setup", "\345\201\234\350\275\246\345\234\272", 0));
        ckWall->setText(QApplication::translate("setup", "\345\233\264\345\242\231", 0));
        speedIcon->setText(QString());
        speedLabel->setText(QApplication::translate("setup", "\345\267\241\350\210\252\351\200\237\345\272\246", 0));
        speedVal->setText(QApplication::translate("setup", "5", 0));
        alarmTitle->setText(QApplication::translate("setup", "\345\221\212\350\255\246\347\255\226\347\225\245\351\205\215\347\275\256", 0));
        ckMotion->setText(QApplication::translate("setup", "\347\247\273\345\212\250\344\276\246\346\265\213", 0));
        ckCover->setText(QApplication::translate("setup", "\347\224\273\351\235\242\351\201\256\346\214\241", 0));
        ckIntrude->setText(QApplication::translate("setup", "\345\214\272\345\237\237\345\205\245\344\276\265", 0));
        ckStorage->setText(QApplication::translate("setup", "\345\255\230\345\202\250\347\251\272\351\227\264\344\270\215\350\266\263", 0));
        ckFace->setText(QApplication::translate("setup", "\344\272\272\350\204\270\350\257\206\345\210\253\345\274\202\345\270\270", 0));
        linkIcon->setText(QString());
        linkLabel->setText(QApplication::translate("setup", "\345\221\212\350\255\246\350\201\224\345\212\250", 0));
        linkCombo->clear();
        linkCombo->insertItems(0, QStringList()
         << QApplication::translate("setup", "\345\274\271\347\252\227 + \345\243\260\351\237\263", 0)
         << QApplication::translate("setup", "\344\273\205\345\274\271\347\252\227", 0)
         << QApplication::translate("setup", "\344\273\205\345\243\260\351\237\263", 0)
         << QApplication::translate("setup", "\344\270\215\350\201\224\345\212\250", 0)
        );
        timeIcon->setText(QString());
        timeLabel->setText(QApplication::translate("setup", "\345\270\203\351\230\262\346\227\266\351\227\264\346\256\265", 0));
        timeStart->setDisplayFormat(QApplication::translate("setup", "HH:mm", 0));
        toLabel->setText(QApplication::translate("setup", "\350\207\263", 0));
        timeEnd->setDisplayFormat(QApplication::translate("setup", "HH:mm", 0));
        resetBt->setText(QApplication::translate("setup", "  \346\201\242\345\244\215\351\273\230\350\256\244", 0));
        saveBt->setText(QApplication::translate("setup", "  \344\277\235\345\255\230\350\256\276\347\275\256", 0));
        storageTitle->setText(QApplication::translate("setup", "\345\255\230\345\202\250\345\256\271\351\207\217\346\246\202\350\247\210", 0));
        totalLabel->setText(QApplication::translate("setup", "\346\200\273\345\256\271\351\207\217 --  |  \345\267\262\347\224\250 --  |  \345\211\251\344\275\231 -- (0%)", 0));
        storageBarBg->setText(QString());
        videoSeg->setText(QString());
        photoSeg->setText(QString());
        otherSeg->setText(QString());
        freeSeg->setText(QString());
        legendVideoDot->setText(QString());
        legendVideo->setText(QApplication::translate("setup", "\345\275\225\345\203\217 --", 0));
        legendPhotoDot->setText(QString());
        legendPhoto->setText(QApplication::translate("setup", "\345\233\276\347\211\207 --", 0));
        legendOtherDot->setText(QString());
        legendOther->setText(QApplication::translate("setup", "\345\205\266\344\273\226 --", 0));
        legendFreeDot->setText(QString());
        legendFree->setText(QApplication::translate("setup", "\347\251\272\351\227\262 --", 0));
        estimateLabel->setText(QApplication::translate("setup", "\351\242\204\350\256\241\350\277\230\345\217\257\345\255\230\345\202\250\347\272\246--\345\244\251", 0));
        pathTitle->setText(QApplication::translate("setup", "\345\255\230\345\202\250\350\267\257\345\276\204", 0));
        pathLabel->setText(QApplication::translate("setup", "\345\255\230\345\202\250\350\267\257\345\276\204", 0));
        browseBt->setText(QApplication::translate("setup", "\342\200\246", 0));
        autoTitle->setText(QApplication::translate("setup", "\350\207\252\345\212\250\345\275\225\345\203\217\344\270\216\346\270\205\347\220\206\347\255\226\347\225\245", 0));
        autoLabel->setText(QApplication::translate("setup", "\350\207\252\345\212\250\345\275\225\345\203\217", 0));
        autoToggle->setText(QString());
        retainLabel->setText(QApplication::translate("setup", "\344\277\235\347\225\231\345\244\251\346\225\260", 0));
        retainCombo->clear();
        retainCombo->insertItems(0, QStringList()
         << QApplication::translate("setup", "7\345\244\251", 0)
         << QApplication::translate("setup", "15\345\244\251", 0)
         << QApplication::translate("setup", "30\345\244\251", 0)
         << QApplication::translate("setup", "60\345\244\251", 0)
         << QApplication::translate("setup", "90\345\244\251", 0)
        );
        policyLabel->setText(QApplication::translate("setup", "\347\243\201\347\233\230\346\273\241\347\255\226\347\225\245", 0));
        policyCombo->clear();
        policyCombo->insertItems(0, QStringList()
         << QApplication::translate("setup", "\350\246\206\347\233\226\346\234\200\346\227\251\345\275\225\345\203\217", 0)
         << QApplication::translate("setup", "\345\201\234\346\255\242\345\275\225\345\203\217", 0)
        );
        resetBtRec->setText(QApplication::translate("setup", "  \346\201\242\345\244\215\351\273\230\350\256\244", 0));
        saveBtRec->setText(QApplication::translate("setup", "  \344\277\235\345\255\230\350\256\276\347\275\256", 0));
        netPlaceholder->setText(QApplication::translate("setup", "\347\275\221\347\273\234\344\270\216\350\256\276\345\244\207\347\256\241\347\220\206", 0));
        userPlaceholder->setText(QApplication::translate("setup", "\347\224\250\346\210\267\346\235\203\351\231\220\347\256\241\347\220\206", 0));
    } // retranslateUi

};

namespace Ui {
    class setup: public Ui_setup {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETUP_H
