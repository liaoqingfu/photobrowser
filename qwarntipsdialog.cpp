#include "qwarntipsdialog.h"
#include "ui_qwarntipsdialog.h"

QWarnTipsDialog::QWarnTipsDialog(QWidget *parent,QString message) :
    QDialog(parent),
    ui(new Ui::QWarnTipsDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() |Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

    QFont font;
    font.setPixelSize(18);
    ui->tipsmessage->setFont(font);
    ui->tipsmessage->setText(message);

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(close()));
    timer->start(2000);

}

QWarnTipsDialog::~QWarnTipsDialog()
{
    delete ui;
}
