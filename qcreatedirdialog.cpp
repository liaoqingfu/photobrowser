#include "qcreatedirdialog.h"
#include "ui_qcreatedirdialog.h"

QCreateDirDialog::QCreateDirDialog(QWidget *parent,QImageListWidget *list) :
    QDialog(parent),
    ui(new Ui::QCreateDirDialog)
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

    ui->lineEdit->setText(tr("newdir"));

    connect(ui->keyboard,SIGNAL(sendWord(QString)),this,SLOT(setLineEdit(QString)));
    connect(ui->keyboard,SIGNAL(delWord()),this,SLOT(delLineEdit()));

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(check_lineEdit()));
    timer->start(500);


}

QCreateDirDialog::~QCreateDirDialog()
{
    delete timer;
    delete ui;
}


//设置标题
void QCreateDirDialog::setLineEdit(QString name)
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

void QCreateDirDialog::delLineEdit()
{
    QString tmp;
    tmp = ui->lineEdit->text();
    if(tmp.isEmpty())
        return;
    tmp = tmp.remove(tmp.size() - 1,1);
    ui->lineEdit->setText(tmp);
}


void QCreateDirDialog::on_sureBtn_clicked()
{
    foreach(QString name,currentlist->getListItemName())
    {
        if(ui->lineEdit->text() == name)
        {
            QWarnTipsDialog *wtdlg = new QWarnTipsDialog(NULL,tr("123123213"));
            wtdlg->setWindowModality(Qt::WindowModal);
            wtdlg->move((this->width() - wtdlg->width())/2 , (this->height() - wtdlg->height())/2);
            wtdlg->exec();
            return;
        }
    }

    QString dir_path = currentlist->getCurrentPath() + tr("/") + ui->lineEdit->text();

    QDir dir;
    if(dir.mkdir(dir_path))
    {
        QIconWidget *iconitem = new QIconWidget(currentlist,0,ui->lineEdit->text(),dir_path);

        QPixmap pixmap(tr(":/images/dir.png"));
        iconitem->setIcon(pixmap.scaled(QSize(WICONSIZE,HICONSIZE)));
        currentlist->setItemWidget(iconitem->returnItem(),iconitem);
        currentlist->addListName(ui->lineEdit->text());
        currentlist->setCurrentItem(iconitem->returnItem());
    }

    this->close();
}

void QCreateDirDialog::on_cancelBtn_clicked()
{
    this->close();
}


void QCreateDirDialog::check_lineEdit()
{
    if(ui->lineEdit->text().isEmpty())
        ui->sureBtn->setEnabled(false);
    else
        ui->sureBtn->setEnabled(true);
}
