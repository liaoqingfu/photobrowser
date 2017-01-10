#include "qshowvieowidget.h"
#include <videoutil.h>

QShowVieoWidget::QShowVieoWidget(QWidget *parent,QString videopath) :
    QWidget(parent)
{
    this->resize(640,480);

    playWidget = new CPlayWidget(this);
    playWidget->resize(640,480);
    playWidget->move(0,0);

    OVideoDec = new VideoDec(NULL,videopath);
    connect(OVideoDec,SIGNAL(sendEndFlag(PlayState)),this,SIGNAL(sendEndFlag(PlayState)));
    connect(OVideoDec,SIGNAL(sendFrame(unsigned char*)),this,SLOT(updateVideoFrame(unsigned char*)));
    OVideoDec->start();
}


QShowVieoWidget::~QShowVieoWidget()
{
    if(OVideoDec->isRunning())
    {
        OVideoDec->terminate();
    }
    delete OVideoDec;
    delete playWidget;
}


void QShowVieoWidget ::updateVideoFrame(uint8_t *yuv)
{
    playWidget->PlayOneFrame(yuv);
}
