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
    controller = new Controller(ui, delegate);
    setFixedSize(size());
}

FileHasher::~FileHasher()
{
    delete ui;
    delete delegate;
    delete controller;
}

void FileHasher::AddFileToTable(QTableWidget* table, const QString& fileName, const QString& filePath, const size_t fileSize)
{
    int row = table->rowCount();
    QString fileSizeStr = QString::number(fileSize / 1000) + " KB";

    table->insertRow(row);
    QTableWidgetItem* fileNameCell = new QTableWidgetItem(fileName);
    QTableWidgetItem* filePathCell = new QTableWidgetItem(filePath);
    QTableWidgetItem* fileSizeCell = new QTableWidgetItem(fileSizeStr);

    // Make cells non-editable
    fileNameCell->setFlags(fileNameCell->flags() ^ Qt::ItemIsEditable);
    filePathCell->setFlags(filePathCell->flags() ^ Qt::ItemIsEditable);
    fileSizeCell->setFlags(fileSizeCell->flags() ^ Qt::ItemIsEditable);

    table->setItem(row, 0, fileNameCell);
    table->setItem(row, 1, filePathCell);
    table->setItem(row, 2, fileSizeCell);

    this->totalFileSize += fileSize / 1000;
}

// Get the file size from a string, for example 123 from "123 KB"
size_t GetSizeFromString(QString string)
{
    QStringList list = string.split(" ");
    if (list.size() > 0)
    {
        return list.at(0).toUInt();
    }
    else
    {
        return 0U;
    }
}

void FileHasher::on_addFileButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open the file");
    QString fileName = "";

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

    AddFileToTable(ui->fileTable, fileName, filePath, delegate->GetFileSize(filePath));
}

void FileHasher::on_clearListButton_clicked()
{
    ui->fileTable->setRowCount(0);
    this->totalFileSize = 0;
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
    int totalFiles = table->rowCount();
    if (totalFiles > 0)
    {
        // Set total-progress bar information
        ui->totalProgressBar->setRange(0U, this->totalFileSize);
        ui->totalProgressBar->setValue(0);
        ui->totalProgressBar->setTextVisible(true);
        ui->totalProgressBar->setFormat("1 / " + QString::number(table->rowCount()));
        // Move Hashing to other thread so that it doesn't impede our GUI's responsiveness.
        // Collect parameters to pass onto Worker thread
        std::vector<QStringList> parameters;
        for (int row = 0; row < totalFiles; row++)
        {
            QStringList currentParam;
            // File name
            currentParam.append(table->item(row, 0)->text());
            // File path
            currentParam.append(table->item(row, 1)->text());
            // File size (as string, e.g. "100 KB")
            currentParam.append(table->item(row, 2)->text());
            parameters.push_back(currentParam);
        }

        // Call to controller who will delegate the call to the Worker thread
        controller->operate(hashAlgoVec, parameters);
    }
    else
    {
        QMessageBox::warning(this, "Warning", "No files selected!");
    }
}

void FileHasher::on_clearOutputButton_clicked()
{
    ui->hashOutputBox->clear();
}

Controller::Controller(Ui::FileHasher* ui, FileHasherDelegate* delegate)
{
    this->ui = ui;
    Worker *worker = new Worker(ui, delegate);
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &Controller::operate, worker, &Worker::DoWork);
    connect(worker, &Worker::resultReady, this, &Controller::HandleResults);
    // Worker thread will stay active and accept input until we close the application
    workerThread.start();
}

Controller::~Controller()
{
    workerThread.quit();
    workerThread.wait();
}

void Controller::HandleResults(const QStringList& result)
{
    if (!(result.size() >= 5))
    {
        return;
    }

    // We can only send events in the main thread,
    // so we need to do this here instead of in the Worker
    ui->hashOutputBox->append(result[0] + ": " + result[1]);

    int value = ui->totalProgressBar->value() + (int)GetSizeFromString(result[2]);
    ui->totalProgressBar->setValue(value);
    ui->totalProgressBar->setFormat(result[4] + " / " + result[3]);
}

Worker::Worker(Ui::FileHasher* ui, FileHasherDelegate* delegate)
{
    this->ui = ui;
    this->delegate = delegate;
}

void Worker::DoWork(const std::vector<HashingAlgorithm*>& hashAlgorithms, const std::vector<QStringList>& parameters)
{
    // This will contain our results to send to the Controller (main thread)
    QStringList result;
    // Parameters contains one QStringList for each file, containing its name, path and size
    int totalFiles = parameters.size();

    for (int row = 0; row < totalFiles; row++)
    {
        result.clear();
        const QStringList& currentParam = parameters[row];
        QString hash = delegate->CreateHash(currentParam[1], hashAlgorithms[0]);
        // Append all values as we got them before starting the hashing,
        // as it may take some time and the user may add / delete files from the list
        // in the meantime
        result.append(currentParam[0]);
        result.append(hash);
        result.append(currentParam[2]);
        result.append(QString::number(totalFiles));
        result.append(QString::number(row + 1));
        // Signal to the controller that we have completed one hash and pass him the result
        emit resultReady(result);
    }

    for (HashingAlgorithm* hashAlgo : hashAlgorithms)
    {
        delete hashAlgo;
    }
}
