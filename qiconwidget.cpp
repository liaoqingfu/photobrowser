#include "qiconwidget.h"

QIconWidget::QIconWidget(QWidget *parent,int num,QString filename,QString path) :
    QWidget(parent)
{
    this->setFixedSize(WICONSIZE,HICONSIZE);
    this->setStyleSheet(tr("QWidget{background: white;}"));

    iconpath = path;
    videosuffix = filename.right(4);
    isStates = Hide;

    item = new QListWidgetItem(filename,(QListWidget *)parent);
    this->setObjectName(filename);


    item->setTextColor(QColor(255,255,255));
    //item->setData(Qt::UserRole,filename);
    item ->setSizeHint(QSize(WICONSIZE,HICONSIZE));


    label = new QLabel(this);
    label->setAlignment(Qt::AlignCenter);
    label->resize(WICONSIZE, HICONSIZE);


    borderlabel = new QLabel(this);
    borderlabel->setStyleSheet("QLabel{background: transparent;}");
    borderlabel->resize(WICONSIZE, HICONSIZE);
    borderlabel->raise();

    QFont font;
    font.setPointSize(13);
    namelabel = new QLabel(this);
    namelabel->setFont(font);
    namelabel->setFixedHeight(20);
    namelabel->resize(100,30);
    namelabel->setStyleSheet("QLabel{background: transparent;color:yellow;}");
    namelabel->raise();
    namelabel->move(10,95);
    //namelabel->setAlignment(Qt::AlignCenter);
    namelabel->setText(filename);
    namelabel->show();




    checkstate = new QLabel(this);
    checkstate->setFixedSize(35,35);
    checkstate->setStyleSheet("background: transparent;");
    checkstate->move(10,20);
    checkstate->show();

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


void QIconWidget::setBorderColor(QString sheet)
{
    borderlabel->setStyleSheet(sheet);
}

void QIconWidget::setName(QString name)
{
    namelabel->setText(name);
}

void QIconWidget::setIcon(QPixmap pixmap)
{
    label->setPixmap(pixmap);
}

QListWidgetItem *QIconWidget::returnItem()
{
    //item->setFlags(Qt::NoItemFlags);
    return item;
}

QString QIconWidget::getIconPath()
{
    return iconpath;
}

void QIconWidget::setIconPath(QString path)
{
    iconpath = path;
}

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
