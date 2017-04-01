#include "qcustommenudialog.h"

QCustomMenuDialog::QCustomMenuDialog(QWidget *parent,QImageListWidget *list):
    QDialog(parent)
{
    this->setWindowFlags(this->windowFlags() |Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

    currentlist = list;

    currentnum = 0;

    QLabel *renamebtn = new QLabel(tr("重命名"));
    renamebtn->setObjectName(QString::number(0));
    renamebtn->setFixedSize(200,60);
    renamebtn->setAlignment(Qt::AlignCenter);
    renamebtn->setStyleSheet("background-color: rgb(85, 170, 255);");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    layout->addWidget(renamebtn);


    QLabel *deletebtn = new QLabel(tr("删除"));
    deletebtn->setObjectName(QString::number(1));
    deletebtn->setFixedSize(200,60);
    deletebtn->setAlignment(Qt::AlignCenter);
    deletebtn->setStyleSheet("background-color: rgb(255, 255, 255);");

    layout->addWidget(deletebtn);
    this->resize(200,120);

     listnum = 1;

    if(currentlist->getDirSeies() == 0)
 {
        QLabel *createbtn = new QLabel(tr("新建文件夹"));
        createbtn->setObjectName(QString::number(2));
        createbtn->setFixedSize(200,60);
        createbtn->setAlignment(Qt::AlignCenter);
        createbtn->setStyleSheet("background-color: rgb(255, 255, 255);");
        layout->addWidget(createbtn);

        QLabel *defaultbtn = new QLabel(tr("设为默认路径"));
        defaultbtn->setObjectName(QString::number(3));
        defaultbtn->setFixedSize(200,60);
        defaultbtn->setAlignment(Qt::AlignCenter);
        defaultbtn->setStyleSheet("background-color: rgb(255, 255, 255);");

        layout->addWidget(defaultbtn);
        this->resize(200,240);

         listnum = 3;
    }

}

//左移
void QCustomMenuDialog::moveLeft()
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

//右移
void QCustomMenuDialog::moveRight()
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


//键盘事件
void QCustomMenuDialog::keyPressEvent(QKeyEvent *e)
{
    qDebug()<<"key event";
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
    else if(e->key() == Qt::Key_Return)
    {
        switch (currentnum) {
        case 0:
            emit optImageFile(0,currentlist);

            break;
        case 1:
            emit optImageFile(1,currentlist);

            break;
        case 2:
            emit optImageFile(2,currentlist);
            break;

        case 3:
            emit optImageFile(3,currentlist);
            this->close();
            break;

        default:
            break;
        }
    }
    else if(e->key() == Qt::Key_R)
    {
        this->close();

        return;
    }

    QWidget::keyPressEvent(e);

}

//关闭事件
void QCustomMenuDialog::closeEvent(QCloseEvent *)
{
    emit closeWidget();
}




