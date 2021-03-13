#include "exportdialog.h"
#include "ui_exportdialog.h"
#include "FileHasherDelegate.h"

#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonParseError>

ExportDialog::ExportDialog(QWidget *parent, FileHasherDelegate* delegate) : QDialog(parent), ui(new Ui::exportDialog)
{
    this->m_pDelegate = delegate;
    this->m_pTimer = new QTimer;

    m_pTimer->setInterval(2000);
    m_pTimer->setSingleShot(true);
    m_pTimer->callOnTimeout(this, &ExportDialog::ResetBtnName);
    ui->setupUi(this);
}

ExportDialog::~ExportDialog()
{
    delete ui;
    m_pTimer->stop();
    delete m_pTimer;
}

void ExportDialog::on_copyClipboardBtn_clicked()
{
    if (m_pDelegate->SetClipboardText(ui->exportedText->toPlainText()))
    {
        ui->copyClipboardBtn->setText("Copied!");
        m_pTimer->start();
    }
}

void ExportDialog::on_saveAsBtn_clicked()
{
    // Set which file type we want to create
    const QString filter = m_bIsJson ? "JSON (*.json)" : "Text files (*.txt)";
    // Get the desired path from the user
    const QString filePath = QFileDialog::getSaveFileName(this, "Save as", "", filter);
    const QByteArray content = ui->exportedText->toPlainText().toUtf8();

    if (m_bIsJson)
    {
        // Check that the JSON is correctly formatted in case the user made any changes manually
        QJsonParseError jsonError;
        QJsonDocument::fromJson(content, &jsonError);
        if (jsonError.error != QJsonParseError::NoError)
        {
            QMessageBox::warning(this, "Invalid JSON", QString("The exported JSON string is invalid\nUndo any manual changes or export it again to fix this warning:\n\"%1\"")
                                 .arg(jsonError.errorString()));
            return;
        }
    }

    QFile newFile = QFile(filePath);
    if (!newFile.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this, "File Error", QString("Couldn't create the requested file:\n\"%1\"").arg(newFile.errorString()));
        return;
    }

    newFile.write(content);
    newFile.close();
}

void ExportDialog::SetTextBoxContent(QString content, bool isJson)
{
    if (isJson)
    {
        content = "```json\n" + content + "\n```";
    }

    m_bIsJson = isJson;
    ui->exportedText->setMarkdown(content);
}

void ExportDialog::ResetBtnName()
{
    ui->copyClipboardBtn->setText("Copy to Clipboard");
}
