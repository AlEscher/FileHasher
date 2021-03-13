#ifndef FILEHASHER_H
#define FILEHASHER_H

#include <QMainWindow>
#include <QThread>
#include <QString>

#include "../../Hashing/HashingAlgorithm.h"
#include "Style/framelesswindow/framelesswindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FileHasher; }
QT_END_NAMESPACE

class FileHasherDelegate;
class QTableWidget;
class Controller;
class HashingAlgorithm;
class ExportDialog;

class FileHasher : public QMainWindow
{
    Q_OBJECT

public:
    FileHasher(QWidget *parent = nullptr);
    ~FileHasher();

private slots:
    void on_addFileButton_clicked();

    void on_clearListButton_clicked();

    void on_hashButton_clicked();

    void on_fileTable_customContextMenuRequested(const QPoint &pos);

    void on_outputList_customContextMenuRequested(const QPoint &pos);

public slots:
    void ClearOutputBox();
    void ExportJsonToClipboard();
    void ExportArrayToClipboard();
    void ShowExportDialog(QString data, bool isJson);

private:
    Ui::FileHasher *ui;
    FileHasherDelegate *delegate;
    Controller* controller;
    QMenu* m_pActionsMenu;
    ExportDialog* m_pExportDialog;
    FramelessWindow m_exportWindow;
    size_t m_nTotalFileSize = 0;

    void AddFileToTable(QTableWidget* table, const QString& fileName, const QString& filePath, const size_t fileSize);
    void PopulateToolButton();
};

#endif // FILEHASHER_H
