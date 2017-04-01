#ifndef QVIDEOBROWSER_H
#define QVIDEOBROWSER_H

#include <QWidget>
#include <QLabel>
#include <QKeyEvent>
#include <QImage>
#include <QPaintEvent>
#include <QPainter>
#include <QApplication>
#include <QTimer>
#include <QPushButton>
#include <QtDebug>
#include <QTime>
#include <QKeyEvent>
#include <QThread>
#include <QLabel>


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "jpeglib.h"
#include "jerror.h"
#include "photobrowser/videoutil.h"
#include "photobrowser/videobrowser/Ffmpegdecode/PacketQueue.h"
#include "photobrowser/videobrowser/Ffmpegdecode/Audio.h"
#include "photobrowser/videobrowser/Ffmpegdecode/Media.h"
#include "photobrowser/videobrowser/Ffmpegdecode/VideoDisplay.h"


#include "qshowvieowidget.h"

class QVideoBrowser : public QWidget
{
    Q_OBJECT
public:
    explicit QVideoBrowser(QWidget *parent = 0,QString videopath = NULL);
    ~QVideoBrowser();

    void initWidget();

    void updateVideoHeadFrame(unsigned char * yuv);

    void startVideoPlayer();

    void closeVideoPlayer();

    void delayMscTime(int msc);

signals:
    void closeWidget();

protected:
    void keyPressEvent(QKeyEvent *e);

    void closeEvent(QCloseEvent *);

    //画布事件
    //void paintEvent(QPaintEvent *e);



public slots:

    void getKeyEvent(int key);

public :
    MediaState *mediaplayer;


private:
    QLabel *playlogo;
    QString videopath;
    QShowVieoWidget *showvideo;

    unsigned char *headframe;
    char * filename;
};

#endif // QVIDEOBROWSER_H
