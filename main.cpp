#include "qimagebrowser.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QImageBrowser w;
    w.show();

    return a.exec();
}
