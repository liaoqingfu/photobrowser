#include "qdeletedialog.h"

QDeleteDialog::QDeleteDialog(QWidget *parent,QImageListWidget *list) :
    QDialog(parent)
{
    this->setFixedSize(370,250);
    this->setWindowFlags(this->windowFlags() |Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    this->setStyleSheet("background-color: rgb(59, 61, 66, 100);border-radius: 10px;  border: 2px groove black;");


    currentlist = list;

    sureBtn = new QPushButton(tr("确定"),this);
    connect(sureBtn,SIGNAL(clicked()),this,SLOT(on_sureBtn_clicked()));
    sureBtn->setFixedSize(100,40);
    sureBtn->move(60,190);

    cancelBtn  = new QPushButton(tr("取消"),this);
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(on_cancelBtn_clicked()));
    cancelBtn->setFixedSize(100,40);
    cancelBtn->move(220,190);

    sureBtn->setStyleSheet("QPushButton{background-color:  rgb(59, 61, 66, 100); \
                           color: white; \
            border-radius: 10px; \
border: 2px groove gray;\
    border-style: outset;}"
"QPushButton:focus{background-color:orange; \
color: white;}"
"QPushButton:pressed{background-color:rgb(85, 170, 255);\
border-style: inset; }");


cancelBtn->setStyleSheet("QPushButton{background-color:  rgb(59, 61, 66, 100); \
                         color: white; \
border-radius: 10px; \
border: 2px groove gray;\
border-style: outset;}"
"QPushButton:focus{background-color:orange; \
color: white;}"
"QPushButton:pressed{background-color:rgb(85, 170, 255);\
border-style: inset; }");

QIconWidget *item = currentlist->findChild<QIconWidget *>(currentlist->currentItem()->text());

label = new QLabel(this);
label->setFixedSize(350,81);
label->move(10,70);

QFont font;
font.setPixelSize(20);
label->setAlignment(Qt::AlignCenter);
label->setStyleSheet("background-color:transparent; \
                     color: white; border-radius: 0px;  border: 0px; ");
label->setText(tr("是否删除") + item->getFileName() + tr("?"));
label->setFont(font);


progressBar = new QProgressBar(this);
progressBar->resize(300,30);
progressBar->setMaximum(100);
progressBar->move(30,60);
progressBar->hide();

qDebug()<<"delete menu";

isDelete = true;

}

QDeleteDialog::~QDeleteDialog()
{
    qDebug()<<"delete close";
}

//改变进度条的值
void QDeleteDialog::on_progressBar_valueChanged(int value,QString path)
{
    progressBar->setValue(value);
}


//确定删除
void QDeleteDialog::on_sureBtn_clicked()
{

    if(!isDelete)
        return;

    isDelete = false;

    //当前选中项目
    QIconWidget *item = currentlist->findChild<QIconWidget *>(currentlist->currentItem()->text());

    if(item != NULL)
    {
        //删除文件和项目
        QString path = item->getIconPath();
        QFileInfo fileinfo(path);
        if(!fileinfo.isDir())
        {
            qDebug()<<item->getIconPath();
            if(QFile::remove(item->getIconPath()))
            {
                currentlist->removeListName(currentlist->currentItem()->text());
                currentlist->removeItemWidget(item->returnItem());
                delete item->returnItem();
                delete item;

            }
        }
        else
        {
            label->hide();
            progressBar->show();

            //删除文件夹
            QClearSd * sd = new QClearSd;

            connect(sd,SIGNAL(FileChanged(int,QString)),this,SLOT(on_progressBar_valueChanged(int,QString)));

            sd->FindFile(item->getIconPath());
            sd->removefilesindir(item->getIconPath());

            currentlist->removeListName(currentlist->currentItem()->text());
            currentlist->removeItemWidget(item->returnItem());

            delete item->returnItem();
            delete item;
            delete sd;

        }
    }

    emit deleteItem();
    this->close();
}

//取消
void QDeleteDialog::on_cancelBtn_clicked()
{
    this->close();
}

//关闭事件
void QDeleteDialog::closeEvent(QCloseEvent *)
{
    emit closeWidget();
}

//获取键值
void QDeleteDialog::getKeyEvent(int key)
{
    if(key == Qt::Key_R)//返回
    {
        this->on_cancelBtn_clicked();
        return;
    }

    QKeyEvent rekey(QEvent::KeyPress,key,Qt::NoModifier);//发送事件
    QCoreApplication::sendEvent(this->focusWidget(),&rekey);

}
