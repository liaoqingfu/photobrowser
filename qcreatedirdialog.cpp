#include "qcreatedirdialog.h"

QCreateDirDialog::QCreateDirDialog(QWidget *parent,QImageListWidget *list) :
    QDialog(parent)
{
    this->setWindowFlags(this->windowFlags() |Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    this->setFixedSize(550,400);
    this->setStyleSheet("background-color: rgb(59, 61, 66, 100);border-radius: 10px;  border: 2px groove black;");

    currentlist = list;

    QFont tittlefont;
    tittlefont.setPixelSize(30);

    tittle = new QLabel(tr("新建文件夹"),NULL);
    tittle->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(68, 119, 180);");
    tittle->setFont(tittlefont);
    tittle->setAlignment(Qt::AlignCenter);

    sureBtn = new QKeyBoardBtn(tr("确定"),30,this);
    sureBtn->setObjectName(tr("44"));
    connect(sureBtn,SIGNAL(clicked()),this,SLOT(on_sureBtn_clicked()));

    cancelBtn = new QKeyBoardBtn(tr("取消"),30,this);
    cancelBtn->setObjectName(tr("45"));
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(on_cancelBtn_clicked()));


    lineEdit = new QLineEdit(NULL);
    lineEdit->setFixedWidth(500);
    QFont font;
    font.setPixelSize(45);
    lineEdit->setFocusPolicy(Qt::NoFocus);
    lineEdit->setFont(font);
    lineEdit->setAlignment(Qt::AlignCenter);
    lineEdit->setStyleSheet("QLineEdit{ color: black; \
                            background-color: white; \
            border-radius: 10px; \
    border-style: outset;}");

//键盘
keyboard = new KeyBoardForm;

//窗体布局
QHBoxLayout  *hlayout1 = new QHBoxLayout;
hlayout1->addStretch();
hlayout1->addWidget(lineEdit);
hlayout1->addStretch();

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

//文件家前缀
lineEdit->setText(tr("DIR_"));

connect(keyboard,SIGNAL(sendWord(QString)),this,SLOT(setLineEdit(QString)));
connect(keyboard,SIGNAL(delWord()),this,SLOT(delLineEdit()));

//检查字符是否存在定时器
timer = new QTimer(this);
connect(timer,SIGNAL(timeout()),this,SLOT(check_lineEdit()));
timer->start(500);

//是否创建
isCreate = true;

QKeyBoardBtn *keybtn = this->findChild<QKeyBoardBtn *>(tr("1"));
keybtn->setFocus();

}

