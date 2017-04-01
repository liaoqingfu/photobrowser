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

    //图片多选
    enum IconStates{ Select,NoSelect,Hide };

    //
    enum DirStates{ Show,Hiden };

    //设置图片路径
    void setIconPath(QString path);

    //设置边框颜色
    void setBorderColor(QString);

    //设置图片名称
    void setName(QString name);

    //设置缩略图
    void setIcon(QPixmap pixmap);

    //设置图片选中状态
    void setCheckState(IconStates state);

    //设置图片选中状态
    void setDirState(DirStates state);

    //返回列表项目
    QListWidgetItem *returnItem();

    //获取图片路径
    QString getIconPath();

    //返回图片状态
    IconStates getIconStates()
    {
        return isStates;
    }

    QString getFileName()
    {
        return this->filename;
    }

    void setFileName(QString name)
    {
        this->filename = name;
    }

private:
    //存放图片缩略图
    QLabel *label;
    //边框
    QLabel *borderlabel;
    //名称
    QLabel *namelabel;
    //默认路径
     QLabel *defaultdirlabel;
    //列表项目
    QListWidgetItem *item;
    //图片状态标示
    QLabel *checkstate ;
    //视频状态
    QLabel *videostate;
    //图片路径
    QString iconpath;
    //图片后缀
    QString videosuffix;
    //图片状态
    IconStates isStates;

    QString filename;

};

#endif // QICONWIDGET_H
