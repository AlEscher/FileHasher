#include "exportdialog.h"
#include "ui_exportdialog.h"
#include "FileHasherDelegate.h"

#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>

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
    const QString content = ui->exportedText->toPlainText();

    QFile newFile = QFile(filePath);
    if (!newFile.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, "Error", "Couldn't create the requested file");
        return;
    }

    newFile.write(content.toUtf8());
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
