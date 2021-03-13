/********************************************************************************
** Form generated from reading UI file 'exportdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXPORTDIALOG_H
#define UI_EXPORTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_exportDialog
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *copyClipboardBtn;
    QPushButton *saveAsBtn;
    QTextEdit *exportedText;

    void setupUi(QDialog *exportDialog)
    {
        if (exportDialog->objectName().isEmpty())
            exportDialog->setObjectName(QString::fromUtf8("exportDialog"));
        exportDialog->resize(480, 643);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(exportDialog->sizePolicy().hasHeightForWidth());
        exportDialog->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(exportDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        copyClipboardBtn = new QPushButton(exportDialog);
        copyClipboardBtn->setObjectName(QString::fromUtf8("copyClipboardBtn"));

        horizontalLayout->addWidget(copyClipboardBtn);

        saveAsBtn = new QPushButton(exportDialog);
        saveAsBtn->setObjectName(QString::fromUtf8("saveAsBtn"));

        horizontalLayout->addWidget(saveAsBtn);


        verticalLayout->addLayout(horizontalLayout);

        exportedText = new QTextEdit(exportDialog);
        exportedText->setObjectName(QString::fromUtf8("exportedText"));

        verticalLayout->addWidget(exportedText);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(exportDialog);

        QMetaObject::connectSlotsByName(exportDialog);
    } // setupUi

    void retranslateUi(QDialog *exportDialog)
    {
        exportDialog->setWindowTitle(QCoreApplication::translate("exportDialog", "Dialog", nullptr));
        copyClipboardBtn->setText(QCoreApplication::translate("exportDialog", "Copy to Clipboard", nullptr));
        saveAsBtn->setText(QCoreApplication::translate("exportDialog", "Save as", nullptr));
    } // retranslateUi

};

namespace Ui {
    class exportDialog: public Ui_exportDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXPORTDIALOG_H
