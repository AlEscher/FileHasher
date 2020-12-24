#ifndef FILEHASHER_H
#define FILEHASHER_H

#include <QMainWindow>
#include <QThread>
#include "../../Hashing/HashingAlgorithm.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FileHasher; class Worker; class Controller; }
QT_END_NAMESPACE

class FileHasherDelegate;
class QTableWidget;
class Controller;
class HashingAlgorithm;

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

    void on_clearOutputButton_clicked();

private:
    Ui::FileHasher *ui;
    FileHasherDelegate *delegate;
    Controller* controller;
    size_t totalFileSize = 0;

    void AddFileToTable(QTableWidget* table, const QString& fileName, const QString& filePath, const size_t fileSize);
};

class Worker : public QObject
{
    Q_OBJECT
    public:
        Worker(Ui::FileHasher* ui, FileHasherDelegate* delegate);
    private:
        Ui::FileHasher* ui;
        FileHasherDelegate* delegate;
    public slots:
        // This function will get called by the Controller
        void DoWork(const std::vector<HashingAlgorithm*> &hashAlgorithms, const std::vector<QStringList> &parameters);

    signals:
        void resultReady(const QStringList &result);
};

class Controller : public QObject
{
    Q_OBJECT
    QThread workerThread;
    public:
        Controller(Ui::FileHasher* ui, FileHasherDelegate* delegate);
        ~Controller();
    private:
        Ui::FileHasher* ui;
    public slots:
        // This function will be called when the Worker thread wants to pass on his results,
        // most importantly this function will be executed in our main thread, meaning we can do send events
        void HandleResults(const QStringList &);
    signals:
        void operate(const std::vector<HashingAlgorithm*> &, const std::vector<QStringList> &);
};

#endif // FILEHASHER_H
