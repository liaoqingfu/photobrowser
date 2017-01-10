#include "qiconwidget.h"

QIconWidget::QIconWidget(QWidget *parent,int num,QString filename,QString path) :
    QWidget(parent)
{
    //设置窗体大小
    this->setFixedSize(WICONSIZE,HICONSIZE);
    //this->setStyleSheet(tr("QWidget{background: white;}"));

    //设置图片路径，保存后缀，设置状态
    iconpath = path;
    videosuffix = filename.right(4);
    isStates = Hide;

    //存放图片项目
    item = new QListWidgetItem(filename,(QListWidget *)parent);
    this->setObjectName(filename);

    //设置项目大小
    item->setTextColor(QColor(255,255,255));
    item ->setSizeHint(QSize(WICONSIZE,HICONSIZE));

     //图片缩略图
    label = new QLabel(this);
    label->setAlignment(Qt::AlignCenter);
    label->resize(WICONSIZE, HICONSIZE);

    //边框
    borderlabel = new QLabel(this);
    borderlabel->setStyleSheet("QLabel{background: transparent;}");
    borderlabel->resize(WICONSIZE, HICONSIZE);
    borderlabel->raise();

    //图片名称
    QFont font;
    font.setPixelSize(18);
    namelabel = new QLabel(this);
    namelabel->setFont(font);
    namelabel->setFixedHeight(20);
    namelabel->resize(100,30);
    namelabel->setStyleSheet("QLabel{background: transparent;color:rgb(68, 119, 180);}");
    namelabel->raise();
    namelabel->move(10,95);
    namelabel->setText(filename);
    namelabel->show();

    //图片状态
    checkstate = new QLabel(this);
    checkstate->setFixedSize(35,35);
    checkstate->setStyleSheet("background: transparent;");
    checkstate->move(10,20);
    checkstate->show();

    //图片标示
    videostate = new QLabel(this);
    videostate->setFixedSize(30,15);
    videostate->setStyleSheet("background: transparent;border-image: url(:/images/video.ico);");
    videostate->move(15,70);
    videostate->hide();

    if( (videosuffix == ".avi") || (videosuffix == ".mkv") )
    {
        videostate->show();
    }

}

QIconWidget::~QIconWidget()
{
    delete videostate;
    delete checkstate;
    delete namelabel;
    delete label;
}

//设置边框
void QIconWidget::setBorderColor(QString sheet)
{
    borderlabel->setStyleSheet(sheet);
}

//设置名称
void QIconWidget::setName(QString name)
{
    namelabel->setText(name);
}

//设置图片
void QIconWidget::setIcon(QPixmap pixmap)
{
    label->setPixmap(pixmap);
}

//返回项目
QListWidgetItem *QIconWidget::returnItem()
{
    //item->setFlags(Qt::NoItemFlags);
    return item;
}

//获取图片路径
QString QIconWidget::getIconPath()
{
    return iconpath;
}

//设置图片路径
void QIconWidget::setIconPath(QString path)
{
    iconpath = path;
}

//设置选中状态
void QIconWidget::setCheckState(IconStates state)
{
    if(state == Select)
    {
        isStates = Select;
        checkstate->setStyleSheet("background: transparent;border-image: url(:/images/radio.png);");
    }
    else if(state == NoSelect)
    {
        isStates = NoSelect;
        checkstate->setStyleSheet("background: transparent;border-image: url(:/images/adjust.png);");
    }
    else if(state == Hide)
    {
        isStates = Hide;
        checkstate->setStyleSheet("background: transparent;");
    }
}
