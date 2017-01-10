#ifndef QSHOWVIEOWIDGET_H
#define QSHOWVIEOWIDGET_H

#include <QWidget>
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


#include <gst/gst.h>
#include <gst/app/app.h>
#include <gst/app/gstappsrc.h>
#include <gst/app/gstappsink.h>

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

#include "PlayWidgetcpy.h"
#include <videobrowser/videodec.h>


/* Structure to contain all our information, so we can pass it around */
typedef struct _CustomData {
    GstElement *pipeline;  /* Our one and only element */
    GstElement *sink;      /* connect element */
    gboolean playing;      /* Are we in the PLAYING state? */
    gboolean terminate;    /* Should we terminate execution? */
    gboolean seek_enabled; /* Is seeking enabled for this media? */
    gboolean seek_done;    /* Have we performed the seek already? */
    gint64 duration;       /* How long does this media last, in nanoseconds */
} CustomData;


class QShowVieoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QShowVieoWidget(QWidget *parent = 0,QString videopath = NULL);
    ~QShowVieoWidget();
signals:
        void sendEndFlag(PlayState state);

public slots:
    void updateVideoFrame(uint8_t *yuv);

public:
    VideoDec *OVideoDec;

private:

    CPlayWidget *playWidget;
    QString videopath;

    bool isPlay;




};

#endif // QSHOWVIEOWIDGET_H
