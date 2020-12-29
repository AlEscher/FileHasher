/********************************************************************************
** Form generated from reading UI file 'filehasher.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
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
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolButton>
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
    QProgressBar *totalProgressBar;
    QProgressBar *fileProgressBar;
    QGroupBox *groupBox;
    QTextBrowser *hashOutputBox;
    QToolButton *actionsButton;

    void setupUi(QMainWindow *FileHasher)
    {
        if (FileHasher->objectName().isEmpty())
            FileHasher->setObjectName(QString::fromUtf8("FileHasher"));
        FileHasher->setEnabled(true);
        FileHasher->resize(898, 545);
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
        font.setWeight(75);
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
        QFont font1;
        font1.setBold(false);
        font1.setWeight(50);
        hashAlgosWidget->setFont(font1);
        hashAlgosWidget->setTabShape(QTabWidget::Rounded);
        hashAlgoTab = new QWidget();
        hashAlgoTab->setObjectName(QString::fromUtf8("hashAlgoTab"));
        sha256CB = new QCheckBox(hashAlgoTab);
        sha256CB->setObjectName(QString::fromUtf8("sha256CB"));
        sha256CB->setGeometry(QRect(10, 10, 131, 19));
        md5CB = new QCheckBox(hashAlgoTab);
        md5CB->setObjectName(QString::fromUtf8("md5CB"));
        md5CB->setEnabled(false);
        md5CB->setGeometry(QRect(10, 30, 101, 19));
        md5CB->setCursor(QCursor(Qt::ArrowCursor));
        md5CB->setCheckable(false);
        hashButton = new QPushButton(hashAlgoTab);
        hashButton->setObjectName(QString::fromUtf8("hashButton"));
        hashButton->setGeometry(QRect(10, 250, 201, 21));
        totalProgressBar = new QProgressBar(hashAlgoTab);
        totalProgressBar->setObjectName(QString::fromUtf8("totalProgressBar"));
        totalProgressBar->setEnabled(true);
        totalProgressBar->setGeometry(QRect(10, 220, 201, 21));
        totalProgressBar->setCursor(QCursor(Qt::ArrowCursor));
        totalProgressBar->setValue(0);
        totalProgressBar->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        totalProgressBar->setTextVisible(true);
        totalProgressBar->setInvertedAppearance(false);
        fileProgressBar = new QProgressBar(hashAlgoTab);
        fileProgressBar->setObjectName(QString::fromUtf8("fileProgressBar"));
        fileProgressBar->setGeometry(QRect(10, 190, 201, 23));
        fileProgressBar->setValue(0);
        hashAlgosWidget->addTab(hashAlgoTab, QString());
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 310, 881, 231));
        groupBox->setAutoFillBackground(true);
        hashOutputBox = new QTextBrowser(groupBox);
        hashOutputBox->setObjectName(QString::fromUtf8("hashOutputBox"));
        hashOutputBox->setGeometry(QRect(10, 30, 861, 192));
        hashOutputBox->viewport()->setProperty("cursor", QVariant(QCursor(Qt::IBeamCursor)));
        actionsButton = new QToolButton(groupBox);
        actionsButton->setObjectName(QString::fromUtf8("actionsButton"));
        actionsButton->setGeometry(QRect(725, 10, 141, 21));
        actionsButton->setPopupMode(QToolButton::MenuButtonPopup);
        FileHasher->setCentralWidget(centralwidget);

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
        totalProgressBar->setFormat(QCoreApplication::translate("FileHasher", "Idle", nullptr));
        fileProgressBar->setFormat(QCoreApplication::translate("FileHasher", "Idle", nullptr));
        hashAlgosWidget->setTabText(hashAlgosWidget->indexOf(hashAlgoTab), QCoreApplication::translate("FileHasher", "Hash Algorithms", nullptr));
        groupBox->setTitle(QCoreApplication::translate("FileHasher", "Output", nullptr));
        hashOutputBox->setHtml(QCoreApplication::translate("FileHasher", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        actionsButton->setText(QCoreApplication::translate("FileHasher", "Actions", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FileHasher: public Ui_FileHasher {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILEHASHER_H
