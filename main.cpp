#include "qimagebrowser.h"
#include <QFontDatabase>
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    //QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);
    //QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(codec);
    QFontDatabase::addApplicationFont("/usr/lib/fonts/wqy/wqyzh.ttf");
    QFont font;
    font.setPixelSize(30);
    font.setFamily("WenQuanYi Zen Hei");
    font.setBold(false);
    a.setFont(font);
    QImageBrowser w;
    w.show();

    return a.exec();
}
