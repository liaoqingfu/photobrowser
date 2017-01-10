#ifndef QIMAGEBROWSER_H
#define QIMAGEBROWSER_H

#include <QWidget>
#include <QKeyEvent>
#include <QFutureWatcher>
#include <QHBoxLayout>
#include <QImage>
#include <QFuture>
#include <QMap>
#include <QtConcurrent/QtConcurrent>
#include <QtConcurrent/qtconcurrentmapkernel.h>
#include <qimagelistwidget.h>
#include <QScrollBar>
#include <videoutil.h>
#include <qiconwidget.h>
#include <QMutex>
#include <keyboardform.h>
#include <qrenamedialog.h>
#include <qdeletedialog.h>
#include <qcreatedirdialog.h>
#include "qcustommenudialog.h"
#include "qshowimagewidget.h"
#include "serial/master.h"
#include "serial/address.h"

#define MAIN_POWER_KEY 0 //关机
#define MAIN_PHOTO_KEY 1 //拍照
#define MAIN_UP_KEY 4 //上键
#define MAIN_LEFT_KEY 5 //左键
#define MAIN_DOWN_KEY 6 //下键
#define MAIN_RIGHT_KEY 7 //右键
#define MAIN_OK_KEY 8 //中键

#define MAIN_UP_LONG_KEY 12 //上键长按
#define MAIN_LEFT_LONG_KEY 13 //左键长按
#define MAIN_DOWN_LONG_KEY 14 //下键长按
#define MAIN_RIGHT_LONG_KEY 15 //右键长按
#define MAIN_OK_LONG_KEY 16 //中键长按

#define MAIN_NONE_KEY 255 //无按键


extern  QMutex mutex;//生成缩略图线程锁

class QImageBrowser : public QWidget
{
    Q_OBJECT

public:
    explicit QImageBrowser(QWidget *parent = 0);
    ~QImageBrowser();

    //初始化布局
    void initLayout();

    //搜索目录
    void findDirs(QString path,QImageListWidget *list);

    //搜索文件
    void findFiles(QString path);



protected:
    //键盘事件
    void keyPressEvent(QKeyEvent *e);

public slots:
    //开启菜单
    void openCustomMenu(QImageListWidget *list);
    //菜单栏操作
    void optImageListFile(int mode ,QListWidget *list);

    //图片点击
    void yearListClicked(QListWidgetItem *yearItem);
    void monthListClicked(QListWidgetItem *monthItem);
    void dayListClicked(QListWidgetItem *dayItem);
    void picListCLicked(QListWidgetItem *picItem);

    //显示图片浏览
    void showImage(int num);

    //获取按键信息
    void getKeyValues(QString value);

    //获取摇杆使用者
    void getRockerUser(QString user);

    //获取摇杆XY值
    void getRockerXY(QString x,QString y);

signals:
    //发送按键
    void sendKey(int);

protected slots:

private:
    QLabel *tiltlelabel;
    QTabWidget *tabWidget;
    //目录和文件列表
    QImageListWidget *yearListWidget;
    QImageListWidget *monthListWidget;
    QImageListWidget *dayListWidget;
    QImageListWidget *picListWidget;

    //路径
    QString rootPath;
    QString secondPath;
    QString thirdPath;
    QString fourPath;

    //线程监控
    QFutureWatcher<QImage> *imageWatcher;

    Master *serial;
};

#endif // QIMAGEBROWSER_H
