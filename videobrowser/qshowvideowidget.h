#ifndef QSHOWVIDEOWIDGET_H
#define QSHOWVIDEOWIDGET_H

#include <QWidget>
#include <photobrowser/videobrowser/qplaywidget.h>

#define VGA_WIDTH 640
#define VGA_HEIGHT 480

class QShowVideoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QShowVideoWidget(QWidget *parent = 0);
    ~QShowVideoWidget();
public:
    void showFrame(unsigned char *yuv);

private:

    QPlayWidget *playWidget;

};

#endif // QSHOWVIDEOWIDGET_H
