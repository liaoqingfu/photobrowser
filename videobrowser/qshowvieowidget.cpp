#include "qshowvieowidget.h"

QShowVieoWidget::QShowVieoWidget(QWidget *parent) :
    QWidget(parent)
{
    this->resize(VGA_WIDTH,VGA_HEIGHT);

    //opengl窗体显示
    playWidget = new QPlayWidget(this,VGA_WIDTH,VGA_HEIGHT);
    playWidget->resize(VGA_WIDTH,VGA_HEIGHT);
    playWidget->move(0,0);

}


QShowVieoWidget::~QShowVieoWidget()
{
}


//显示视频帧
void QShowVieoWidget::showFrame(unsigned char *yuv)
{
    playWidget->PlayOneFrame(yuv);
}
