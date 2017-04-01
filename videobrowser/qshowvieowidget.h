#ifndef QSHOWVIEOWIDGET_H
#define QSHOWVIEOWIDGET_H

#include <QWidget>
#include <photobrowser/videobrowser/qplaywidget.h>

#define VGA_WIDTH 640
#define VGA_HEIGHT 480

class QShowVieoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QShowVieoWidget(QWidget *parent = 0);
    ~QShowVieoWidget();
public:
    void showFrame(unsigned char *yuv);

private:

    QPlayWidget *playWidget;

};

#endif // QSHOWVIEOWIDGET_H
