/********************************************************************************
** Form generated from reading UI file 'framelesswindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRAMELESSWINDOW_H
#define UI_FRAMELESSWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "windowdragger.h"

QT_BEGIN_NAMESPACE

class Ui_FramelessWindow
{
public:
    QVBoxLayout *verticalLayout_2;
    QWidget *windowFrame;
    QVBoxLayout *verticalLayout;
    WindowDragger *windowTitlebar;
    QHBoxLayout *horizontalLayout;
    QLabel *spacer;
    QLabel *icon;
    QLabel *titleText;
    QToolButton *minimizeButton;
    QToolButton *closeButton;
    QWidget *windowContent;
    QVBoxLayout *verticalLayout_3;

    void setupUi(QWidget *FramelessWindow)
    {
        if (FramelessWindow->objectName().isEmpty())
            FramelessWindow->setObjectName(QString::fromUtf8("FramelessWindow"));
        FramelessWindow->resize(560, 398);
        FramelessWindow->setAutoFillBackground(false);
        verticalLayout_2 = new QVBoxLayout(FramelessWindow);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(5, 5, 5, 5);
        windowFrame = new QWidget(FramelessWindow);
        windowFrame->setObjectName(QString::fromUtf8("windowFrame"));
        windowFrame->setAutoFillBackground(false);
        windowFrame->setStyleSheet(QString::fromUtf8("#windowFrame{border:1px solid palette(highlight); border-radius:5px 5px 5px 5px; background-color:palette(Window);}"));
        verticalLayout = new QVBoxLayout(windowFrame);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(1, 1, 1, 1);
        windowTitlebar = new WindowDragger(windowFrame);
        windowTitlebar->setObjectName(QString::fromUtf8("windowTitlebar"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(windowTitlebar->sizePolicy().hasHeightForWidth());
        windowTitlebar->setSizePolicy(sizePolicy);
        windowTitlebar->setMinimumSize(QSize(0, 0));
        windowTitlebar->setAutoFillBackground(false);
        windowTitlebar->setStyleSheet(QString::fromUtf8("#windowTitlebar{border: 0px none palette(base); border-top-left-radius:5px; border-top-right-radius:5px; background-color:palette(shadow); height:20px;}"));
        horizontalLayout = new QHBoxLayout(windowTitlebar);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        spacer = new QLabel(windowTitlebar);
        spacer->setObjectName(QString::fromUtf8("spacer"));
        spacer->setMinimumSize(QSize(4, 0));
        spacer->setMaximumSize(QSize(4, 16777215));

        horizontalLayout->addWidget(spacer);

        icon = new QLabel(windowTitlebar);
        icon->setObjectName(QString::fromUtf8("icon"));
        icon->setMinimumSize(QSize(16, 16));
        icon->setMaximumSize(QSize(16, 16));
        icon->setContextMenuPolicy(Qt::NoContextMenu);
        icon->setStyleSheet(QString::fromUtf8("#icon {background-color:palette(shadow);}"));

        horizontalLayout->addWidget(icon);

        titleText = new QLabel(windowTitlebar);
        titleText->setObjectName(QString::fromUtf8("titleText"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        titleText->setFont(font);
        titleText->setStyleSheet(QString::fromUtf8("  padding-left:5px;\n"
"  color:rgb(153,153,153);"));
        titleText->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout->addWidget(titleText);

        minimizeButton = new QToolButton(windowTitlebar);
        minimizeButton->setObjectName(QString::fromUtf8("minimizeButton"));
        minimizeButton->setFont(font);
        minimizeButton->setStyleSheet(QString::fromUtf8("#minimizeButton{\n"
"  background-color:none;\n"
"  border:none;\n"
"  width:16px;\n"
"  height:16px;\n"
"  padding:4px;\n"
"  image:url(:/images/icon_window_minimize.png);\n"
"}\n"
"#minimizeButton:hover{\n"
"  background-color:palette(alternate-base);\n"
"}\n"
"#minimizeButton:pressed{\n"
"  background-color:palette(highlight);\n"
"}"));

        horizontalLayout->addWidget(minimizeButton);

        closeButton = new QToolButton(windowTitlebar);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));
        closeButton->setFont(font);
        closeButton->setStyleSheet(QString::fromUtf8("#closeButton{\n"
"  background-color:none;\n"
"  border:none;\n"
"  width:16px;\n"
"  height:16px;\n"
"  padding:4px;\n"
"  image:url(:/images/icon_window_close.png);\n"
"  border-top-right-radius: 5px;\n"
"}\n"
"#closeButton:hover{\n"
"  background-color:palette(alternate-base);\n"
"}\n"
"##closeButton:pressed{\n"
"  background-color:palette(highlight);\n"
"}"));

        horizontalLayout->addWidget(closeButton);

        horizontalLayout->setStretch(2, 1);

        verticalLayout->addWidget(windowTitlebar);

        windowContent = new QWidget(windowFrame);
        windowContent->setObjectName(QString::fromUtf8("windowContent"));
        windowContent->setAutoFillBackground(false);
        windowContent->setStyleSheet(QString::fromUtf8("#windowContent{\n"
"  border: 0px none palette(base);\n"
"  border-radius:0px 0px 5px 5px;\n"
"}"));
        verticalLayout_3 = new QVBoxLayout(windowContent);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);

        verticalLayout->addWidget(windowContent);


        verticalLayout_2->addWidget(windowFrame);


        retranslateUi(FramelessWindow);

        QMetaObject::connectSlotsByName(FramelessWindow);
    } // setupUi

    void retranslateUi(QWidget *FramelessWindow)
    {
        FramelessWindow->setWindowTitle(QString());
        titleText->setText(QCoreApplication::translate("FramelessWindow", "Frameless Window (Dark Style)", nullptr));
        minimizeButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class FramelessWindow: public Ui_FramelessWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRAMELESSWINDOW_H
