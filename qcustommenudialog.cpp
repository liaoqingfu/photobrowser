#include "qcustommenudialog.h"

QCustomMenuDialog::QCustomMenuDialog(QWidget *parent,QImageListWidget *list):
    QDialog(parent)
{
    this->setWindowFlags(this->windowFlags() |Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

    currentlist = list;

    listnum = 2;
    currentnum = 0;

    QLabel *renamebtn = new QLabel("重命名");
    renamebtn->setObjectName(QString::number(0));
    renamebtn->setFixedSize(160,60);
    renamebtn->setAlignment(Qt::AlignCenter);
    renamebtn->setStyleSheet("background-color: rgb(85, 170, 255);");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    layout->addWidget(renamebtn);


    if(currentlist->getDirSeies() == 3)
    {
        QLabel *deletebtn = new QLabel("删除");
        deletebtn->setObjectName(QString::number(1));
        deletebtn->setFixedSize(160,60);
        deletebtn->setAlignment(Qt::AlignCenter);
        deletebtn->setStyleSheet("background-color: rgb(255, 255, 255);");

        layout->addWidget(deletebtn);

    }
    else
    {
        QLabel *createbtn = new QLabel("新建文件夹");
        createbtn->setObjectName(QString::number(1));
        createbtn->setFixedSize(160,60);
        createbtn->setAlignment(Qt::AlignCenter);
        createbtn->setStyleSheet("background-color: rgb(255, 255, 255);");
        layout->addWidget(createbtn);
    }

    this->resize(160,120);

}


void QCustomMenuDialog::keyPressEvent(QKeyEvent *e)
{

    if(e->key() == Qt::Key_Up)
    {
        QLabel *pbtn = this->findChild<QLabel *>(QString::number(currentnum));
        pbtn->setStyleSheet("background-color: rgb(255, 255, 255);");

        currentnum--;
        if(currentnum < 0)
        {
            currentnum = listnum;
        }

        QLabel *cbtn = this->findChild<QLabel *>(QString::number(currentnum));
        cbtn->setStyleSheet("background-color: rgb(85, 170, 255);");

    }
    else if(e->key() == Qt::Key_Down)
    {
        QLabel *pbtn = this->findChild<QLabel *>(QString::number(currentnum));
        pbtn->setStyleSheet("background-color: rgb(255, 255, 255);");

        currentnum++;
        if(currentnum > listnum)
        {
            currentnum = 0;
        }

        QLabel *cbtn = this->findChild<QLabel *>(QString::number(currentnum));
        cbtn->setStyleSheet("background-color: rgb(85, 170, 255);");
    }
    else if(e->key() == Qt::Key_Left)
    {

    }
    else if(e->key() == Qt::Key_Right)
    {

    }
    else if(e->key() == Qt::Key_Return)
    {
        switch (currentnum) {
        case 0:
            emit optImageFile(0,currentlist);

            break;
        case 1:
            emit optImageFile(1,currentlist);

            break;
        default:
            break;
        }
    }
    else if(e->key() == Qt::Key_Enter)
    {
        this->close();
    }

    QWidget::keyPressEvent(e);

}





