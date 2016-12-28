#ifndef QSHOWIMAGEWIDGET_H
#define QSHOWIMAGEWIDGET_H

#include <QWidget>
#include <QMutex>
#include <qimagelistwidget.h>
#include <QPainter>
#include <QPixmap>
#include <videoutil.h>

#include "qcustommenudialog.h"
#include "qrenamedialog.h"
#include "qdeletedialog.h"

#define VGA_WIDTH 640
#define VGA_HEIGHT 480

extern QMutex mutex;
class QShowImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QShowImageWidget(QWidget *parent = 0,QImageListWidget *list = 0);
    ~QShowImageWidget();

public slots:
    void optImageListFile(int,QListWidget*);

    void changeImageName();

    void switchImageMode();


protected:
    void keyPressEvent(QKeyEvent *e);

    void paintEvent(QPaintEvent *e);



private:
    QLabel *numberlabel;
    QLabel *zoomlabel;
    QLabel *filenamelabel;
    QLabel *videologolabel;

    QPixmap backgroundpixmap;

    QImageListWidget *currentlist;

    bool isFilp;
    bool isVideo;

    double zoomnum;

    unsigned char *rgbframe;

};

#endif // QSHOWIMAGEWIDGET_H
