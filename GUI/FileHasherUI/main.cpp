#include "filehasher.h"
#include "framelesswindow.h"
#include "Style/DarkStyle.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle(new DarkStyle);
    QApplication::setPalette(QApplication::style()->standardPalette());
    FileHasher* w = new FileHasher;
    FramelessWindow framelessWindow;
    framelessWindow.setContent(w);
    framelessWindow.setWindowTitle("FileHasher UI");
    framelessWindow.show();

    return a.exec();
}
