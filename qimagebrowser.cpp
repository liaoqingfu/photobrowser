#include "qimagebrowser.h"

QMutex mutex;
extern QString defaultDir;
QImageBrowser::QImageBrowser(QWidget *parent) :
    QWidget(parent)
{
    //初始化布局
    this->initLayout();

    //设置TF卡路径
    rootPath = tr("/mnt/sdcard/");

    //线程监控
    imageWatcher = new QFutureWatcher<QImage>(this);
    //图片动态加载
    connect(imageWatcher,SIGNAL(resultReadyAt(int)), this, SLOT(showImage(int)));

    //查找sd卡定时器
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(findSdDir()));
    timer->start(500);
}


//查找SD卡
void QImageBrowser::findSdDir()
{
    //判断是否为空
    QFile devfile("/dev/mmcblk0p1");
    if(devfile.exists())
        this->findDirs(rootPath,yearListWidget);

    timer->stop();
}


//初始化布局
void QImageBrowser::initLayout()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setStyleSheet(tr("QWidget{background-color: rgb(59, 61, 66, 100);}"));

    QFont font;
    font.setPixelSize(30);

    tiltlelabel = new QLabel ;
    tiltlelabel->setFixedHeight(50);
    tiltlelabel->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(68, 119, 180);");
    tiltlelabel->setFont(font);
    tiltlelabel->setText(tr("  图像浏览"));

    tabWidget = new QTabWidget;
    tabWidget->setStyleSheet("QTabWidget::pane{border: 1px;}QTabWidget::tab-bar{alignment:left;}\
                             QTabBar::tab{font-size: 16px;margin-left:1;background:rgb(255, 255, 255, 100); color:black; min-width:10px; min-height:5px;}\
                             QTabBar::tab:hover{background:rgb(59, 61, 66, 100);}\
                             QTabBar::tab:selected{border-color: white;background:white;color:black;}");


    //年路径目录列表
    yearListWidget = new QImageListWidget;
    connect(yearListWidget,SIGNAL(closeTabList()),this,SLOT(closeTabListWidget()));
    yearListWidget->setFixedSize(640,380);
    yearListWidget->setDirSeries(0);//设置为0级目录


    //日路径目录列表
    picListWidget = new QImageListWidget;
    connect(picListWidget,SIGNAL(closeTabList()),this,SLOT(closeTabListWidget()));
    picListWidget->setFixedSize(640,380);
    picListWidget->setDirSeries(1);//设置为1级目录

    //开启菜单
    connect(yearListWidget,SIGNAL(clickselectList(QImageListWidget*)),
            this,SLOT(openCustomMenu(QImageListWidget*)));

    //开启菜单
    connect(picListWidget,SIGNAL(clickselectList(QImageListWidget*)),
            this,SLOT(openCustomMenu(QImageListWidget*)));

    //点击目录
    connect(yearListWidget,SIGNAL(itemClicked(QListWidgetItem*)),
            this,SLOT(yearListClicked(QListWidgetItem*)));

    //点击目录
    connect(picListWidget,SIGNAL(itemClicked(QListWidgetItem*)),
            this,SLOT(picListCLicked(QListWidgetItem*)));

    tabWidget->tabBar()->setFocusPolicy(Qt::NoFocus);
    tabWidget->addTab(yearListWidget,tr("根目录"));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0,0,0,9);
    layout->addWidget(tiltlelabel);
    layout->addWidget(tabWidget);
    layout->addSpacing(7);
    this->setLayout(layout);

    isOpenMenu = false;
    isOptMenu = false;
    isOpenImage = false;

}

QImageBrowser::~QImageBrowser()
{
    delete timer;
    delete imageWatcher;
    delete yearListWidget;
    delete picListWidget;
    delete tiltlelabel;
    delete tabWidget;

}

//搜索目录
void QImageBrowser::findDirs(QString path,QImageListWidget *listWidget)
{
    QFile file(tr("/home/root/cfg/dir_cfg.txt"));
    QString defaultDirName ;

    if(file.exists())
    {
        QReadWriteFile readFile;
        defaultDirName  = readFile.readFile(tr("/home/root/cfg/dir_cfg.txt"));
    }

    int i = 0;
    QPixmap pixmap(tr(":/images/dir.png"));

    QDir dir(path);
    QFileInfoList list = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs);

    foreach (QFileInfo info,list) {
        //载入目录文件夹
        QIconWidget *iconitem = new QIconWidget(listWidget,i,info.fileName(),info.absoluteFilePath());

        if(info.fileName() ==  defaultDirName)
        {
            iconitem->setDirState(QIconWidget::Show);
        }
        else
        {
            iconitem->setDirState(QIconWidget::Hiden);
        }

        iconitem->setIcon(pixmap.scaled(QSize(WICONSIZE,HICONSIZE)));
        listWidget->setItemWidget(iconitem->returnItem(),iconitem);
        listWidget->addListName(iconitem->objectName());
        i++;
    }

    listWidget->setCurrentPath(path);
    listWidget->setCurrentRow(0);
    return;
}

