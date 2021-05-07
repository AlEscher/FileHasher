#include "filehasher.h"
#include "ui_filehasher.h"
#include "FileHasherDelegate.h"
#include "Controller.h"
#include "../../Hashing/SHA256Hash.h"
#include "../../Hashing/SHA512Hash.h"
#include "../../Hashing/MD5Hash.h"
#include "../../Hashing/SHA1Hash.h"
#include "../../Utility/FileUtility.h"
#include "exportdialog.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QMenu>
#include <QTime>
#include <QDesktopServices>
#include <QCloseEvent>
#include <QMimeData>

using namespace std;

FileHasher::FileHasher(QWidget *parent) : QMainWindow(parent), ui(new Ui::FileHasher)
{
    ui->setupUi(this);
    ui->fileTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    delegate = new FileHasherDelegate();
    controller = new Controller(ui, delegate);
    m_pExportDialog = new ExportDialog(this, delegate);
    m_exportWindow.setContent(m_pExportDialog);
    m_exportWindow.setWindowTitle("Exported Data");

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
    delete m_pActionsMenu;
    delete m_pExportDialog;
}

void FileHasher::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls())
    {
        e->accept();
    }
}

void FileHasher::dragMoveEvent(QDragMoveEvent *e)
{
    // Only allow drops inside the file table
    QRect fileTableArea = ui->hashTargetsWidget->geometry();
    if (fileTableArea.contains(e->pos()))
    {
        e->acceptProposedAction();
    }
    else
    {
        e->ignore();
    }
}

void FileHasher::dropEvent(QDropEvent *e)
{
    const QMimeData* data = e->mimeData();
    if (data->hasUrls())
    {
        for (const QUrl& url : data->urls())
        {
            const QString filePath = url.toLocalFile();
            // Don't add folders
            if (delegate->CheckFilePath(filePath))
            {
                this->AddFileToTable(ui->fileTable, url.fileName(), filePath, delegate->GetFileSize(filePath));
            }
        }
    }
}

void FileHasher::closeEvent(QCloseEvent *event)
{
    m_exportWindow.close();
    event->accept();
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
    m_pActionsMenu = new QMenu;
    // Create and assign Actions to menu, menu will take ownership of these
    QAction* clearAction = m_pActionsMenu->addAction("Clear Output");
    QAction* exportJsonAction = m_pActionsMenu->addAction("Export as JSON");
    QAction* exportArrayAction = m_pActionsMenu->addAction("Export as C-Array");

    connect(clearAction, &QAction::triggered, this, &FileHasher::ClearOutputBox);
    connect(exportJsonAction, &QAction::triggered, this, &FileHasher::ExportJsonToClipboard);
    connect(exportArrayAction, &QAction::triggered, this, &FileHasher::ExportArrayToClipboard);

    ui->actionsButton->setMenu(m_pActionsMenu);
    ui->actionsButton->setDefaultAction(exportJsonAction);
    ui->actionsButton->setToolTip("Exported data will always be pasted into your clipboard");
    ui->actionsButton->setToolTipDuration(0);
    // Let the last chosen action be the currently displayed action
    connect(ui->actionsButton, &QToolButton::triggered, ui->actionsButton, &QToolButton::setDefaultAction);
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
    // Clear file table
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

    if (ui->md5CB->isChecked())
    {
        hashAlgoVec.push_back(new MD5Hasher());
    }
    if (ui->sha1CB->isChecked())
    {
        hashAlgoVec.push_back(new SHA1Hasher());
    }
    if (ui->sha256CB->isChecked())
    {
        hashAlgoVec.push_back(new SHA256Hasher());
    }
    if (ui->sha512CB->isChecked())
    {
        hashAlgoVec.push_back(new SHA512Hasher());
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
        int progressBarMax = std::max((int)(m_nTotalFileSize * hashAlgoVec.size()), 1);
        // Set total-progress bar information
        ui->totalProgressBar->setRange(0, progressBarMax);
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

void FileHasher::on_fileTable_customContextMenuRequested(const QPoint& pos)
{
    QMenu menu(this);

    // Menu takes ownership of this action, which will be freed once the function exits
    QAction* removeAction = menu.addAction("Remove Row");
    // Wait for an action to be selected or the menu to be closed
    QAction* selectedAction = menu.exec(ui->fileTable->viewport()->mapToGlobal(pos));
    // Execute the selected action, currently only remove
    if (selectedAction == removeAction)
    {
        QTableWidget* table = ui->fileTable;
        if (table->rowCount() > 0)
        {
            m_nTotalFileSize -= delegate->GetSizeFromString(table->item(table->currentRow(), 2)->text());
            table->removeRow(table->currentRow());
        }
    }
}

void FileHasher::on_outputList_customContextMenuRequested(const QPoint &pos)
{
    if (ui->outputList->count() < 1)
        return;

    QMenu menu(this);

    QAction* copyCell = menu.addAction("Copy Hash");
    QAction* compareCell = menu.addAction("Compare to clipboard");
    QAction* lookupCell = menu.addAction("Lookup on Virustotal");
    QAction* selectedAction = menu.exec(ui->outputList->viewport()->mapToGlobal(pos));

    QListWidgetItem* selectedItem = ui->outputList->currentItem();
    if (!selectedItem)
    {
        return;
    }

    if (selectedAction == copyCell)
    {
        // Items are structured like so: TIMESTAMP FILENAME HASHALGO: HASH
        QString hash = delegate->GetHashFromString(selectedItem->text());
        delegate->SetClipboardText(hash);
    }
    else if (selectedAction == compareCell)
    {
        QString hash = delegate->GetHashFromString(selectedItem->text());
        QString clipboardText = delegate->GetClipboardText();
        if (QString::compare(hash, clipboardText, Qt::CaseInsensitive) == 0)
        {
            QMessageBox::information(this, "Hashes match", "The selected hash and the one saved in your clipboard match.");
        }
        else
        {
            QString message = QString("The selected computed hash:\n\"%1\"\nand the hash in your clipboard:\n\"%2\"\ndon't match")
                    .arg(hash, clipboardText);
            QMessageBox::warning(this, "Hashes don't match", message);
        }
    }
    else if (selectedAction == lookupCell)
    {
        QString hash = delegate->GetHashFromString(selectedItem->text());
        QUrl url = QUrl(QString("https://www.virustotal.com/gui/file/%1/detection").arg(hash));
        QDesktopServices::openUrl(url);
    }
}

void FileHasher::ClearOutputBox()
{
    ui->outputList->clear();
    controller->ClearCache();
}

void FileHasher::ExportJsonToClipboard()
{
    ShowExportDialog(controller->GetCacheContentsAsJson(), true);
}

void FileHasher::ExportArrayToClipboard()
{
    ShowExportDialog(controller->GetCacheContentAsArray(), false);
}

void FileHasher::ShowExportDialog(QString data, bool isJson)
{
    m_pExportDialog->SetTextBoxContent(data, isJson);
    m_exportWindow.show();
}
