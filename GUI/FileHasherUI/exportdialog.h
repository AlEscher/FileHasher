#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class exportDialog; }
QT_END_NAMESPACE

class FileHasherDelegate;
class QTimer;

class ExportDialog : public QDialog
{
    Q_OBJECT
private:
    Ui::exportDialog* ui;
    FileHasherDelegate* m_pDelegate;
    QTimer* m_pTimer;
    bool m_bIsJson = false;

public:
    ExportDialog(QWidget *parent = nullptr, FileHasherDelegate* delegate = nullptr);
    ~ExportDialog();
    // Set the content of the text box, adds markdown syntax if necessary
    void SetTextBoxContent(QString content, bool isJson);
    void ResetBtnName();
private slots:
    void on_copyClipboardBtn_clicked();
    void on_saveAsBtn_clicked();
};

#endif // EXPORTDIALOG_H
