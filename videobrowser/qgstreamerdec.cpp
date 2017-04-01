#include "qgstreamerdec.h"
#include "photobrowser/videobrowser/gstreamerdec.h"

QGstreamerDec::QGstreamerDec(QObject *parent) :
    QThread(parent)
{
    isRun = true;

    //注册类型
    qRegisterMetaType<PlayState >("PlayState");
}

void QGstreamerDec::run()
{
    while (isRun){

        gstmutex.lock();
        qDebug()<<"frame";
        emit sendFrame(rawdata);
    }
    emit updatePlayState(Finish);
}


void QGstreamerDec::startPlay(QString  name)
{
    if(StartPlayGstreamer(name.toLatin1().data()))
    {
        this->start();
    }
}


void QGstreamerDec::stopPlay()
{
    StopGstreamer();

    isRun = false;
}


bool QGstreamerDec::getPauseState()
{
    return IsPauseStatus();
}
