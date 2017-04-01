#ifndef VIDEODEC_H
#define VIDEODEC_H
extern "C"{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libavutil/mem.h"
#include "libavutil/fifo.h"
#include "libswscale/swscale.h"
}
#include <QObject>
#include <QApplication>
#include <QImage>
#include <QThread>
#include <QDebug>
#include <QList>
#include <QMutex>
#include <QTimer>

#define VGA_WIDTH 640
#define VGA_HEIGHT 480

#define uint8_t  unsigned char

enum PlayState{ Play,Pause,Finish};

extern  QMutex mutex;//生成缩略图线程锁

class VideoDec : public QThread
{
    Q_OBJECT
public:
    explicit VideoDec(QObject *parent = 0, QString name = NULL);
    ~VideoDec();

    void initVideo();

    void closeVideo();


    bool getPlayState()
    {
        return playTimer->isActive();
    }

    void setPauseState()
    {
        if(playTimer->isActive())
            playTimer->stop();
        else
            playTimer->start();
    }

    QString videoCountTime;


signals:
    void sendFrame(uint8_t * img);
    void updatePlayState(PlayState state);
public slots:

    void play();

private:
    AVFormatContext *pFormatCtx;
    AVCodecContext *pCodecCtx;
    AVCodec *pCodec;
    AVPacket packet;
    AVFrame *pFrame,*pFrameRGB;
    QString FileName ;
    QList<QImage> VideoImg;
    SwsContext* pSWSCtx;

    uint8_t *out_buffer;
    int videoStream;

    int frameFinished;

    QTimer *playTimer;

};

#endif // VIDEODEC_H
