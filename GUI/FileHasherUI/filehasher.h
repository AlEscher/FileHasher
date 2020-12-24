#ifndef FILEHASHER_H
#define FILEHASHER_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class FileHasher; }
QT_END_NAMESPACE

class FileHasherDelegate;

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
};
#endif // FILEHASHER_H
