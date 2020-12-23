#include "filehasher.h"
#include "ui_filehasher.h"
#include "filehasher_delegate.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTableWidgetItem>

FileHasher::FileHasher(QWidget *parent) : QMainWindow(parent), ui(new Ui::FileHasher)
{
    ui->setupUi(this);
    ui->fileTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    delegate = new FileHasherDelegate();
}

FileHasher::~FileHasher()
{
    delete ui;
}


void FileHasher::on_addFileButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open the file");
    QString fileName = "";
    QString fileSize = "";

    // Check if the file path is valid
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, "Error", "Cannot open file: " + file.errorString());
        return;
    }
    QFileInfo fileInfo(file.fileName());
    fileName = fileInfo.fileName();
    file.close();
    fileSize = QString::number(delegate->GetFileSize(filePath) / 1000) + " KB";

    QTableWidget* table = ui->fileTable;

    int row = table->rowCount();
    table->insertRow(row);
    QTableWidgetItem* fileNameCell = new QTableWidgetItem(fileName);
    QTableWidgetItem* filePathCell = new QTableWidgetItem(filePath);
    QTableWidgetItem* fileSizeCell = new QTableWidgetItem(fileSize);
    // Make cells non-editablf
    fileNameCell->setFlags(fileNameCell->flags() ^ Qt::ItemIsEditable);
    filePathCell->setFlags(filePathCell->flags() ^ Qt::ItemIsEditable);
    fileSizeCell->setFlags(fileSizeCell->flags() ^ Qt::ItemIsEditable);
    table->setItem(row, 0, fileNameCell);
    table->setItem(row, 1, filePathCell);
    table->setItem(row, 2, fileSizeCell);

    QMessageBox::information(this, "Message", filePath);
}

void FileHasher::on_clearListButton_clicked()
{
    ui->fileTable->setRowCount(0);
}
