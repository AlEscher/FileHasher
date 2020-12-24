/********************************************************************************
** Form generated from reading UI file 'filehasher.ui'
**
** Created by: Qt User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILEHASHER_H
#define UI_FILEHASHER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FileHasher
{
public:
    QWidget *centralwidget;
    QTabWidget *hashTargetsWidget;
    QWidget *filesToHashTab;
    QTableWidget *fileTable;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *addFileButton;
    QPushButton *clearListButton;
    QWidget *strToHashTab;
    QTabWidget *hashAlgosWidget;
    QWidget *hashAlgoTab;
    QCheckBox *sha256CB;
    QCheckBox *md5CB;
    QPushButton *hashButton;
    QProgressBar *progressBar;
    QGroupBox *groupBox;
    QTextBrowser *hashOutputBox;
    QPushButton *clearOutputButton;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *FileHasher)
    {
        if (FileHasher->objectName().isEmpty())
            FileHasher->setObjectName(QString::fromUtf8("FileHasher"));
        FileHasher->setEnabled(true);
        FileHasher->resize(898, 578);
        FileHasher->setTabShape(QTabWidget::Rounded);
        centralwidget = new QWidget(FileHasher);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        hashTargetsWidget = new QTabWidget(centralwidget);
        hashTargetsWidget->setObjectName(QString::fromUtf8("hashTargetsWidget"));
        hashTargetsWidget->setEnabled(true);
        hashTargetsWidget->setGeometry(QRect(240, 0, 651, 311));
        hashTargetsWidget->setAutoFillBackground(false);
        hashTargetsWidget->setMovable(true);
        filesToHashTab = new QWidget();
        filesToHashTab->setObjectName(QString::fromUtf8("filesToHashTab"));
        fileTable = new QTableWidget(filesToHashTab);
        if (fileTable->columnCount() < 3)
            fileTable->setColumnCount(3);
        QFont font;
        font.setBold(true);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setFont(font);
        fileTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        __qtablewidgetitem1->setFont(font);
        fileTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        __qtablewidgetitem2->setFont(font);
        fileTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        fileTable->setObjectName(QString::fromUtf8("fileTable"));
        fileTable->setGeometry(QRect(9, 9, 631, 241));
        fileTable->setAcceptDrops(true);
        fileTable->setLineWidth(1);
        fileTable->setShowGrid(false);
        fileTable->horizontalHeader()->setCascadingSectionResizes(false);
        fileTable->horizontalHeader()->setStretchLastSection(false);
        fileTable->verticalHeader()->setStretchLastSection(false);
        layoutWidget = new QWidget(filesToHashTab);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(9, 250, 631, 23));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        addFileButton = new QPushButton(layoutWidget);
        addFileButton->setObjectName(QString::fromUtf8("addFileButton"));

        horizontalLayout->addWidget(addFileButton);

        clearListButton = new QPushButton(layoutWidget);
        clearListButton->setObjectName(QString::fromUtf8("clearListButton"));

        horizontalLayout->addWidget(clearListButton);

        hashTargetsWidget->addTab(filesToHashTab, QString());
        strToHashTab = new QWidget();
        strToHashTab->setObjectName(QString::fromUtf8("strToHashTab"));
        strToHashTab->setEnabled(true);
        hashTargetsWidget->addTab(strToHashTab, QString());
        hashAlgosWidget = new QTabWidget(centralwidget);
        hashAlgosWidget->setObjectName(QString::fromUtf8("hashAlgosWidget"));
        hashAlgosWidget->setEnabled(true);
        hashAlgosWidget->setGeometry(QRect(10, 0, 221, 311));
        hashAlgoTab = new QWidget();
        hashAlgoTab->setObjectName(QString::fromUtf8("hashAlgoTab"));
        sha256CB = new QCheckBox(hashAlgoTab);
        sha256CB->setObjectName(QString::fromUtf8("sha256CB"));
        sha256CB->setGeometry(QRect(10, 10, 131, 19));
        md5CB = new QCheckBox(hashAlgoTab);
        md5CB->setObjectName(QString::fromUtf8("md5CB"));
        md5CB->setGeometry(QRect(10, 30, 101, 19));
        md5CB->setCursor(QCursor(Qt::ForbiddenCursor));
        md5CB->setCheckable(false);
        hashButton = new QPushButton(hashAlgoTab);
        hashButton->setObjectName(QString::fromUtf8("hashButton"));
        hashButton->setGeometry(QRect(10, 250, 201, 21));
        progressBar = new QProgressBar(hashAlgoTab);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(10, 220, 201, 21));
        progressBar->setCursor(QCursor(Qt::BusyCursor));
        progressBar->setValue(40);
        progressBar->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        progressBar->setTextVisible(true);
        progressBar->setInvertedAppearance(false);
        hashAlgosWidget->addTab(hashAlgoTab, QString());
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 310, 881, 231));
        groupBox->setAutoFillBackground(true);
        hashOutputBox = new QTextBrowser(groupBox);
        hashOutputBox->setObjectName(QString::fromUtf8("hashOutputBox"));
        hashOutputBox->setGeometry(QRect(10, 30, 861, 192));
        hashOutputBox->viewport()->setProperty("cursor", QVariant(QCursor(Qt::IBeamCursor)));
        clearOutputButton = new QPushButton(groupBox);
        clearOutputButton->setObjectName(QString::fromUtf8("clearOutputButton"));
        clearOutputButton->setGeometry(QRect(790, 10, 80, 21));
        FileHasher->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(FileHasher);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        FileHasher->setStatusBar(statusbar);
        toolBar = new QToolBar(FileHasher);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        FileHasher->addToolBar(Qt::TopToolBarArea, toolBar);

        retranslateUi(FileHasher);

        hashTargetsWidget->setCurrentIndex(0);
        hashAlgosWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(FileHasher);
    } // setupUi

    void retranslateUi(QMainWindow *FileHasher)
    {
        FileHasher->setWindowTitle(QCoreApplication::translate("FileHasher", "FileHasher", nullptr));
        QTableWidgetItem *___qtablewidgetitem = fileTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("FileHasher", "Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = fileTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("FileHasher", "Path", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = fileTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("FileHasher", "Size", nullptr));
        addFileButton->setText(QCoreApplication::translate("FileHasher", "Add File", nullptr));
        clearListButton->setText(QCoreApplication::translate("FileHasher", "Clear List", nullptr));
        hashTargetsWidget->setTabText(hashTargetsWidget->indexOf(filesToHashTab), QCoreApplication::translate("FileHasher", "Files to Hash", nullptr));
        hashTargetsWidget->setTabText(hashTargetsWidget->indexOf(strToHashTab), QCoreApplication::translate("FileHasher", "Strings to Hash", nullptr));
        sha256CB->setText(QCoreApplication::translate("FileHasher", "SHA256", nullptr));
        md5CB->setText(QCoreApplication::translate("FileHasher", "MD5", nullptr));
        hashButton->setText(QCoreApplication::translate("FileHasher", "Generate Hashes", nullptr));
        hashAlgosWidget->setTabText(hashAlgosWidget->indexOf(hashAlgoTab), QCoreApplication::translate("FileHasher", "Hash Algorithms", nullptr));
        groupBox->setTitle(QCoreApplication::translate("FileHasher", "Output", nullptr));
        hashOutputBox->setHtml(QCoreApplication::translate("FileHasher", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">MD5: Blablabla</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">SHA256: Blaaaa</p></body></html>", nullptr));
        clearOutputButton->setText(QCoreApplication::translate("FileHasher", "Clear Output", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("FileHasher", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FileHasher: public Ui_FileHasher {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILEHASHER_H
