#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QMainWindow>
#include <QThread>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>

QT_BEGIN_NAMESPACE
namespace Ui { class FileHasher; }
QT_END_NAMESPACE

class FileHasherDelegate;
class Controller;
class HashingAlgorithm;

class Worker : public QObject
{
    Q_OBJECT
    public:
        Worker(Ui::FileHasher* ui, FileHasherDelegate* delegate, Controller* controller);
    private:
        Ui::FileHasher* ui;
        FileHasherDelegate* delegate;
        Controller* controller;
        bool m_bContinue = false;
    public slots:
        // This function will get called by the Controller to do the hashing (HashWorker)
        void DoWork(const std::vector<HashingAlgorithm*> &hashAlgorithms, const std::vector<QStringList> &parameters);
        // This function will monitor the progress for the current file, aswell as handle some animations (DisplayWorker)
        void MonitorProgress(const HashingAlgorithm* algorithm, const size_t fileSize);
        void TerminateMonitoring();

    signals:
        void resultReady(const QStringList &result);
        void UpdateFileStatus(const size_t fileSize, const size_t value);
        void WaitForMonitor();
        void ReportError(const QStringList &data);
};

class Controller : public QObject
{
    Q_OBJECT
    QThread workerThread;
    QThread displayThread;
    public:
        Controller(Ui::FileHasher* ui, FileHasherDelegate* delegate);
        ~Controller();
        void AddToCache(QStringList data);
        inline void ClearCache()
        {
            m_mCache = QJsonObject();
        }
        // Returns the Cache's contents as a JSON-string
        QString GetCacheContentsAsJson();
        // Returns the Cache's contents formatted as a C-Array
        QString GetCacheContentAsArray();
        void SetHashingStatus(const bool status);
        bool GetHashingStatus() const
        {
            return m_bHashing;
        }
    private:
        Ui::FileHasher* ui;
        bool m_bHashing = false;
        QJsonObject m_mCache;
        FileHasherDelegate* delegate;
    public slots:
        // This function will be called when the Worker thread wants to pass on his results,
        // most importantly this function will be executed in our main thread, meaning we can do send events
        void HandleResults(const QStringList &);
        void UpdateFileProgress(const size_t fileSize, const size_t value);
        void HandleError(const QStringList &data);

    signals:
        void operate(const std::vector<HashingAlgorithm*> &, const std::vector<QStringList> &);
        void StartMonitoring(const HashingAlgorithm* algorithm, const size_t fileSize);
};
#endif // CONTROLLER_H
