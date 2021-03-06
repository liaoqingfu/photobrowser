#include "qimagelistwidget.h"

QImageListWidget::QImageListWidget(QWidget *parent) :
    QListWidget(parent)
{
    getPos = true;

    this->setStyleSheet(tr("background-color:rgb(59, 61, 66, 100);"));
    //设置QImageListWidget中的单元项的图片大小
    this->setIconSize(QSize(WICONSIZE,HICONSIZE));
    this->setResizeMode(QListView::Fixed);//
    //设置QImageListWidget的显示模式
    this->setViewMode(QListView::IconMode);
    //设置QImageListWidget中的单元项不可被拖动
    this->setMovement(QListView::Static);
    //设置QImageListWidget中的单元项的间距
    this->setSpacing(31);

    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //点击选中时，前后状态
    connect(this,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this,SLOT(selectCurrentItem(QListWidgetItem *, QListWidgetItem *)));

}

QImageListWidget:: ~QImageListWidget()
{

}


//键盘事件
void QImageListWidget::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Up)//上移
    {
        if(this->currentRow() <= 3)
        {
            if(this->currentRow() == 0)
                return;

            this->setCurrentRow(this->currentRow() - 1);
            return;
        }

        this->setCurrentRow(this->currentRow() - 4);
    }
    else if(e->key() == Qt::Key_Down)//xia移
    {

        if((this->count() - this->currentRow()) <= 4)
        {
            if((this->count() - 1) == this->currentRow())
            {
                this->scrollToBottom();
                return;
            }

            this->setCurrentRow(this->currentRow() + 1);
            return;
        }

        this->setCurrentRow(this->currentRow() + 4);

    }
    else if(e->key() == Qt::Key_Left)//左移
    {
        if(this->currentRow() == 0)
            return;

        this->setCurrentRow(this->currentRow() - 1);
    }
    else if(e->key() == Qt::Key_Right)//右移
    {

        if((this->count() - 1) == this->currentRow())
        {
            this->scrollToBottom();
            return;
        }

        this->setCurrentRow(this->currentRow() + 1);
    }
    else if(e->key() == Qt::Key_Return)//点击
    {
        if(this->count() !=  0)
              emit itemClicked(this->currentItem());
    }
    else if(e->key() == Qt::Key_R)//返回
    {
        emit  closeTabList();
    }
    else if(e->key() == Qt::Key_M)//菜单
    {
        emit clickselectList(this);
    }

    QWidget::keyPressEvent(e);
}


//删除列表
void QImageListWidget::deleteListWidget()
{
    foreach (QString itemname, listItemName) {
        qDebug()<<itemname;
        QIconWidget *item = this->findChild<QIconWidget *>(itemname);
        this->removeItemWidget(item->returnItem());
        delete item->returnItem();
        delete item;
    }

    listItemName.clear();
    this->clear();
}


//删除列表的一个
void QImageListWidget::removeListName(QString itemname)
{
    listItemName.removeOne(itemname);
}

//加入列表的一个
void QImageListWidget::addListName(QString itemname)
{
    listItemName<<itemname;
}

//改变列表一个名称
void QImageListWidget::changeListName(QString oldname, QString newname)
{
    if(listItemName.removeOne(oldname))
    {
        listItemName<<newname;
    }
    qDebug()<<listItemName;
}

//鼠标移动事件
void QImageListWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(getPos)
    {
        if(event->pos().y() < 300)
        {
            m =event->pos();
            getPos = false;
        }
        else if(event->pos().y() > 300)
        {
            m =event->pos();
            getPos = false;
        }

    }

    if((m.y() + 20) <= event->pos().y())
    {

        if(this->currentRow()/7 < this->count()/7 - 1)
        {
            this->setCurrentRow(this->currentRow()+7);
        }
        else
        {
            if(this->currentRow() != this->count()-1)
                this->setCurrentRow(this->currentRow()+ 1);
        }
        getPos = true;
    }
    else if((m.y() - 20) >= event->pos().y())
    {
        if((this->count() -this->currentRow())/7 < this->count()/7 - 1)
        {
            this->setCurrentRow(this->currentRow()-7);
        }
        else
        {
            if(this->currentRow() != 0)
                this->setCurrentRow(this->currentRow()- 1);
        }
        getPos = true;
    }

}


//选择当前项目
void QImageListWidget::selectCurrentItem(QListWidgetItem *current, QListWidgetItem *previous)
{

    if(previous != NULL)
    {
        QIconWidget *pitem = this->findChild<QIconWidget *>(previous->text());
        if(pitem != NULL)
            pitem->setBorderColor("QLabel{background: transparent;}");
    }

    if(current != NULL)
    {
        QIconWidget *citem = this->findChild<QIconWidget *>(current->text());
        if(citem != NULL)
            citem->setBorderColor("QLabel{background: transparent;border-width:5px;border-color:rgb(85, 170, 255);;border-style: solid;border-radius:3}");
    }
}

