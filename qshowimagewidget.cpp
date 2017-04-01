#include "qshowimagewidget.h"

QShowImageWidget::QShowImageWidget(QWidget *parent, QImageListWidget *list) :
    QWidget(parent)
{
    this->setWindowFlags(this->windowFlags() |Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    this->resize(VGA_WIDTH,VGA_HEIGHT);
    this->setStyleSheet("background-color:black;");

    currentlist = list;//图像列表
    zoomnum = 1;//放大倍数

    isVideo = false;//是否视频
    isFilp = false;//是否翻转

    isOpenVideo = false;//是否打开视频
    isOpenMenu = false;//是否打开菜单
    isOptMenu = false;//是否操作菜单

    QFont mainfont;
    mainfont.setPixelSize(30);
    numberlabel = new QLabel(this);
    numberlabel->move(550,10);
    numberlabel->setFont(mainfont);
    numberlabel->setStyleSheet("background-color:transparent;color:rgb(85, 170, 255);");
    numberlabel->resize(100,30);
    numberlabel->setText(QString::number(currentlist->currentRow() + 1) + tr("/") + QString::number(currentlist->count()));

    QIconWidget *item = currentlist->findChild<QIconWidget *>(currentlist->currentItem()->text());

    mainfont.setPixelSize(25);
    filenamelabel = new QLabel(this);
    filenamelabel->move(10,10);
    filenamelabel->setFont(mainfont);
    filenamelabel->setStyleSheet("background-color:transparent;color:rgb(85, 170, 255);");
    filenamelabel->setText(item->getFileName());

    QFont smallfont;
    smallfont.setPixelSize(25);
    smallfont.setWeight(3);
    zoomlabel = new QLabel(this);
    zoomlabel->move(10,450);
    zoomlabel->setFont(smallfont);
    zoomlabel->resize(140,30);
    zoomlabel->setStyleSheet("background-color:transparent;color:rgb(68,119,180);");//color:yellow;scaleLabel->setStyleSheet(tr("color:rgb(68,119,180)"));
    zoomlabel->setText(tr("Zoom ") + QString::number(zoomnum,'g',2) + tr("X"));

    //翻转显示
    rolabel = new QLabel(this);
    rolabel->move(160,450);
    rolabel->resize(32,32);
    rolabel->setStyleSheet(QString("background-color:transparent;background-image: url(:/images/R.ico);"));

    videologolabel = new QLabel(this);
    videologolabel->resize(120,120);
    videologolabel->move((this->width() - 120)/2,(this->height() - 120)/2);
    videologolabel->setStyleSheet("background-color:transparent;border-image: url(:/images/play.png)");
    videologolabel->hide();

    //分配rgb帧空间
    rgbframe = (unsigned char*)malloc(VGA_WIDTH*VGA_HEIGHT*3);

    if(item->getFileName().right(3) == "avi")
    {
        QString videopath =item->getIconPath();
        mutex.lock();
        get_Video_Frame(videopath.toLocal8Bit().data(),rgbframe,VGA_WIDTH,VGA_HEIGHT);
        mutex.unlock();
        backgroundpixmap = backgroundpixmap.fromImage( QImage(rgbframe,VGA_WIDTH,VGA_HEIGHT,QImage::Format_RGB888));

        zoomlabel->hide();
        rolabel->hide();
        videologolabel->show();
        isVideo = true;
    }
    else
    {
        backgroundpixmap.load(item->getIconPath());

    }

}

QShowImageWidget::~QShowImageWidget()
{
    delete rgbframe;
    delete rolabel;
    delete videologolabel;
    delete zoomlabel;
    delete numberlabel;
    delete filenamelabel;
}


//右移
void QShowImageWidget::moveLeft()
{
    if(currentlist->currentRow() == 0)
        currentlist->setCurrentRow(currentlist->count() - 1);
    else
        currentlist->setCurrentRow(currentlist->currentRow() - 1);

    this-> switchImageMode();
}

//左移
void QShowImageWidget::moveRight()
{
    if(currentlist->currentRow() == (currentlist->count() - 1))
        currentlist->setCurrentRow(0);
    else
        currentlist->setCurrentRow(currentlist->currentRow() + 1);

    this->switchImageMode();
}



//键盘事件
void QShowImageWidget::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Up)
    {
        this->moveLeft();
    }
    else if(e->key() == Qt::Key_Down)
    {
        this->moveRight();
    }
    else if(e->key() == Qt::Key_Left)
    {
        this->moveLeft();
    }
    else if(e->key() == Qt::Key_Right)
    {
        this->moveRight();
    }
    else if(e->key() == Qt::Key_S)
    {
        int zoommax= zoomnum*10;
        if(zoommax == 15)
        {
            zoomnum = 1.0;
        }
        else
        {
            zoomnum += 0.1;
        }

        zoomlabel->setText(tr("Zoom ") + QString::number(zoomnum,'g',2) + tr("X"));

        update();

    }
    else if(e->key() == Qt::Key_F)
    {

    }
    else if(e->key() == Qt::Key_R)
    {
        this->close();
        return;
    }
    else if(e->key() == Qt::Key_Return)
    {
        if(isVideo)
        {
            if( (currentlist->count() != 0) && !isOpenVideo)
            {
                isOpenVideo = true;

                QIconWidget *item = currentlist->findChild<QIconWidget *>(currentlist->currentItem()->text());
                QString videopath =item->getIconPath();

                videobroswer =new QVideoBrowser(NULL,videopath);
                videobroswer->updateVideoHeadFrame(rgbframe);

                connect(this,SIGNAL(sendKey(int)),videobroswer,SLOT(getKeyEvent(int)));
                connect(videobroswer,SIGNAL(closeWidget()),this,SLOT(closeVideoBrowser()));

                videobroswer->setWindowModality(Qt::ApplicationModal);
                videobroswer->show();

            }
        }
        else
        {
            if(isFilp)
            {
                rolabel->setStyleSheet(QString("background-color:transparent;background-image: url(:/images/R.ico);"));
                isFilp = false;
            }
            else
            {
                rolabel->setStyleSheet(QString("background-color:transparent;background-image: url(:/images/R180.ico);"));
                isFilp = true;
            }
            update();
        }
    }
    else if(e->key() == Qt::Key_M)
    {

        if((currentlist->count() != 0 ) && !isOpenMenu)
        {
            isOpenMenu = true;

            dlg = new QCustomMenuDialog(this,currentlist);


            connect(dlg,SIGNAL(optImageFile(int,QListWidget*)),this,SLOT(optImageListFile(int,QListWidget*)));
            connect(dlg,SIGNAL(closeWidget()),this,SLOT(closeCustomMenu()));

            dlg->setWindowModality(Qt::WindowModal);
            dlg->move((this->width() - dlg->width())/2 , (this->height() - dlg->height())/2);
            dlg->show();
        }
    }


    QWidget::keyPressEvent(e);
}


