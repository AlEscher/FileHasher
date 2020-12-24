#include "filehasher.h"
#include "ui_filehasher.h"
#include "filehasher_delegate.h"
#include "../../Hashing/SHA256Hash.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTableWidgetItem>

using namespace std;

FileHasher::FileHasher(QWidget *parent) : QMainWindow(parent), ui(new Ui::FileHasher)
{
    ui->setupUi(this);
    ui->fileTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    delegate = new FileHasherDelegate();
    setFixedSize(size());
}

FileHasher::~FileHasher()
{
    delete ui;
    delete delegate;
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
    // Make cells non-editable
    fileNameCell->setFlags(fileNameCell->flags() ^ Qt::ItemIsEditable);
    filePathCell->setFlags(filePathCell->flags() ^ Qt::ItemIsEditable);
    fileSizeCell->setFlags(fileSizeCell->flags() ^ Qt::ItemIsEditable);
    table->setItem(row, 0, fileNameCell);
    table->setItem(row, 1, filePathCell);
    table->setItem(row, 2, fileSizeCell);
}

void FileHasher::on_clearListButton_clicked()
{
    ui->fileTable->setRowCount(0);
}

void FileHasher::on_hashButton_clicked()
{
    vector<HashingAlgorithm*> hashAlgoVec;
    vector<QString> hashVector;
    if (ui->sha256CB->isChecked())
    {
        hashAlgoVec.push_back(new SHA256Hasher());
    }

    if (hashAlgoVec.empty())
    {
        QMessageBox::warning(this, "Warning", "Select a Hash Algorithm!");
        return;
    }

    QTableWidget* table = ui->fileTable;
    if (table->rowCount() > 0)
    {
        for (int row = 0; row < table->rowCount(); row++)
        {
            QString fileName = table->item(row, 0)->text();
            QString filePath = table->item(row, 1)->text();
            QString hash = delegate->CreateHash(filePath, hashAlgoVec[0]);
            ui->hashOutputBox->append(fileName + ": " + hash);
        }
    }
    else
    {
        QMessageBox::warning(this, "Warning", "No files selected!");
    }

    for (HashingAlgorithm* hashAlgo : hashAlgoVec)
    {
        delete hashAlgo;
    }
}

void FileHasher::on_clearOutputButton_clicked()
{
    ui->hashOutputBox->clear();
}
