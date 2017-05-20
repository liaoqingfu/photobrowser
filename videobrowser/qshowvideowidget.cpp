#include "qshowvideowidget.h"

QShowVideoWidget::QShowVideoWidget(QWidget *parent) :
    QWidget(parent)
{
    this->resize(VGA_WIDTH,VGA_HEIGHT);

    //opengl窗体显示
    playWidget = new QPlayWidget(this,VGA_WIDTH,VGA_HEIGHT);
    playWidget->resize(VGA_WIDTH,VGA_HEIGHT);
    playWidget->move(0,0);

}


QShowVideoWidget::~QShowVideoWidget()
{
}


//显示视频帧
void QShowVideoWidget::showFrame(unsigned char *yuv)
{
    playWidget->PlayOneFrame(yuv);
}
