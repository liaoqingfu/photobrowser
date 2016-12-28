#include "qdeletedialog.h"
#include "ui_qdeletedialog.h"

QDeleteDialog::QDeleteDialog(QWidget *parent,QImageListWidget *list) :
    QDialog(parent),
    ui(new Ui::QDeleteDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() |Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

    currentlist = list;

    QFont font;
    font.setPixelSize(20);

    ui->label->setText(tr("delete") + list->currentItem()->text() + tr("?"));
    ui->label->setFont(font);

}

QDeleteDialog::~QDeleteDialog()
{
    delete ui;
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
