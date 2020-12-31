#include "filehasher.h"
#include "ui_filehasher.h"
#include "FileHasherDelegate.h"
#include "../../Hashing/SHA256Hash.h"
#include "../../Utility/FileUtility.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QMenu>
#include <QClipboard>
#include <QTime>

using namespace std;

FileHasher::FileHasher(QWidget *parent) : QMainWindow(parent), ui(new Ui::FileHasher)
{
    ui->setupUi(this);
    ui->fileTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    delegate = new FileHasherDelegate();
    controller = new Controller(ui, delegate);
    setFixedSize(size());
    PopulateToolButton();

    qRegisterMetaType<std::vector<HashingAlgorithm*>>("std::vector<HashingAlgorithm*>");
    qRegisterMetaType<std::vector<QStringList>>("std::vector<QStringList>");
    qRegisterMetaType<size_t>("size_t");
    qRegisterMetaType<HashingAlgorithm*>("HashingAlgorithm*");
}

FileHasher::~FileHasher()
{
    delete ui;
    delete delegate;
    delete controller;
    delete actionsMenu;
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

    m_nTotalFileSize += fileSize / 1000;
}

void FileHasher::PopulateToolButton()
{
    actionsMenu = new QMenu;
    // Create and assign Actions to menu, menu will take ownership of these
    QAction* clearAction = actionsMenu->addAction("Clear Output");
    QAction* exportAction = actionsMenu->addAction("Export to Clipboard");

    connect(clearAction, &QAction::triggered, this, &FileHasher::ClearOutputBox);
    connect(exportAction, &QAction::triggered, this, &FileHasher::ExportOuputToClipboard);

    ui->actionsButton->setMenu(actionsMenu);
    // Let the last chosen action be the current default
    connect(ui->actionsButton, &QToolButton::triggered, ui->actionsButton, &QToolButton::setDefaultAction);
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
    QString filePath = QFileDialog::getOpenFileName(this, "Select a file to hash");
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
    m_nTotalFileSize = 0;
}

void FileHasher::on_hashButton_clicked()
{
    vector<HashingAlgorithm*> hashAlgoVec;
    vector<QString> hashVector;

    // If we are already hashing, don't accept new jobs
    if (controller->GetHashingStatus())
    {
        return;
    }

    if (ui->sha256CB->isChecked())
    {
        hashAlgoVec.push_back(new SHA256Hasher());
    }

    if (hashAlgoVec.empty())
    {
        QMessageBox::warning(this, "Warning", "Select a Hash Algorithm!");
        return;
    }
    controller->SetHashingStatus(true);
    ui->hashButton->setCursor(Qt::WaitCursor);

    QTableWidget* table = ui->fileTable;
    int totalFiles = table->rowCount();
    if (totalFiles > 0)
    {
        // Set total-progress bar information
        ui->totalProgressBar->setRange(0, m_nTotalFileSize);
        ui->totalProgressBar->setValue(0);
        ui->totalProgressBar->setTextVisible(true);
        ui->totalProgressBar->setFormat("0%");
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
        emit controller->operate(hashAlgoVec, parameters);
    }
    else
    {
        QMessageBox::warning(this, "Warning", "No files selected!");
        controller->SetHashingStatus(false);
        ui->hashButton->setCursor(Qt::ArrowCursor);
        return;
    }
}

void FileHasher::ClearOutputBox()
{
    ui->hashOutputBox->clear();
    controller->ClearCache();
}

void FileHasher::ExportOuputToClipboard()
{
    const QString hashes = "{ " + controller->GetCacheContents() + " }";

    QClipboard* clipboard = QGuiApplication::clipboard();
    if (clipboard)
    {
        clipboard->clear();
        clipboard->setText(hashes, QClipboard::Clipboard);
    }
}

// ========================= Controller / Worker class =========================

Controller::Controller(Ui::FileHasher* ui, FileHasherDelegate* delegate)
{
    this->ui = ui;
    Worker *worker = new Worker(ui, delegate, this);
    worker->moveToThread(&workerThread);
    // The worker will delete itself once the thread finishes
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &Controller::operate, worker, &Worker::DoWork);
    connect(worker, &Worker::resultReady, this, &Controller::HandleResults);
    connect(worker, &Worker::UpdateFileStatus, this, &Controller::UpdateFileProgress);
    // Worker thread will stay active and accept input until we close the application
    workerThread.start(QThread::HighPriority);

    Worker *displayWorker = new Worker(ui, delegate, this);
    displayWorker->moveToThread(&displayThread);
    connect(&displayThread, &QThread::finished, displayWorker, &QObject::deleteLater);
    connect(displayWorker, &Worker::UpdateFileStatus, this, &Controller::UpdateFileProgress);
    connect(this, &Controller::StartMonitoring, displayWorker, &Worker::MonitorProgress);
    // When worker has finished hashing a file, he will send a blocking signal to the displayWorker,
    // which will only return once the displayWorker has safely stopped his monitoring (Checked his loop condition once)
    connect(worker, &Worker::WaitForMonitor, displayWorker, &Worker::TerminateMonitoring, Qt::BlockingQueuedConnection);
    displayThread.start(QThread::LowestPriority);
}

