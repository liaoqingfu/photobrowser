#include "qimagebrowser.h"

QMutex mutex;

QImageBrowser::QImageBrowser(QWidget *parent) :
    QWidget(parent)
{
    //初始化布局
    this->initLayout();

    //设置TF卡路径
    rootPath = tr("/mnt");

    //初始化串口
    serial = new Master(this);
    serial->InitForm();
    serial->writeRocker(SLAVE_Y);


    //摇杆使用者
    connect(serial,SIGNAL(sendRockerInfo(QString)),this,SLOT(getRockerUser(QString)));
    //摇杆X/Y值
    connect(serial,SIGNAL(sendRockerXYInfo(QString,QString)),this,SLOT(getRockerXY(QString,QString)));
    connect(serial,SIGNAL(sendKeyInfo(QString)),this,SLOT(getKeyValues(QString)));


    //线程监控
    imageWatcher = new QFutureWatcher<QImage>(this);
    //图片动态加载
    connect(imageWatcher,SIGNAL(resultReadyAt(int)), this, SLOT(showImage(int)));

    //判断TF路径是否为空
    if(!rootPath.isEmpty())
        this->findDirs(rootPath,yearListWidget);

}

//初始化布局
void QImageBrowser::initLayout()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setStyleSheet(tr("QWidget{background-color: rgb(59, 61, 66, 100);}"));

    QFont font;
    font.setPixelSize(30);
    //font.setBold(true);

    tiltlelabel = new QLabel ;
    tiltlelabel->setFixedHeight(50);
    tiltlelabel->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(68, 119, 180);");
    tiltlelabel->setFont(font);
    tiltlelabel->setText("  图像浏览");


    tabWidget = new QTabWidget;
    tabWidget->setStyleSheet("QTabWidget::pane{border: 1px;}"
                             "QTabWidget::tab-bar{alignment:left;}"
                             "QTabBar::tab{font-size: 16px;margin-left:1;background:rgb(255, 255, 255, 100); color:black; min-width:10ex; min-height:5ex;}"
                             "QTabBar::tab:hover{background:rgb(59, 61, 66, 100);}"
                             "QTabBar::tab:selected{border-color: white;background:white;color:black;}");



    //年路径目录列表
    yearListWidget = new QImageListWidget;
    yearListWidget->setDirSeries(0);//设置为0级目录

    //月路径目录列表
    monthListWidget = new QImageListWidget;
    monthListWidget->setDirSeries(1);//设置为1级目录

    //日路径目录列表
    dayListWidget = new QImageListWidget;
    dayListWidget->setDirSeries(2);//设置为2级目录

    //日路径目录列表
    picListWidget = new QImageListWidget;
    picListWidget->setDirSeries(3);//设置为3级目录

    //开启菜单
    connect(yearListWidget,SIGNAL(clickselectList(QImageListWidget*)),
            this,SLOT(openCustomMenu(QImageListWidget*)));

    //开启菜单
    connect(monthListWidget,SIGNAL(clickselectList(QImageListWidget*)),
            this,SLOT(openCustomMenu(QImageListWidget*)));

    //开启菜单
    connect(dayListWidget,SIGNAL(clickselectList(QImageListWidget*)),
            this,SLOT(openCustomMenu(QImageListWidget*)));

    //开启菜单
    connect(picListWidget,SIGNAL(clickselectList(QImageListWidget*)),
            this,SLOT(openCustomMenu(QImageListWidget*)));

    //点击目录
    connect(yearListWidget,SIGNAL(itemClicked(QListWidgetItem*)),
            this,SLOT(yearListClicked(QListWidgetItem*)));

    //点击目录
    connect(monthListWidget,SIGNAL(itemClicked(QListWidgetItem*)),
            this,SLOT(monthListClicked(QListWidgetItem*)));

    //点击目录
    connect(dayListWidget,SIGNAL(itemClicked(QListWidgetItem*)),
            this,SLOT(dayListClicked(QListWidgetItem*)));

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

}

QImageBrowser::~QImageBrowser()
{
    imageWatcher->cancel();
    imageWatcher->waitForFinished();
    delete yearListWidget;
    delete monthListWidget;
    delete dayListWidget;
    delete picListWidget;

}


