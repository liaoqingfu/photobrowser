#include "qwarntipsdialog.h"

QWarnTipsDialog::QWarnTipsDialog(QWidget *parent,QString message) :
    QDialog(parent)
{
    this->setFixedSize(300,160);
    this->setWindowFlags(this->windowFlags() |Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    this->setStyleSheet("background-color: rgb(59, 61, 66, 100);border-radius: 10px;  border: 3px  rgb(59, 61, 66, 100);");

    warnlogo = new QLabel(this);
    warnlogo->setStyleSheet("border-image: url(:/images/warning.png)");
    warnlogo->setFixedSize(61,61);
    warnlogo->move(20,40);

    tipsmessage = new QLabel(this);
    tipsmessage ->setStyleSheet("background-color:transparent; \
                             color: white; ");
    tipsmessage->setFixedSize(160,81);
    tipsmessage->move(100,40);

    QFont font;
    font.setPixelSize(23);
    tipsmessage->setFont(font);
    tipsmessage->setText(message);

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(close()));
    timer->start(1000);

}

QWarnTipsDialog::~QWarnTipsDialog()
{
    delete warnlogo;
    delete tipsmessage;
    delete timer;
}

//关闭事件
void QWarnTipsDialog::closeEvent(QCloseEvent *)
{
    emit closeWidget();
}
