#ifndef QVIDEOBROWSER_H
#define QVIDEOBROWSER_H

#include <QWidget>
#include <QLabel>
#include <QKeyEvent>

#include "qshowvieowidget.h"

class QVideoBrowser : public QWidget
{
    Q_OBJECT
public:
    explicit QVideoBrowser(QWidget *parent = 0,QString videopath = NULL);
    ~QVideoBrowser();

    void initWidget();


protected:
    void keyPressEvent(QKeyEvent *e);


public slots:
    void updatePlayState(PlayState state);


private:
    QLabel *playlogo;
    QLabel *timeLabel;
    QShowVieoWidget *showvideo;
    QString videopath;

};

#endif // QVIDEOBROWSER_H
