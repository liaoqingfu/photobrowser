#ifndef QIMAGELISTWIDGET_H
#define QIMAGELISTWIDGET_H

#include <QListWidget>
#include <QKeyEvent>
#include <QMouseEvent>
#include <qmenu.h>
#include <qaction.h>
#include <QPoint>
#include <QDebug>
#include "qiconwidget.h"


class QImageListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit QImageListWidget(QWidget *parent = 0);
    ~QImageListWidget();

    //设置对象名称
    void setObjectParent(QWidget *parent);

    //加入列表的一个
    void addListName(QString itemname);

    //删除列表的一个
    void removeListName(QString itemname);

    //改变列表的一个名称
    void changeListName(QString oldnaeme, QString newname);

    //获取列表名称
    QStringList getListItemName()
    {
        return listItemName;
    }

    //设置当前路径
    void setCurrentPath(QString path)
    {
        currentPath = path;
    }

    //获取当前路径
    QString getCurrentPath()
    {
        return currentPath;
    }

    //设置当前目录级数
    void setDirSeries(int num)
    {
        dirseries = num;
    }

    //获取当前目录级数
    int getDirSeies()
    {
        return dirseries;
    }


protected:
    //鼠标移动事件
    void mouseMoveEvent(QMouseEvent *event);

    //键盘事件
    void keyPressEvent(QKeyEvent *e);


signals:
    //点击选择事件
    void clickselectList(QImageListWidget *list);

public slots:
    //选择当前项目
    void selectCurrentItem(QListWidgetItem *current, QListWidgetItem *previous);

private:
    //是否鼠标位置
    bool getPos;

    //是否位置
    QPoint m;

    //列表名称
    QStringList listItemName;

    //当前路径
    QString currentPath;

    //目录级数
    int dirseries;

};

#endif // QIMAGELISTWIDGET_H