Controller::~Controller()
{
    workerThread.quit();
    displayThread.quit();
    workerThread.wait();
    displayThread.wait();
}

void Controller::HandleResults(const QStringList& result)
{
    if (result.size() < 3)
    {
        SetHashingStatus(false);
        ui->hashButton->setCursor(Qt::ArrowCursor);
        return;
    }

    // Get current time
    QTime time = QTime::currentTime();
    QString timeStr = time.toString("hh:mm:ss");

    // We can only send events to the GUI in the main thread,
    // so we need to do this here instead of in the Worker
    ui->hashOutputBox->append("[" + timeStr + "] " + result[0] + ": " + result[1]);
    AddToCache(result[1]);

    int value = ui->totalProgressBar->value() + (int)GetSizeFromString(result[2]);
    float percentage = (float)value / (float)ui->totalProgressBar->maximum();
    ui->totalProgressBar->setValue(value);
    // Fix rounding error at 100%
    if (value == ui->totalProgressBar->maximum())
    {
        ui->totalProgressBar->setFormat("100%");
    }
    else
    {
        ui->totalProgressBar->setFormat(QString::number((percentage * 100), 'g', 2)+ "%");
    }
}

void Controller::UpdateFileProgress(const size_t min, const size_t max, const size_t value)
{
    ui->fileProgressBar->setRange(min, max);
    ui->fileProgressBar->setValue(std::min(value, max));

    if (value >= max)
    {
        ui->fileProgressBar->setFormat("100%");
    }
    else
    {
        double percentage = (double)value / (double)ui->fileProgressBar->maximum();
        ui->fileProgressBar->setFormat(QString::number((percentage * 100), 'g', 3)+ "%");
    }
}

QString Controller::GetCacheContents()
{
    QString hashes = "";
    for (int i = 0; i < m_vGeneratedHashes.size(); i++)
    {
        hashes += "\"" + m_vGeneratedHashes[i] + "\"" + ((i < m_vGeneratedHashes.size() - 1) ? ", " : "");
    }

    return hashes;
}

Worker::Worker(Ui::FileHasher* ui, FileHasherDelegate* delegate, Controller* controller)
{
    this->ui = ui;
    this->delegate = delegate;
    this->controller = controller;
}

void Worker::DoWork(const std::vector<HashingAlgorithm*>& hashAlgorithms, const std::vector<QStringList>& parameters)
{
    // This will contain our results to send to the Controller (main thread)
    QStringList result;
    // Parameters contains one QStringList for each file, containing its name, path and size
    size_t totalFiles = parameters.size();

    for (size_t row = 0; row < totalFiles; row++)
    {
        const QStringList& currentParam = parameters[row];
        // Set the progress bar for this file
        size_t currentFileSize = delegate->GetFileSize(currentParam[1]);
        // Reset the HashAlgorithm's state before starting the displayWorker
        delegate->ResetHashingAlgorithm(hashAlgorithms[0]);
        emit UpdateFileStatus(0U, currentFileSize, 0U);
        // Tell the display worker to start monitoring the progress of this hash
        emit controller->StartMonitoring(hashAlgorithms[0], currentFileSize);

        QString hash = delegate->CreateHash(currentParam[1], hashAlgorithms[0]);

        result.clear();
        // Append all values as we got them before starting the hashing,
        // as it may take some time and the user may add / delete files from the list
        // in the meantime
        result.append(currentParam[0]);
        result.append(hash);
        result.append(currentParam[2]);
        // Signal to the controller that we have completed one hash and pass him the result
        emit resultReady(result);
        // Send blocking signal to wait for displayWorker
        emit WaitForMonitor();
    }

    // Send empty result list to Controller to signal that we are done hashing
    result.clear();
    emit resultReady(result);

    for (HashingAlgorithm* hashAlgo : hashAlgorithms)
    {
        delete hashAlgo;
    }
}

void Worker::MonitorProgress(const HashingAlgorithm* algorithm, const size_t fileSize)
{
    size_t oldValue = 0U;
    m_bContinue = true;
    while (algorithm->GetBytesProcessed() <= fileSize)
    {
        if (!controller->GetHashingStatus() || !m_bContinue)
        {
            return;
        }

        size_t progress = algorithm->GetBytesProcessed();
        if (progress != oldValue)
        {
            emit UpdateFileStatus(ui->fileProgressBar->minimum(), fileSize, progress);
            oldValue = progress;
        }

        QThread::msleep(10);
    }

    emit UpdateFileStatus(ui->fileProgressBar->minimum(), fileSize, algorithm->GetBytesProcessed());
}

void Worker::TerminateMonitoring()
{
    // This function won't really stop the monitoring process itself,
    // it is only intended to offer a blocking signal for the hashing thread
    m_bContinue = false;
}
