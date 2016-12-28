#include "qshowimagewidget.h"

QShowImageWidget::QShowImageWidget(QWidget *parent, QImageListWidget *list) :
    QWidget(parent)
{
    this->setWindowFlags(this->windowFlags() |Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    this->resize(VGA_WIDTH,VGA_HEIGHT);

    currentlist = list;
    zoomnum = 1;

    isVideo = false;
    isFilp = false;

    QFont mainfont;
    mainfont.setPixelSize(30);
    numberlabel = new QLabel(this);
    numberlabel->move(550,10);
    numberlabel->setFont(mainfont);
    numberlabel->setStyleSheet("background-color:transparent;color:rgb(85, 170, 255);");
    numberlabel->resize(100,30);
    numberlabel->setText(QString::number(currentlist->currentRow() + 1) + tr("/") + QString::number(currentlist->count()));

    mainfont.setPixelSize(25);
    filenamelabel = new QLabel(this);
    filenamelabel->move(10,10);
    filenamelabel->setFont(mainfont);
    filenamelabel->setStyleSheet("background-color:transparent;color:rgb(85, 170, 255);");
    filenamelabel->setText(currentlist->currentItem()->text());

    QFont smallfont;
    smallfont.setPixelSize(18);
    smallfont.setWeight(3);
    zoomlabel = new QLabel(this);
    zoomlabel->move(10,430);
    zoomlabel->setFont(smallfont);
    zoomlabel->resize(100,30);
    zoomlabel->setStyleSheet("background-color:transparent;color:yellow;");
    zoomlabel->setText(tr("Zoom ") + QString::number(zoomnum,'g',2) + tr("X"));
    zoomlabel->setStyleSheet("background-color:transparent;color:yellow;");

    videologolabel = new QLabel(this);
    videologolabel->resize(120,120);
    videologolabel->move((this->width() - 150)/2,(this->height() - 150)/2);
    videologolabel->setStyleSheet("background-color:transparent;border-image: url(:/images/play.png)");
    videologolabel->hide();

    rgbframe = (unsigned char*)malloc(VGA_WIDTH*VGA_HEIGHT*3);

    if(currentlist->currentItem()->text().right(3) == "avi")
    {
        QString videopath =currentlist->getCurrentPath() + tr("/") + currentlist->currentItem()->text();

        mutex.lock();
        get_Video_Frame(videopath.toLocal8Bit().data(),rgbframe,VGA_WIDTH,VGA_HEIGHT);
        mutex.unlock();
        backgroundpixmap = backgroundpixmap.fromImage( QImage(rgbframe,VGA_WIDTH,VGA_HEIGHT,QImage::Format_RGB888));

        zoomlabel->hide();
        videologolabel->show();
        isVideo = true;
    }
    else
        backgroundpixmap.load(currentlist->getCurrentPath() + tr("/") + currentlist->currentItem()->text());

}

QShowImageWidget::~QShowImageWidget()
{
    delete zoomlabel;
    delete numberlabel;
    delete filenamelabel;
}


//keyevent
void QShowImageWidget::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Up)
    {

    }
    else if(e->key() == Qt::Key_Down)
    {

    }
    else if(e->key() == Qt::Key_Left)
    {
        if(currentlist->currentRow() == 0)
            currentlist->setCurrentRow(currentlist->count() - 1);
        else
            currentlist->setCurrentRow(currentlist->currentRow() - 1);

        switchImageMode();

        update();
    }
    else if(e->key() == Qt::Key_Right)
    {

        if(currentlist->currentRow() == (currentlist->count() - 1))
            currentlist->setCurrentRow(0);
        else
            currentlist->setCurrentRow(currentlist->currentRow() + 1);

        numberlabel->setText(QString::number(currentlist->currentRow() + 1) + tr("/") + QString::number(currentlist->count()));
        filenamelabel->setText(currentlist->currentItem()->text());

        switchImageMode();

        update();
    }
    else if(e->key() == Qt::Key_Z)
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

        if(isFilp)
            isFilp = false;
        else
            isFilp = true;

        update();

    }
    else if(e->key() == Qt::Key_Enter)
    {
        this->close();
    }
    else if(e->key() == Qt::Key_Return)
    {
        if(isVideo)
        {

        }
    }
    else if(e->key() == Qt::Key_O)
    {
        QCustomMenuDialog *dlg = new QCustomMenuDialog(this,currentlist);
        dlg->setWindowModality(Qt::WindowModal);
        connect(dlg,SIGNAL(optImageFile(int,QListWidget*)),this,SLOT(optImageListFile(int,QListWidget*)));
        dlg->move((this->width() - dlg->width())/2 , (this->height() - dlg->height())/2);
        dlg->exec();
    }


    QWidget::keyPressEvent(e);
}


void QShowImageWidget::switchImageMode()
{
    if(currentlist->count() == 0)
    {
        numberlabel->setText(tr("0/0"));
        filenamelabel->setText("null");
        return;
    }
    else if(currentlist->currentItem()->text().right(3) == "avi")
    {
        QString videopath =currentlist->getCurrentPath() + tr("/") + currentlist->currentItem()->text();

        mutex.lock();
        get_Video_Frame(videopath.toLocal8Bit().data(),rgbframe,VGA_WIDTH,VGA_HEIGHT);
        mutex.unlock();
        backgroundpixmap = backgroundpixmap.fromImage( QImage(rgbframe,VGA_WIDTH,VGA_HEIGHT,QImage::Format_RGB888));

        zoomlabel->hide();
        videologolabel->show();
        isVideo = true;
    }
    else
    {
        backgroundpixmap.load(currentlist->getCurrentPath() + tr("/") + currentlist->currentItem()->text());
        isFilp = false;

        zoomnum = 1.0;
        zoomlabel->setText(tr("Zoom ") + QString::number(zoomnum,'g',2) + tr("X"));
        zoomlabel->show();
        videologolabel->hide();
        isVideo = false;
    }

    numberlabel->setText(QString::number(currentlist->currentRow() + 1) + tr("/") + QString::number(currentlist->count()));
    filenamelabel->setText(currentlist->currentItem()->text());

}


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



void QShowImageWidget::optImageListFile(int mode ,QListWidget *list)
{
    QImageListWidget *currentlist = (QImageListWidget *)list;

    switch (mode) {
    case 0:
    {

        QRenameDialog *rdlg = new QRenameDialog(this,currentlist);
        rdlg->setWindowModality(Qt::WindowModal);
        connect(rdlg,SIGNAL(changeName()),this,SLOT(changeImageName()));
        rdlg->move((this->width() - rdlg->width())/2 , (this->height() - rdlg->height())/2);
        rdlg->exec();

    }
        break;
    case 1:
    {
        QDeleteDialog *ddlg = new QDeleteDialog(this,currentlist);
        ddlg->setWindowModality(Qt::WindowModal);
         connect(ddlg,SIGNAL(deleteItem()),this,SLOT(switchImageMode()));
        ddlg->move((this->width() - ddlg->width())/2 , (this->height() - ddlg->height())/2);
        ddlg->exec();

    }
        break;

    default:
        break;
    }
}


void QShowImageWidget::changeImageName()
{
     filenamelabel->setText(currentlist->currentItem()->text());
}
