#include "qrenamedialog.h"

QRenameDialog::QRenameDialog(QWidget *parent,QImageListWidget *list) :
    QDialog(parent)
{
    this->setWindowFlags(this->windowFlags() |Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    this->setFixedSize(550,400);
    this->setStyleSheet("background-color: rgb(59, 61, 66, 100);border-radius: 10px;  border: 2px groove black;");

    currentlist = list;

    QFont tittlefont;
    tittlefont.setPixelSize(30);

    tittle = new QLabel(tr("重命名"),NULL);
    tittle->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(68, 119, 180);");
    tittle->setFont(tittlefont);
    tittle->setAlignment(Qt::AlignCenter);

    sureBtn = new QKeyBoardBtn(tr("确定"),30,this);
    sureBtn->setObjectName(tr("44"));
    connect(sureBtn,SIGNAL(clicked()),this,SLOT(on_sureBtn_clicked()));
    cancelBtn = new QKeyBoardBtn(tr("取消"),30,this);
    cancelBtn->setObjectName(tr("45"));
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(on_cancelBtn_clicked()));


    lineEdit = new QLineEdit;
    lineEdit->setFixedWidth(500);
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

//显示当前名字
QIconWidget *item = currentlist->findChild<QIconWidget *>(currentlist->currentItem()->text());
lineEdit->setText(item->getFileName().split('.').first());

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

//隐藏后缀
if(item->getFileName().right(4).left(1) == ".")
suffix = tr(".") + item->getFileName().split('.').last();

connect(keyboard,SIGNAL(sendWord(QString)),this,SLOT(setLineEdit(QString)));
connect(keyboard,SIGNAL(delWord()),this,SLOT(delLineEdit()));

//检查字符是否存在
timer = new QTimer(this);
connect(timer,SIGNAL(timeout()),this,SLOT(check_lineEdit()));
timer->start(500);

//是否命名
isRename = true;

QKeyBoardBtn *keybtn = this->findChild<QKeyBoardBtn *>(tr("1"));
keybtn->setFocus();
}

QRenameDialog::~QRenameDialog()
{
    delete tittle;
    delete keyboard;
    delete sureBtn;
    delete cancelBtn;
    delete timer;
}


//设置标题
void QRenameDialog::setLineEdit(QString name)
{
    QString tmp;
    tmp = lineEdit->text() + name;
    if(tmp.size() > 15)
    {
        tmp = tmp.remove(0,1);
    }
    lineEdit->setText(tmp);
}

//删除一个字母
void QRenameDialog::delLineEdit()
{
    QString tmp;
    tmp = lineEdit->text();
    if(tmp.isEmpty())
        return;
    tmp = tmp.remove(tmp.size() - 1,1);
    lineEdit->setText(tmp);
}

//确定
void QRenameDialog::on_sureBtn_clicked()
{
    if(!isRename)
        return;

    isRename = false;

    QIconWidget *currentitem = currentlist->findChild<QIconWidget *>(currentlist->currentItem()->text());

    if( currentitem->getFileName() != (lineEdit->text() + suffix) )
        foreach(QString name,currentlist->getListItemName())
        {
            QIconWidget *item = currentlist->findChild<QIconWidget *>(name);
            if((lineEdit->text() + suffix) == item->getFileName())
            {
                QWarnTipsDialog *wtdlg = new QWarnTipsDialog(NULL,tr("命名重复！"));
                wtdlg->setWindowModality(Qt::WindowModal);
                wtdlg->move((this->width() - wtdlg->width())/2 , (this->height() - wtdlg->height())/2);
                wtdlg->exec();

                isRename = true;

                return;
            }
        }

    QString newname = currentitem->getIconPath().remove(currentitem->getFileName()) + lineEdit->text() + suffix;

    if(QFile::rename(currentitem->getIconPath(),newname))
    {
        currentitem->setName(lineEdit->text() + suffix);
        currentitem->setIconPath(newname);

        emit changeName();
    }

    this->close();
}

//取消
void QRenameDialog::on_cancelBtn_clicked()
{
    this->close();
}

//关闭事件
void QRenameDialog::closeEvent(QCloseEvent *)
{
    emit closeWidget();
}

// 检查是否输入
void QRenameDialog::check_lineEdit()
{
    if(lineEdit->text().isEmpty())
        sureBtn->setEnabled(false);
    else
        sureBtn->setEnabled(true);
}

//获取键值
void QRenameDialog::getKeyEvent(int key)
{
    if(key == Qt::Key_R)
    {
        this->on_cancelBtn_clicked();
        return;
    }
    qDebug()<<"key presserd"<<key;

    QKeyEvent rekey(QEvent::KeyPress,key,Qt::NoModifier);
    QCoreApplication::sendEvent(this,&rekey);
}



//键盘事件
void QRenameDialog::keyPressEvent(QKeyEvent *e)
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