QCreateDirDialog::~QCreateDirDialog()
{
    delete tittle;
    delete keyboard;
    delete sureBtn;
    delete cancelBtn;
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

//删除一个字母
void QCreateDirDialog::delLineEdit()
{
    QString tmp;
    tmp = lineEdit->text();
    if(tmp.isEmpty())
        return;
    tmp = tmp.remove(tmp.size() - 1,1);
    lineEdit->setText(tmp);
}

//确定
void QCreateDirDialog::on_sureBtn_clicked()
{
    if(!isCreate)
        return;

    isCreate = false;

    int newnum = currentlist->getListItemName().count();

    foreach(QString name,currentlist->getListItemName())
    {
        QIconWidget *item = currentlist->findChild<QIconWidget *>(name);

        if(newnum == name.toInt())
        {
            newnum++;
        }

        if(lineEdit->text() == item->getFileName())
        {
            QWarnTipsDialog *wtdlg = new QWarnTipsDialog(NULL,tr("文件夹重复！"));
            wtdlg->setWindowModality(Qt::WindowModal);
            wtdlg->move((this->width() - wtdlg->width())/2 , (this->height() - wtdlg->height())/2);
            wtdlg->exec();

            isCreate = true;

            return;
        }
    }

    QString dir_path = currentlist->getCurrentPath() + tr("/") + lineEdit->text();

    QDir dir;
    if(dir.mkdir(dir_path))
    {

        QIconWidget *iconitem = new QIconWidget(currentlist,newnum,lineEdit->text(),dir_path);

        QPixmap pixmap(tr(":/images/dir.png"));
        iconitem->setIcon(pixmap.scaled(QSize(WICONSIZE,HICONSIZE)));
        currentlist->setItemWidget(iconitem->returnItem(),iconitem);
        currentlist->addListName(iconitem->objectName());
        currentlist->setCurrentItem(iconitem->returnItem());
    }

    this->close();
}

//取消
void QCreateDirDialog::on_cancelBtn_clicked()
{    
    this->close();
}

//关闭事件
void QCreateDirDialog::closeEvent(QCloseEvent *)
{
    emit closeWidget();
}

// 检查是否输入
void QCreateDirDialog::check_lineEdit()
{
    if(lineEdit->text().isEmpty())
        sureBtn->setEnabled(false);
    else
        sureBtn->setEnabled(true);
}

//获取键值
void QCreateDirDialog::getKeyEvent(int key)
{
    if(key == Qt::Key_R)
    {
        this->on_cancelBtn_clicked();
        return;
    }

    QKeyEvent rekey(QEvent::KeyPress,key,Qt::NoModifier);
    QCoreApplication::sendEvent(this,&rekey);
}

//键盘事件
void QCreateDirDialog::keyPressEvent(QKeyEvent *e)
{
    QKeyBoardBtn *item;

    if(e->key() == Qt::Key_Return)
    {
        QCoreApplication::sendEvent(focusWidget(),e);
    }

    if(e->key()== Qt::Key_Left)
    {

        if( focusWidget()->objectName().toInt()  ==  1 )
        {
            item = this->findChild<QKeyBoardBtn *>(tr("45"));
            item->setFocus();
            return  ;
        }

        item = this->findChild<QKeyBoardBtn *>(QString::number(focusWidget()->objectName().toInt( ) - 1));

        if(!item->isEnabled() && (focusWidget()->objectName().toInt( ) >= 2))
               item = this->findChild<QKeyBoardBtn *>(QString::number(focusWidget()->objectName().toInt( ) - 2));

        item->setFocus();

    }
    else if(e->key() == Qt::Key_Right)
    {
        if( focusWidget()->objectName().toInt()  ==  45 )
        {
            item = this->findChild<QKeyBoardBtn *>(tr("1"));
            item->setFocus();
            return  ;
        }

        item = this->findChild<QKeyBoardBtn *>(QString::number(focusWidget()->objectName().toInt( ) + 1));

        if(!item->isEnabled())
               item = this->findChild<QKeyBoardBtn *>(QString::number(focusWidget()->objectName().toInt( ) + 2));

        item->setFocus();
    }
    else if(e->key() == Qt::Key_Up)
    {
        if((focusWidget()->objectName().toInt() <= 10 ) || (focusWidget()->objectName().toInt() >= 41 ))
        {
            if(focusWidget()->objectName().toInt()  ==  1)
            {
                item = this->findChild<QKeyBoardBtn *>(tr("45"));
                item->setFocus();
                return  ;
            }
            //            item = this->findChild<QKeyBoardBtn *>(QString::number(focusWidget()->objectName().toInt( ) - 1));
            //            item->setFocus();
            return ;
        }

        item = this->findChild<QKeyBoardBtn *>(QString::number(focusWidget()->objectName().toInt( ) - 10));
        item->setFocus();
    }

    else if(e->key() == Qt::Key_Down)
    {
        if((focusWidget()->objectName().toInt() >= 31) )
        {
            if(focusWidget()->objectName().toInt()  ==  45)
            {
                item = this->findChild<QKeyBoardBtn *>(tr("1"));
                item->setFocus();
                return  ;
            }

            //            item = this->findChild<QKeyBoardBtn *>(QString::number(focusWidget()->objectName().toInt( ) + 1));
            //            item->setFocus();
            return ;
        }

        if(focusWidget()->objectName().toInt() <= 30)
        {
            item = this->findChild<QKeyBoardBtn *>(QString::number(focusWidget()->objectName().toInt( ) + 10));
            item->setFocus();
        }
    }

    QWidget::keyPressEvent(e);
}
