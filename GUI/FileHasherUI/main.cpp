#include "filehasher.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FileHasher w;
    //w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    QFile f(":qdarkstyle/style.qss");

    if (!f.exists())
    {
        return -1;
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        w.setStyleSheet(ts.readAll());
        f.close();
    }
    w.show();
    return a.exec();
}