//切换图像模式
void QShowImageWidget::switchImageMode()
{

    if(currentlist->count() == 0)
    {
        numberlabel->setText(tr("0/0"));
        filenamelabel->setText("null");
        backgroundpixmap.fill(QColor(59, 61, 66));
        videologolabel->hide();
        update();
        return;
    }

    QIconWidget *item = currentlist->findChild<QIconWidget *>(currentlist->currentItem()->text());

    if(item->getFileName().right(3) == "avi")
    {
        QString videopath = item->getIconPath();

        mutex.lock();
        get_Video_Frame(videopath.toLocal8Bit().data(),rgbframe,VGA_WIDTH,VGA_HEIGHT);
        mutex.unlock();
        backgroundpixmap = backgroundpixmap.fromImage( QImage(rgbframe,VGA_WIDTH,VGA_HEIGHT,QImage::Format_RGB888));

        zoomlabel->hide();
        rolabel->hide();
        videologolabel->show();
        isVideo = true;
    }
    else
    {
        backgroundpixmap.load( item->getIconPath());
        isFilp = false;

        zoomnum = 1.0;
        zoomlabel->setText(tr("Zoom ") + QString::number(zoomnum,'g',2) + tr("X"));
        zoomlabel->show();
        rolabel->show();
        videologolabel->hide();
        isVideo = false;
    }

    numberlabel->setText(QString::number(currentlist->currentRow() + 1) + tr("/") + QString::number(currentlist->count()));
    filenamelabel->setText(item->getFileName());

    update();
}