//搜索目录
void QImageBrowser::findDirs(QString path,QImageListWidget *listWidget)
{
    if(imageWatcher->isRunning())
    {
        imageWatcher->cancel();
        imageWatcher->waitForFinished();
    }

    int i = 0;
    QPixmap pixmap(tr(":/images/dir.png"));

    QDir dir(path);
    QFileInfoList list = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs);

    foreach (QFileInfo info,list) {

        //载入目录文件夹
        QIconWidget *iconitem = new QIconWidget(listWidget,i,info.fileName(),info.absoluteFilePath());
        iconitem->setIcon(pixmap.scaled(QSize(WICONSIZE,HICONSIZE)));
        listWidget->setItemWidget(iconitem->returnItem(),iconitem);
        listWidget->addListName(info.fileName());
        i++;
    }

    listWidget->setCurrentPath(path);
    listWidget->setCurrentRow(0);
    return;
}

//点击年目录
void QImageBrowser::yearListClicked(QListWidgetItem *yearItem)
{
    tabWidget->addTab(monthListWidget,yearItem->text());
    tabWidget->setCurrentIndex(1);
    secondPath = rootPath + tr("/") + yearItem->text();
    this->findDirs(secondPath,monthListWidget);

}

//点击月目录
void QImageBrowser::monthListClicked(QListWidgetItem *monthItem)
{

    tabWidget->addTab(dayListWidget,monthItem->text());
    tabWidget->setCurrentIndex(2);
    thirdPath = secondPath + tr("/") + monthItem->text();
    this->findDirs(thirdPath,dayListWidget);
}

//点击日目录
void QImageBrowser::dayListClicked(QListWidgetItem *dayItem)
{

    tabWidget->addTab(picListWidget,dayItem->text());
    tabWidget->setCurrentIndex(3);
    fourPath = thirdPath + tr("/") + dayItem->text();
    picListWidget->clear();
    this->findFiles(fourPath);
}




//图片回调函数
QImage scale(QString imageFileName)
{
    if( (imageFileName.right(3) == "avi") || (imageFileName.right(3) == "mkv") )
    {
        mutex.lock();

        //获取视频缩略图
        unsigned char rgbframe[WICONSIZE*HICONSIZE*3];
        if(-1 == get_Video_Frame(imageFileName.toLocal8Bit().data(),rgbframe,WICONSIZE,HICONSIZE))
        {
            qDebug()<<"-1";
            QFile file(imageFileName);
            file.remove();
            mutex.unlock();
            return QImage("");
        }
        mutex.unlock();
        return  QImage(rgbframe,WICONSIZE,HICONSIZE,QImage::Format_RGB888);
    }

    //获取视频缩略图
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
        picListWidget->addListName(info.fileName());
        i++;
    }
    picListWidget->setCurrentPath(path);
    picListWidget->setCurrentRow(0);

    imageWatcher->setFuture(QtConcurrent::mapped(files, scale));

}

//显示图片浏览
void QImageBrowser::showImage(int num)
{

    QIconWidget *pitem = this->findChild<QIconWidget *>(picListWidget->item(num)->text());
    pitem->setIcon(QPixmap::fromImage(imageWatcher->resultAt(num)));
}

//点击图片
void QImageBrowser::picListCLicked(QListWidgetItem *picItem)
{
    if(!picItem)
    {
        return;
    }
    QImageListWidget * list= (QImageListWidget *)picItem->listWidget();

    QShowImageWidget *displaydlg = new QShowImageWidget(NULL,list);
    displaydlg->setWindowModality(Qt::WindowModal);
    displaydlg->move((this->width() - displaydlg->width())/2 , (this->height() - displaydlg->height())/2);
    displaydlg->show();
}


//键盘事件
void QImageBrowser::keyPressEvent(QKeyEvent *e)
{
    //返回操作
    if(e->key() == Qt::Key_Enter)
    {
        int index =tabWidget->currentIndex();
        if( index != 0)
        {
            tabWidget->removeTab(index);//删除表头
        }
        else
        {
            this->close();
        }
    }
    else if(e->key() == Qt::Key_O)
    {

    }

    QWidget::keyPressEvent(e);
}


