#include "qimagebrowser.h"
#include "ui_qimagebrowser.h"

QMutex mutex;

QImageBrowser::QImageBrowser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QImageBrowser)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint);

    this->initLayout();

    rootPath = tr("/media/yateks/9877-A831");

    //this->setStyleSheet(tr("QWidget{background: rgb(85, 170, 255);}"));

    imageWatcher = new QFutureWatcher<QImage>(this);

    connect(imageWatcher,SIGNAL(resultReadyAt(int)), this, SLOT(showImage(int)));

    if(!rootPath.isEmpty())
        this->findDirs(rootPath,yearListWidget);
}


void QImageBrowser::initLayout()
{
    ui->tabWidget->setStyleSheet("QTabWidget::pane{border: 0px;}"
                                 "QTabWidget::tab-bar{alignment:left;}"
                                 "QTabBar::tab{font-size: 16px;margin-left:1;background:rgb(255, 255, 255, 100); color:black; min-width:10ex; min-height:5ex;}"
                                 "QTabBar::tab:hover{background:rgb(255, 255, 255, 100);}"
                                 "QTabBar::tab:selected{border-color: white;background:white;color:black;}");



    yearListWidget = new QImageListWidget;
    yearListWidget->setDirSeries(0);

    monthListWidget = new QImageListWidget;
    monthListWidget->setDirSeries(1);

    dayListWidget = new QImageListWidget;
    dayListWidget->setDirSeries(2);

    picListWidget = new QImageListWidget;
    picListWidget->setDirSeries(3);


    connect(yearListWidget,SIGNAL(clickselectList(QImageListWidget*)),
            this,SLOT(openCustomMenu(QImageListWidget*)));


    connect(monthListWidget,SIGNAL(clickselectList(QImageListWidget*)),
            this,SLOT(openCustomMenu(QImageListWidget*)));


    connect(dayListWidget,SIGNAL(clickselectList(QImageListWidget*)),
            this,SLOT(openCustomMenu(QImageListWidget*)));

    connect(picListWidget,SIGNAL(clickselectList(QImageListWidget*)),
            this,SLOT(openCustomMenu(QImageListWidget*)));



    connect(yearListWidget,SIGNAL(itemClicked(QListWidgetItem*)),
            this,SLOT(yearListClicked(QListWidgetItem*)));


    connect(monthListWidget,SIGNAL(itemClicked(QListWidgetItem*)),
            this,SLOT(monthListClicked(QListWidgetItem*)));


    connect(dayListWidget,SIGNAL(itemClicked(QListWidgetItem*)),
            this,SLOT(dayListClicked(QListWidgetItem*)));

    connect(picListWidget,SIGNAL(itemClicked(QListWidgetItem*)),
            this,SLOT(picListCLicked(QListWidgetItem*)));

    ui->tabWidget->tabBar()->setFocusPolicy(Qt::NoFocus);
    ui->tabWidget->addTab(yearListWidget,tr("SD卡"));

}

QImageBrowser::~QImageBrowser()
{
    imageWatcher->cancel();
    imageWatcher->waitForFinished();
    delete yearListWidget;
    delete monthListWidget;
    delete dayListWidget;
    delete picListWidget;

    delete ui;
}


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

void QImageBrowser::yearListClicked(QListWidgetItem *yearItem)
{
    ui->tabWidget->addTab(monthListWidget,yearItem->text());
    ui->tabWidget->setCurrentIndex(1);
    secondPath = rootPath + tr("/") + yearItem->text();
    this->findDirs(secondPath,monthListWidget);

}

void QImageBrowser::monthListClicked(QListWidgetItem *monthItem)
{

    ui->tabWidget->addTab(dayListWidget,monthItem->text());
    ui->tabWidget->setCurrentIndex(2);
    thirdPath = secondPath + tr("/") + monthItem->text();
    this->findDirs(thirdPath,dayListWidget);
}

void QImageBrowser::dayListClicked(QListWidgetItem *dayItem)
{

    ui->tabWidget->addTab(picListWidget,dayItem->text());
    ui->tabWidget->setCurrentIndex(3);
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
    QImage image(imageFileName);
    return image.scaled(QSize(WICONSIZE,HICONSIZE),
                        Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}


void QImageBrowser::findFiles(QString path)
{
    if(imageWatcher->isRunning())
    {
        imageWatcher->cancel();
        imageWatcher->waitForFinished();
    }

    //int i = 0;
    //QPixmap pixmap;
    QStringList files;
    QDir dir(path);
    QStringList filters;
    filters << "*.jpg";
    filters << "*.png";
    filters<<"*.avi";
    QFileInfoList list = dir.entryInfoList(filters,QDir::Files,QDir::Time);
    //qDebug()<<list.size();

    picListWidget->clear();

    int i = 0;
    foreach (QFileInfo info,list) {
        files<<info.absoluteFilePath();

        QIconWidget *iconitem = new QIconWidget(picListWidget,i,info.fileName(),info.absoluteFilePath());
        picListWidget->setItemWidget(iconitem->returnItem(),iconitem);
        picListWidget->addListName(info.fileName());
        i++;
    }
    picListWidget->setCurrentPath(path);
    picListWidget->setCurrentRow(0);

    imageWatcher->setFuture(QtConcurrent::mapped(files, scale));

}

void QImageBrowser::showImage(int num)
{

    QIconWidget *pitem = this->findChild<QIconWidget *>(picListWidget->item(num)->text());
    pitem->setIcon(QPixmap::fromImage(imageWatcher->resultAt(num)));
}


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

    //    QDisplayPhotoDialog *displaydlg = new QDisplayPhotoDialog(NULL,list);
    //    displaydlg->setWindowModality(Qt::WindowModal);
    //    displaydlg->move((this->width() - displaydlg->width())/2 , (this->height() - displaydlg->height())/2);
    //    displaydlg->exec();

}


//keyevent
void QImageBrowser::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Enter)
    {
        int index = ui->tabWidget->currentIndex();
        if( index != 0)
        {

            ui->tabWidget->removeTab(index);

        }
        else
        {
            this->close();
        }
    }
    if(e->key() == Qt::Key_O)
    {

    }

    QWidget::keyPressEvent(e);
}



void QImageBrowser::openCustomMenu(QImageListWidget *list)
{
    QCustomMenuDialog *dlg = new QCustomMenuDialog(this,list);
    dlg->setWindowModality(Qt::WindowModal);
    connect(dlg,SIGNAL(optImageFile(int,QListWidget*)),this,SLOT(optImageListFile(int,QListWidget*)));
    dlg->move((this->width() - dlg->width())/2 , (this->height() - dlg->height())/2);
    dlg->exec();
}



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
