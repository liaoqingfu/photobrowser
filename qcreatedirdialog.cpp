#include "qcreatedirdialog.h"
#include "ui_qcreatedirdialog.h"

QCreateDirDialog::QCreateDirDialog(QWidget *parent,QImageListWidget *list) :
    QDialog(parent)
{
    this->setWindowFlags(this->windowFlags() |Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    this->setFixedSize(550,400);
    this->setStyleSheet("background-color: rgb(59, 61, 66, 100);border-radius: 10px;  border: 3px  rgb(59, 61, 66, 100);");

    currentlist = list;

    QFont tittlefont;
    tittlefont.setPixelSize(30);

    tittle = new QLabel(tr("新建文件夹"),NULL);
    tittle->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(68, 119, 180);");
    tittle->setFont(tittlefont);
    tittle->setAlignment(Qt::AlignCenter);

    sureBtn = new QPushButton(tr("确定"));
    connect(sureBtn,SIGNAL(clicked()),this,SLOT(on_sureBtn_clicked()));
    cancelBtn = new QPushButton(tr("取消"));
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(on_cancelBtn_clicked()));

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


    lineEdit = new QLineEdit;
    lineEdit->setFixedWidth(250);
    QFont font;
    font.setPixelSize(45);
    lineEdit->setFocusPolicy(Qt::NoFocus);
    lineEdit->setFont(font);
    lineEdit->setAlignment(Qt::AlignCenter);
    lineEdit->setStyleSheet("QLineEdit{ color: black; \
                            background-color: white; \
                            border-radius: 10px; \
                            border: 2px groove gray;\
                            border-style: outset;}");

    lineEdit->setText(currentlist->currentItem()->text().split('.').first());

    keyboard = new KeyBoardForm;

    QHBoxLayout  *hlayout1 = new QHBoxLayout;
    hlayout1->addSpacing(200);
    hlayout1->addWidget(lineEdit);
    hlayout1->addSpacing(200);

    QHBoxLayout  *hlayout2 = new QHBoxLayout;
    hlayout2->addSpacing(200);
    hlayout2->addWidget(sureBtn);
    hlayout2->addSpacing(200);
    hlayout2->addWidget(cancelBtn);
    hlayout2->addSpacing(200);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(9,9,9,9);
    layout->addWidget(tittle);
    layout->addLayout(hlayout1);
    layout->addWidget(keyboard);
    layout->addLayout(hlayout2);

    this->setLayout(layout);

    lineEdit->setText(tr("新建文件夾"));

    connect(keyboard,SIGNAL(sendWord(QString)),this,SLOT(setLineEdit(QString)));
    connect(keyboard,SIGNAL(delWord()),this,SLOT(delLineEdit()));

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(check_lineEdit()));
    timer->start(500);



}

QCreateDirDialog::~QCreateDirDialog()
{
    delete timer;
}


//设置标题
void QCreateDirDialog::setLineEdit(QString name)
{
    //this->name += name;
    QString tmp;
    tmp = lineEdit->text() + name;
    if(tmp.size() > 15)
    {
        tmp = tmp.remove(0,1);
    }
    lineEdit->setText(tmp);
}

void QCreateDirDialog::delLineEdit()
{
    QString tmp;
    tmp = lineEdit->text();
    if(tmp.isEmpty())
        return;
    tmp = tmp.remove(tmp.size() - 1,1);
    lineEdit->setText(tmp);
}


void QCreateDirDialog::on_sureBtn_clicked()
{
    foreach(QString name,currentlist->getListItemName())
    {
        if(lineEdit->text() == name)
        {
            QWarnTipsDialog *wtdlg = new QWarnTipsDialog(NULL,tr("不能新建相同的文件夾！"));
            wtdlg->setWindowModality(Qt::WindowModal);
            wtdlg->move((this->width() - wtdlg->width())/2 , (this->height() - wtdlg->height())/2);
            wtdlg->exec();
            return;
        }
    }

    QString dir_path = currentlist->getCurrentPath() + tr("/") + lineEdit->text();

    QDir dir;
    if(dir.mkdir(dir_path))
    {
        QIconWidget *iconitem = new QIconWidget(currentlist,0,lineEdit->text(),dir_path);

        QPixmap pixmap(tr(":/images/dir.png"));
        iconitem->setIcon(pixmap.scaled(QSize(WICONSIZE,HICONSIZE)));
        currentlist->setItemWidget(iconitem->returnItem(),iconitem);
        currentlist->addListName(lineEdit->text());
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
    if(lineEdit->text().isEmpty())
        sureBtn->setEnabled(false);
    else
        sureBtn->setEnabled(true);
}