//开启菜单
void QImageBrowser::openCustomMenu(QImageListWidget *list)
{
    QCustomMenuDialog *dlg = new QCustomMenuDialog(this,list);
    dlg->setWindowModality(Qt::WindowModal);
    connect(dlg,SIGNAL(optImageFile(int,QListWidget*)),this,SLOT(optImageListFile(int,QListWidget*)));
    dlg->move((this->width() - dlg->width())/2 , (this->height() - dlg->height())/2);
    dlg->exec();
}

//菜单栏操作
void QImageBrowser::optImageListFile(int mode ,QListWidget *list)
{
    QImageListWidget *currentlist = (QImageListWidget *)list;

    switch (mode) {
    case 0:
    {

        QRenameDialog *rdlg = new QRenameDialog(this,currentlist);
        rdlg->setWindowModality(Qt::WindowModal);
        rdlg->move((this->width() - rdlg->width())/2 , (this->height() - rdlg->height())/2);
        rdlg->exec();

    }
        break;
    case 1:
    {
        if(currentlist->getDirSeies() == 3)
        {
            QDeleteDialog *ddlg = new QDeleteDialog(this,currentlist);
            ddlg->setWindowModality(Qt::WindowModal);
            ddlg->move((this->width() - ddlg->width())/2 , (this->height() - ddlg->height())/2);
            ddlg->exec();
        }
        else
        {
            QCreateDirDialog *cdlg = new QCreateDirDialog(this,currentlist);
            cdlg->setWindowModality(Qt::WindowModal);
            cdlg->move((this->width() - cdlg->width())/2 , (this->height() - cdlg->height())/2);
            cdlg->exec();
        }
    }
        break;

    default:
        break;
    }
}


//获取按键值

void QImageBrowser::getKeyValues(QString value)
{
    switch (value.toInt(NULL,16)) {
    case MAIN_UP_KEY:
        emit sendKey(Qt::Key_F);
        break;

    case MAIN_LEFT_KEY:
        emit sendKey(Qt::Key_S);
        break;

    case MAIN_DOWN_KEY:
        emit sendKey(Qt::Key_Return);
        break;

    case MAIN_RIGHT_KEY:
        emit sendKey(Qt::Key_R);
        break;

    case MAIN_OK_KEY:
        emit sendKey(Qt::Key_M);
        break;

    case MAIN_UP_LONG_KEY:
        break;

    case MAIN_LEFT_LONG_KEY:
        break;

    case MAIN_DOWN_LONG_KEY:
        break;

    case MAIN_RIGHT_LONG_KEY:
        break;

    case MAIN_OK_LONG_KEY:
        break;

    case MAIN_NONE_KEY:
        break;

    default:
        break;
    }

}


//获取摇杆使用者
void QImageBrowser::getRockerUser(QString user)
{
    if(user == MASTER_Y)
    {
        serial->curRockerUser = MASTER_Y;
    }

    if(user == SLAVE_Y)
    {
        serial->curRockerUser = SLAVE_Y;
    }
}

//获取摇杆XY值
void QImageBrowser::getRockerXY(QString x,QString y)
{
    int x1,y1;
    x1 = x.toInt(NULL,16) - CENTER_X;
    y1 = y.toInt(NULL,16) - CENTER_Y;
    if(qAbs(x1) >= qAbs(y1))
    {
        if(qAbs(x1) > X_MIN_STEP)
        {
            if(x1 > 0)
            {
                qDebug()<<"RIGHT";
                emit sendKey(Qt::Key_Right);
            }
            else
            {
                qDebug()<<"LEFT";
                emit sendKey(Qt::Key_Left);
            }
        }
    }
    else
    {
        if(qAbs(y1) > Y_MIN_STEP)
        {
            if(y1 > 0)
            {
                qDebug()<<"UP";
                emit sendKey(Qt::Key_Up);
            }
            else
            {
                qDebug()<<"Down";
                emit sendKey(Qt::Key_Down);
            }
        }
    }
}