//画布事件
void QShowImageWidget::paintEvent(QPaintEvent *e)
{
    QPainter p(this);

    if(!isVideo)
    {
        if(isFilp)
        {
            p.translate(VGA_WIDTH,VGA_HEIGHT);
            p.rotate(+180);
        }

        p.translate(-(zoomnum - 1.0)*VGA_WIDTH/2,-(zoomnum - 1.0)*VGA_HEIGHT/2);
        p.scale(zoomnum,zoomnum);
    }

    p.drawPixmap(0,0,VGA_WIDTH,VGA_HEIGHT,backgroundpixmap);
    p.setRenderHint(QPainter::Antialiasing);
}


//菜单列表
void QShowImageWidget::optImageListFile(int mode ,QListWidget *list)
{
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
        connect(rdlg,SIGNAL(changeName()),this,SLOT(changeImageName()));
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
        connect(ddlg,SIGNAL(deleteItem()),this,SLOT(switchImageMode()));
        ddlg->move((this->width() - ddlg->width())/2 , (this->height() - ddlg->height())/2);
        ddlg->show();

    }
        break;

    default:
        break;
    }
}

//关闭菜单
void QShowImageWidget::closeCustomMenu()
{
    isOpenMenu = false;
   // delete dlg;
}

//关闭删除菜单
void QShowImageWidget::closeOptDeleteMenu()
{
    disconnect(this,SIGNAL(sendKey(int)),ddlg,SLOT(getKeyEvent(int)));
    disconnect(ddlg,SIGNAL(closeWidget()),this,SLOT(closeOptDeleteMenu()));
    isOptMenu = false;
    delete ddlg;
}

//关闭重命名菜单
void QShowImageWidget::closeOptRenameMenu()
{
    disconnect(this,SIGNAL(sendKey(int)),rdlg,SLOT(getKeyEvent(int)));
    disconnect(rdlg,SIGNAL(closeWidget()),this,SLOT(closeOptRenameMenu()));
    isOptMenu = false;
    delete rdlg;
}

//关闭视频浏览
void QShowImageWidget::closeVideoBrowser()
{
    disconnect(this,SIGNAL(sendKey(int)),videobroswer,SLOT(getKeyEvent(int)));
    disconnect(videobroswer,SIGNAL(closeWidget()),this,SLOT(closeVideoBrowser()));
    isOpenVideo = false;
    delete videobroswer;
    videobroswer  = NULL;
    qDebug()<<"delete videobroswer";
}


//改变图像名称
void QShowImageWidget::changeImageName()
{
    QIconWidget *item = currentlist->findChild<QIconWidget *>(currentlist->currentItem()->text());
    filenamelabel->setText(item->getFileName());
}

//获取事件
void QShowImageWidget::getKeyEvent(int key)
{
    QKeyEvent rekey(QEvent::KeyPress,key,Qt::NoModifier);

    if(isOpenMenu)
    {
        if(isOptMenu)
        {
            emit sendKey(key);
        }
        else
        {
            QCoreApplication::sendEvent(dlg,&rekey);
        }
    }
    else if(isOpenVideo)
    {
        emit sendKey(key);
    }
    else
    {
        QCoreApplication::sendEvent(this,&rekey);
    }
}

//关闭事件
void QShowImageWidget::closeEvent(QCloseEvent *)
{
    emit closeWidget();
}
