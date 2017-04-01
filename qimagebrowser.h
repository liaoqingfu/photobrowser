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
#include <photobrowser/qimagelistwidget.h>
#include <QScrollBar>
#include <photobrowser/videoutil.h>
#include <photobrowser/qiconwidget.h>
#include <QMutex>
#include <keyboardform.h>
#include <qreadwritefile.h>
#include <photobrowser/qrenamedialog.h>
#include <photobrowser/qdeletedialog.h>
#include <photobrowser/qcreatedirdialog.h>
#include "photobrowser/qcustommenudialog.h"
#include "photobrowser/qshowimagewidget.h"


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

signals:
    //发送按键
    void sendKey(int);

    void closeWidget();

public:

protected:
    void closeEvent(QCloseEvent *);

public slots:
    void getKeyEvent(int key);

    //开启菜单
    void openCustomMenu(QImageListWidget *list);
    //菜单栏操作
    void optImageListFile(int mode ,QListWidget *list);

    //图片点击
    void yearListClicked(QListWidgetItem *yearItem);
    void picListCLicked(QListWidgetItem *picItem);

    //显示图片浏览
    void showImage(int num);

    void closeCustomMenu();

    void closeOptCreateMenu();

    void closeOptDeleteMenu();

    void closeOptRenameMenu();

    void closeImageBrowser();

    void closeTabListWidget();

    void findSdDir();

protected slots:

private:
    QRenameDialog * rdlg ;
    QDeleteDialog  *ddlg;
    QCreateDirDialog *cdlg;
    QShowImageWidget *displaydlg;
    QCustomMenuDialog *dlg;


    QLabel *tiltlelabel;
    QTabWidget *tabWidget;
    //目录和文件列表
    QImageListWidget *yearListWidget;
    QImageListWidget *picListWidget;

    //路径
    QString rootPath;
    QString secondPath;

    //线程监控
    QFutureWatcher<QImage> *imageWatcher;

    QTimer *timer;

    bool isOpenMenu;
    bool isOptMenu;
    bool isOpenImage;

};

#endif // QIMAGEBROWSER_H
