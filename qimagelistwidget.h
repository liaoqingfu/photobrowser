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

    void setObjectParent(QWidget *parent);

    void addListName(QString itemname);

    void removeListName(QString itemname);

    void changeListName(QString oldnaeme, QString newname);

    QStringList getListItemName()
    {
        return listItemName;
    }

    void setCurrentPath(QString path)
    {
        currentPath = path;
    }


    QString getCurrentPath()
    {
        return currentPath;
    }

    void setDirSeries(int num)
    {
        dirseries = num;
    }

    int getDirSeies()
    {
        return dirseries;
    }


protected:
    void mouseMoveEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *e);


signals:
    void clickselectList(QImageListWidget *list);

public slots:
    void selectCurrentItem(QListWidgetItem *current, QListWidgetItem *previous);

private:
    bool getPos;

    QPoint m;

    QStringList listItemName;

    QString currentPath;

    int dirseries;

};

#endif // QIMAGELISTWIDGET_H
