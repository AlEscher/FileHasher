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
    FramelessWindow framelessWindow = FramelessWindow(Q_NULLPTR, true);
    framelessWindow.setContent(w);
    framelessWindow.setWindowTitle("FileHasher UI");
    framelessWindow.setWindowIcon(QIcon(":./appico.ico"));
    framelessWindow.show();
    // Lock the size in (Size of FramelessWindow is determined by its content)
    framelessWindow.setFixedSize(framelessWindow.size());

    return a.exec();
}
