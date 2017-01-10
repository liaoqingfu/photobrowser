#include "qdeletedialog.h"
#include "ui_qdeletedialog.h"

QDeleteDialog::QDeleteDialog(QWidget *parent,QImageListWidget *list) :
    QDialog(parent)
{

    this->setFixedSize(370,250);
    this->setWindowFlags(this->windowFlags() |Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
   this->setStyleSheet("background-color: rgb(59, 61, 66, 100);border-radius: 10px;  border: 3px  rgb(59, 61, 66, 100);");


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


    label = new QLabel(this);
    label->setFixedSize(241,61);
    label->move(70,70);

    QFont font;
    font.setPixelSize(20);
    label->setAlignment(Qt::AlignCenter);
    label->setText(tr("是否删除") + list->currentItem()->text() + tr("?"));
    label->setFont(font);

}

QDeleteDialog::~QDeleteDialog()
{
    delete sureBtn;
    delete cancelBtn;
    delete label;
}

void QDeleteDialog::on_sureBtn_clicked()
{

    QIconWidget *item = currentlist->findChild<QIconWidget *>(currentlist->currentItem()->text());

    if(item != NULL)
    {
        QString path = item->getIconPath();

        QFileInfo fileinfo(path);
        if(!fileinfo.isDir())
        {
            if(QFile::remove(item->getIconPath()))
            {
                currentlist->removeListName(currentlist->currentItem()->text());
                currentlist->removeItemWidget(item->returnItem());
                delete item->returnItem();
                delete item;

                emit deleteItem();

            }

        }
        else
        {

        }


    }


    this->close();
}

void QDeleteDialog::on_cancelBtn_clicked()
{
    this->close();
}
