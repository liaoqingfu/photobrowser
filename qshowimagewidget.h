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
#include "videobrowser/qvideobrowser.h"

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
    //菜单操作
    void optImageListFile(int,QListWidget*);

    //改变图像名称
    void changeImageName();

    //切换图像模式
    void switchImageMode();


protected:
    //键盘事件
    void keyPressEvent(QKeyEvent *e);

    //画布事件
    void paintEvent(QPaintEvent *e);


private:
    //图像序号
    QLabel *numberlabel;
    //缩放
    QLabel *zoomlabel;
    //图像名称
    QLabel *filenamelabel;
    //视频logo
    QLabel *videologolabel;
    //背景
    QPixmap backgroundpixmap;
    //当前列表
    QImageListWidget *currentlist;
    //是否翻转
    bool isFilp;
    //是否视频
    bool isVideo;
    //放大倍数
    double zoomnum;
    //一帧
    unsigned char *rgbframe;

};

#endif // QSHOWIMAGEWIDGET_H
