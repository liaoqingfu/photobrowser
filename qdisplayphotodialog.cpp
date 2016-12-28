#include "qdisplayphotodialog.h"
#include "ui_qdisplayphotodialog.h"

QDisplayPhotoDialog::QDisplayPhotoDialog(QWidget *parent, QImageListWidget *list) :
    QDialog(parent),
    ui(new Ui::QDisplayPhotoDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() |Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

    currentlist = list;

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
    zoomlabel->setStyleSheet("background-color:transparent;color:yellow;");
    zoomlabel->setText("Zoom 1.0");

    backgroundpixmap.load(currentlist->getCurrentPath() + tr("/") + currentlist->currentItem()->text());

    isFilp = false;

}

QDisplayPhotoDialog::~QDisplayPhotoDialog()
{
    delete ui;
}


//keyevent
void QDisplayPhotoDialog::keyPressEvent(QKeyEvent *e)
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

        numberlabel->setText(QString::number(currentlist->currentRow() + 1) + tr("/") + QString::number(currentlist->count()));
        filenamelabel->setText(currentlist->currentItem()->text());


    }
    else if(e->key() == Qt::Key_Right)
    {

        if(currentlist->currentRow() == (currentlist->count() - 1))
            currentlist->setCurrentRow(0);
        else
            currentlist->setCurrentRow(currentlist->currentRow() + 1);

        numberlabel->setText(QString::number(currentlist->currentRow() + 1) + tr("/") + QString::number(currentlist->count()));
        filenamelabel->setText(currentlist->currentItem()->text());

    }
    else if(e->key() == Qt::Key_Z)
    {


    }
    else if(e->key() == Qt::Key_F)
    {
        if(isFilp)
            isFilp = false;
        else
            isFilp = true;

    }
    else if(e->key() == Qt::Key_Return)
    {
        this->close();
    }
    else if(e->key() == Qt::Key_Enter)
    {

    }


    QWidget::keyPressEvent(e);
}


void QDisplayPhotoDialog::paintEvent(QPaintEvent *e)
{

    QPainter p(&backgroundpixmap);

    p.setRenderHint(QPainter::Antialiasing);

    QTransform transform;

    if(isFilp)
        transform.rotate(180);

    p.setTransform(transform);


}
