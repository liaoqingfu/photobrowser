#ifndef QICONWIDGET_H
#define QICONWIDGET_H

#include <QWidget>
#include <qdialog.h>
#include <QWidget>
#include <QListWidgetItem>
#include <QDebug>
#include <QLabel>


#define WICONSIZE 120
#define HICONSIZE 120


class QIconWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QIconWidget(QWidget *parent = 0,int num = 0,QString filename = NULL,QString path = NULL);
    ~QIconWidget();

    enum IconStates{ Select,NoSelect,Hide };


    void setIconPath(QString path);

    void setBorderColor(QString);

    void setName(QString name);

    void setIcon(QPixmap pixmap);

    void setCheckState(IconStates state);

    QListWidgetItem *returnItem();

    QString getIconPath();

    IconStates getIconStates()
    {
        return isStates;
    }

private:

    QLabel *label;
    QLabel *borderlabel;
    QLabel *namelabel;

    QListWidgetItem *item;
    QLabel *checkstate ;
    QLabel *videostate;

    QString iconpath;
    QString videosuffix;

    IconStates isStates;

};

#endif // QICONWIDGET_H