//点击年目录
void QImageBrowser::yearListClicked(QListWidgetItem *yearItem)
{
     QIconWidget *pitem = this->findChild<QIconWidget *>(yearItem->text());
    tabWidget->addTab(picListWidget,pitem->getFileName());
    tabWidget->setCurrentIndex(1);
    secondPath = rootPath + tr("/") + pitem->getFileName();
    // this->findDirs(secondPath,monthListWidget);
    this->findFiles(secondPath);
}

//图片回调函数
QImage scale(QString imageFileName)
{
    if(imageFileName.right(3) == "avi")
    {
        mutex.lock();

        //获取视频缩略图
        unsigned char rgbframe[WICONSIZE*HICONSIZE*3/4];
        if(-1 == get_Video_Frame(imageFileName.toLocal8Bit().data(),rgbframe,WICONSIZE/2,HICONSIZE/2))
        {
            qDebug()<<"-1";
            QFile file(imageFileName);
            file.remove();
            mutex.unlock();
            return QImage("");
        }
        mutex.unlock();
        return  QImage(rgbframe,WICONSIZE/2,HICONSIZE/2,QImage::Format_RGB888).scaled(QSize(WICONSIZE,HICONSIZE),
                                                                                       Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }

    //获取图片缩略图
    QImage image(imageFileName);
    return image.scaled(QSize(WICONSIZE,HICONSIZE),
                        Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

//搜索文件
void QImageBrowser::findFiles(QString path)
{
    if(imageWatcher->isRunning())
    {
        imageWatcher->cancel();
        imageWatcher->waitForFinished();
    }

    QStringList files;
    QDir dir(path);
    QStringList filters;
    filters << "*.jpg";
    filters << "*.png";
    filters<<"*.avi";
    QFileInfoList list = dir.entryInfoList(filters,QDir::Files,QDir::Time);

    picListWidget->clear();

    int i = 0;
    foreach (QFileInfo info,list) {
        files<<info.absoluteFilePath();

        //载入图片视频
        QIconWidget *iconitem = new QIconWidget(picListWidget,i,info.fileName(),info.absoluteFilePath());
        picListWidget->setItemWidget(iconitem->returnItem(),iconitem);
        picListWidget->addListName(iconitem->objectName());
        i++;
    }
    picListWidget->setCurrentPath(path);
    picListWidget->setCurrentRow(0);

    imageWatcher->setFuture(QtConcurrent::mapped(files, scale));

}

//显示图片浏览
void QImageBrowser::showImage(int num)
{
    QIconWidget *pitem = picListWidget->findChild<QIconWidget *>(picListWidget->item(num)->text());

    if(pitem == NULL)
        return;

    pitem->setIcon(QPixmap::fromImage(imageWatcher->resultAt(num)));
}

//点击图片
void QImageBrowser::picListCLicked(QListWidgetItem *picItem)
{
    if(!picItem)
    {
        return;
    }
    isOpenImage = true;

    QImageListWidget * list= (QImageListWidget *)picItem->listWidget();

    displaydlg = new QShowImageWidget(NULL,list);
    displaydlg->setWindowModality(Qt::WindowModal);
    connect(displaydlg,SIGNAL(closeWidget()),this,SLOT(closeImageBrowser()));
    connect(this,SIGNAL(sendKey(int)),displaydlg,SLOT(getKeyEvent(int)));
    displaydlg->move(0,0);
    displaydlg->show();
}


//键盘事件
void QImageBrowser::getKeyEvent(int key)
{
    QKeyEvent rekey(QEvent::KeyPress,key,Qt::NoModifier);

    if(isOpenMenu)//开启菜单
    {
        if(isOptMenu)//操作菜单
        {
            emit sendKey(key);
        }
        else
        {
            QCoreApplication::sendEvent(dlg,&rekey);
        }
    }
    else if(isOpenImage)//打开图像
    {
        emit sendKey(key);
    }
    else{//点击列表

       // qDebug()<<"list key";
        if(tabWidget->currentIndex() == 0)
        {
            QCoreApplication::sendEvent(yearListWidget,&rekey);
        }
        else if(tabWidget->currentIndex() == 1){

            QCoreApplication::sendEvent(picListWidget,&rekey);
        }
    }
}

//开启菜单
void QImageBrowser::openCustomMenu(QImageListWidget *list)
{
    if((list->count() == 0) || isOpenMenu)
        return;

    isOpenMenu = true;
    dlg = new QCustomMenuDialog(this,list);
    dlg->setWindowModality(Qt::WindowModal);
    connect(dlg,SIGNAL(optImageFile(int,QListWidget*)),this,SLOT(optImageListFile(int,QListWidget*)));
    connect(dlg,SIGNAL(closeWidget()),this,SLOT(closeCustomMenu()));
    dlg->move((this->width() - dlg->width())/2 , (this->height() - dlg->height())/2);
    dlg->show();

}

//关闭菜单栏
void QImageBrowser::closeCustomMenu()
{
    isOpenMenu = false;
    //delete dlg;
}

//关闭菜单
void QImageBrowser::closeOptCreateMenu()
{
    disconnect(this,SIGNAL(sendKey(int)),cdlg,SLOT(getKeyEvent(int)));
    disconnect(cdlg,SIGNAL(closeWidget()),this,SLOT(closeOptCreateMenu()));
    isOptMenu = false;
    delete cdlg;
}

//关闭删除菜单
void QImageBrowser::closeOptDeleteMenu()
{
    disconnect(this,SIGNAL(sendKey(int)),ddlg,SLOT(getKeyEvent(int)));
    disconnect(ddlg,SIGNAL(closeWidget()),this,SLOT(closeOptDeleteMenu()));
    isOptMenu = false;
    delete ddlg;
    qDebug()<<"close delete";
}

//关闭重命名菜单
void QImageBrowser::closeOptRenameMenu()
{
    disconnect(this,SIGNAL(sendKey(int)),rdlg,SLOT(getKeyEvent(int)));
    disconnect(rdlg,SIGNAL(closeWidget()),this,SLOT(closeOptRenameMenu()));
    isOptMenu = false;
    delete rdlg;

}

//关闭图像浏览
void QImageBrowser::closeImageBrowser()
{
    disconnect(displaydlg,SIGNAL(closeWidget()),this,SLOT(closeImageBrowser()));
    disconnect(this,SIGNAL(sendKey(int)),displaydlg,SLOT(getKeyEvent(int)));
    isOpenImage = false;
}

//关闭列表
void QImageBrowser::closeTabListWidget()
{

    int index =tabWidget->currentIndex();
    if( index != 0)
    {

        if(imageWatcher->isRunning())
        {
            imageWatcher->cancel();
            imageWatcher->waitForFinished();
        }

        picListWidget->deleteListWidget();
        tabWidget->removeTab(index);//删除表头
    }
    else
    {
        yearListWidget->deleteListWidget();
        this->close();
        return;
    }

}

//关闭事件
void QImageBrowser::closeEvent(QCloseEvent *)
{
    emit closeWidget();
}

//菜单栏操作
void QImageBrowser::optImageListFile(int mode ,QListWidget *list)
{
    qDebug()<<"optMenu";
    QImageListWidget *currentlist = (QImageListWidget *)list;

    if((currentlist->count() == 0) || isOptMenu)
        return;

    isOptMenu = true;

    switch (mode) {
    case 0:
    {
        rdlg = new QRenameDialog(NULL,currentlist);
        rdlg->setWindowModality(Qt::WindowModal);
        connect(this,SIGNAL(sendKey(int)),rdlg,SLOT(getKeyEvent(int)));
        connect(rdlg,SIGNAL(closeWidget()),this,SLOT(closeOptRenameMenu()));
        rdlg->move((this->width() - rdlg->width())/2 , (this->height() - rdlg->height())/2);
        rdlg->show();

    }
        break;
    case 1:
    {
            ddlg = new QDeleteDialog(NULL,currentlist);
            ddlg->setWindowModality(Qt::WindowModal);
            connect(this,SIGNAL(sendKey(int)),ddlg,SLOT(getKeyEvent(int)));
            connect(ddlg,SIGNAL(closeWidget()),this,SLOT(closeOptDeleteMenu()));
            ddlg->move((this->width() - ddlg->width())/2 , (this->height() - ddlg->height())/2);
            ddlg->show();
    }
        break;
    case 2:
    {
            cdlg = new QCreateDirDialog(NULL,currentlist);
            cdlg->setWindowModality(Qt::WindowModal);
            connect(this,SIGNAL(sendKey(int)),cdlg,SLOT(getKeyEvent(int)));
            connect(cdlg,SIGNAL(closeWidget()),this,SLOT(closeOptCreateMenu()));
            cdlg->move((this->width() - cdlg->width())/2 , (this->height() - cdlg->height())/2);
            cdlg->show();

    }
        break;
    case 3:
    {

        foreach (QString itemname, currentlist->getListItemName()) {

            QIconWidget *item = currentlist->findChild<QIconWidget *>(itemname);
            if(itemname == currentlist->currentItem()->text())
            {
                item->setDirState(QIconWidget::Show);
                QReadWriteFile writeFile;
                writeFile.writeFile(tr("/home/root/cfg/dir_cfg.txt"),item->getFileName());
                defaultDir = item->getFileName();
            }
            else
            {
                item->setDirState(QIconWidget::Hiden);
            }
        }

        isOptMenu = false;
    }
        break;

    default:
        break;
    }
}

