#include "qrenamedialog.h"
#include "ui_qrenamedialog.h"

QRenameDialog::QRenameDialog(QWidget *parent,QImageListWidget *list) :
    QDialog(parent),
    ui(new Ui::QRenameDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() |Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

    currentlist = list;

    ui->lineEdit->setFocusPolicy(Qt::NoFocus);
    QFont font;

    font.setPixelSize(45);

    ui->lineEdit->setFont(font);

    ui->lineEdit->setAlignment(Qt::AlignCenter);

    ui->lineEdit->setStyleSheet("QLabel{background-color: blue; \
                                color: white; \
                                border-radius: 10px; \
                                border: 2px groove gray;\
                                border-style: outset;}");



    ui->lineEdit->setText(currentlist->currentItem()->text().split('.').first());

    if(currentlist->currentItem()->text().right(4).left(1) == ".")
        suffix = tr(".") + currentlist->currentItem()->text().split('.').last();


    connect(ui->keyboard,SIGNAL(sendWord(QString)),this,SLOT(setLineEdit(QString)));
    connect(ui->keyboard,SIGNAL(delWord()),this,SLOT(delLineEdit()));

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(check_lineEdit()));
    timer->start(500);

}

QRenameDialog::~QRenameDialog()
{
    delete timer;
    delete ui;
}


//设置标题
void QRenameDialog::setLineEdit(QString name)
{
    //this->name += name;
    QString tmp;
    tmp = ui->lineEdit->text() + name;
    if(tmp.size() > 15)
    {
        tmp = tmp.remove(0,1);
    }
    ui->lineEdit->setText(tmp);
}

void QRenameDialog::delLineEdit()
{
    QString tmp;
    tmp = ui->lineEdit->text();
    if(tmp.isEmpty())
        return;
    tmp = tmp.remove(tmp.size() - 1,1);
    ui->lineEdit->setText(tmp);
}


void QRenameDialog::on_sureBtn_clicked()
{
    foreach(QString name,currentlist->getListItemName())
    {
        if(ui->lineEdit->text() + suffix == name)
        {
            QWarnTipsDialog *wtdlg = new QWarnTipsDialog(NULL,tr("123123213"));
            wtdlg->setWindowModality(Qt::WindowModal);
            wtdlg->move((this->width() - wtdlg->width())/2 , (this->height() - wtdlg->height())/2);
            wtdlg->exec();
            return;
        }
    }

    QIconWidget *item = currentlist->findChild<QIconWidget *>(currentlist->currentItem()->text());
    QString newname = item->getIconPath().remove(item->returnItem()->text()) + ui->lineEdit->text() + suffix;


    if(QFile::rename(item->getIconPath(),newname))
    {
        currentlist->changeListName(item->returnItem()->text(),ui->lineEdit->text() + suffix);
        item->setName(ui->lineEdit->text() + suffix);
        item->setObjectName(ui->lineEdit->text() + suffix);
        item->setIconPath(newname);
        item->returnItem()->setText(ui->lineEdit->text() + suffix);

        emit changeName();
    }

    this->close();
}

void QRenameDialog::on_cancelBtn_clicked()
{
    this->close();
}



void QRenameDialog::check_lineEdit()
{
    if(ui->lineEdit->text().isEmpty())
        ui->sureBtn->setEnabled(false);
    else
        ui->sureBtn->setEnabled(true);
}
