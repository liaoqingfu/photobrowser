#ifndef QSHOWIMAGEWIDGET_H
#define QSHOWIMAGEWIDGET_H

#include <QWidget>
#include <QMutex>
#include <photobrowser/qimagelistwidget.h>
#include <QPainter>
#include <QPixmap>
#include <photobrowser/videoutil.h>

#include "qcustommenudialog.h"
#include "qrenamedialog.h"
#include "qdeletedialog.h"
#include "videobrowser/qvideobrowser.h"

extern QMutex mutex;
class QShowImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QShowImageWidget(QWidget *parent = 0,QImageListWidget *list = 0);
    ~QShowImageWidget();

signals:
    void closeWidget();
    void sendKey(int);

public slots:
    //菜单操作
    void optImageListFile(int,QListWidget*);

    //改变图像名称
    void changeImageName();

    //切换图像模式
    void switchImageMode();

    void getKeyEvent(int key);

    void closeCustomMenu();

    void closeOptDeleteMenu();

    void closeOptRenameMenu();

    void closeVideoBrowser();

protected:
    //键盘事件
    void keyPressEvent(QKeyEvent *e);

    //画布事件
    void paintEvent(QPaintEvent *e);

    void closeEvent(QCloseEvent *);

    void moveLeft();

    void moveRight();


private:
    QCustomMenuDialog *dlg ;
    QRenameDialog * rdlg ;
    QDeleteDialog  *ddlg;

    QVideoBrowser *videobroswer ;

    QLabel *rolabel;

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

    bool isOpenVideo;
    bool isOpenMenu;
    bool isOptMenu;

    int frame_width;
    int frame_height;
};

#endif // QSHOWIMAGEWIDGET_H
