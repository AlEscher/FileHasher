#include "Controller.h"
#include "ui_filehasher.h"
#include "FileHasherDelegate.h"

#include <QTime>
#include <QJsonDocument>

void AddToOutput(QString entry, QListWidget* output)
{
    // Get current time
    QTime time = QTime::currentTime();
    QString timeStr = time.toString("hh:mm:ss");

    QListWidgetItem *newResult = new QListWidgetItem;
    newResult->setText("[" + timeStr + "] " + entry);
    output->addItem(newResult);
}

Controller::Controller(Ui::FileHasher* ui, FileHasherDelegate* delegate)
{
    this->ui = ui;
    this->delegate = delegate;
    this->m_mCache = QJsonObject();
    Worker *worker = new Worker(ui, delegate, this);

    worker->moveToThread(&workerThread);
    // The worker will delete itself once the thread finishes
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &Controller::operate, worker, &Worker::DoWork);
    connect(worker, &Worker::resultReady, this, &Controller::HandleResults);
    // Allows us to send an error message to the Controller instead of a result
    connect(worker, &Worker::ReportError, this, &Controller::HandleError);
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
    if (result.size() < 5)
    {
        SetHashingStatus(false);
        ui->hashButton->setCursor(Qt::ArrowCursor);
        return;
    }

    // We can only send events to the GUI in the main thread,
    // so we need to do this here instead of in the Worker
    QString resultText = result[0] + " " + result[1] + ": " + result[3];
    AddToOutput(resultText, ui->outputList);
    AddToCache(result);

    int value = ui->totalProgressBar->value() + (int)delegate->GetSizeFromString(result[4]);
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
    ui->fileProgressBar->setRange((int)min, (int)max);
    ui->fileProgressBar->setValue((int)std::min(value, max));

    if (value >= max)
    {
        ui->fileProgressBar->setFormat("100%");
    }
    else
    {
        double percentage = static_cast<double>(value) / static_cast<double>(max);
        ui->fileProgressBar->setFormat(QString::number((percentage * 100), 'g', 3)+ "%");
    }
}

QString Controller::GetCacheContentsAsJson()
{
    return QJsonDocument(m_mCache).toJson(QJsonDocument::Indented);
}

QString Controller::GetCacheContentAsArray()
{

}

void Controller::AddToCache(QStringList data)
{
    if (data.size() < 5)
    {
        return;
    }

    QJsonObject entry;
    // Check if an entry for this file already exists, if not create a new one
    if (m_mCache.contains(data[0]))
    {
        entry = m_mCache[data[0]].toObject();
    }
    else
    {
        entry = QJsonObject();
        entry["filePath"] = data[2];
        entry["fileSizeKB"] = (int)delegate->GetSizeFromString(data[4]);
        entry["hashes"] = QJsonObject();
    }

    QJsonObject hashes = entry["hashes"].toObject();
    hashes[data[1]] = data[3];
    entry["hashes"] = hashes;

    m_mCache[data[0]] = entry;
}

void Controller::HandleError(const QStringList& data)
{
    if (data.size() >= 2)
    {
        QString resultText = data[0] + ": " + data[1];
        AddToOutput(resultText, ui->outputList);
    }
}

/* ======================= Worker =======================
 * A Controller may manage many Workers, one Worker will
 * always have one Controller assigned to them
 */

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

        if (!delegate->CheckFilePath(currentParam[1]))
        {
            QStringList errorData;
            errorData.append(currentParam[0] + " " + "N/A");
            errorData.append("ERROR: Check that the file exists / is accessible!");
            emit ReportError(errorData);
            continue;
        }

        for (HashingAlgorithm* hashAlgorithm : hashAlgorithms)
        {
            // Set the progress bar for this file
            size_t currentFileSize = delegate->GetFileSize(currentParam[1]);

            // Reset the HashAlgorithm's state before starting the displayWorker
            delegate->ResetHashingAlgorithm(hashAlgorithm);
            // No need to monitor empty files
            if (currentFileSize > 0)
            {
                emit UpdateFileStatus(0U, currentFileSize, 0U);
                // Tell the display worker to start monitoring the progress of this hash
                emit controller->StartMonitoring(hashAlgorithm, currentFileSize);
            }

            QString hash = delegate->CreateHash(currentParam[1], hashAlgorithm);
            if (hash.compare("") == 0)
            {
                QStringList errorData;
                errorData.append(currentParam[0] + " " + QString::fromStdString(hashAlgorithm->GetName()));
                errorData.append("ERROR: Something went wrong while hashing!");
                emit ReportError(errorData);
                emit WaitForMonitor();
                continue;
            }

            result.clear();
            // Append all values as we got them before starting the hashing,
            // as it may take some time and the user may add / delete files from the list
            // in the meantime.
            // filename
            result.append(currentParam[0]);
            // hashalgorithm
            result.append(QString::fromStdString(hashAlgorithm->GetName()));
            // filepath
            result.append(currentParam[1]);
            // hash
            result.append(hash);
            // fileSize in KB
            result.append(currentParam[2]);
            // Signal to the controller that we have completed one hash and pass him the result
            emit resultReady(result);
            // Send blocking signal to wait for displayWorker
            emit WaitForMonitor();
        }
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
