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
#include <QImage>
#include <QThread>
#include <QDebug>
#include <QList>
#include <QMutex>

#define uint8_t  unsigned char
#define CAM_WIDTH 1280
#define CAM_HEIGHT 720

enum PlayState{ Play,Pause,Finish};

extern  QMutex mutex;//生成缩略图线程锁

class VideoDec : public QThread
{
    Q_OBJECT
public:
    explicit VideoDec(QObject *parent = 0, QString name = NULL);
    ~VideoDec();

    void initVideo();

    bool getPauseState()
    {
        return isPause;
    }

    void setPauseState(bool state)
    {
        isPause = state;
    }

    bool getPlayState()
    {
        return isPlay;
    }

    void setPlayState(bool state)
    {
        isPlay = state;
    }


protected:
    void run();

signals:
    void sendFrame(uint8_t * img);
    void sendEndFlag(PlayState state);
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

    bool isPlay;
    bool isPause;

};

#endif // VIDEODEC_H
