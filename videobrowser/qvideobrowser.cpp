#include "qvideobrowser.h"

extern "C" {

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>

}

#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>

#include <iostream>

bool quit = false;

//播放线程
int loop(void *data)
{
    QVideoBrowser *videoBrowser = (QVideoBrowser*)data;

    SDL_Event event;
    while (!quit) // SDL event loop
    {
        SDL_WaitEvent(&event);
        switch (event.type)
        {
        case FF_QUIT_EVENT://退出
            printf("退出播放\n");
            videoBrowser->closeVideoPlayer();
            break;

        case FF_REFRESH_EVENT://刷新
            if(videoBrowser->mediaplayer->play_state  == 0)
            {
                video_refresh_timer(videoBrowser->mediaplayer);
            }
            break;

        case FF_PAUSE_EVENT://暂停
            SDL_PauseAudio(1);
            break;

        case FF_PLAY_EVENT://播放
            SDL_PauseAudio(0);
            video_refresh_timer(videoBrowser->mediaplayer);//播放
            break;

        default:
            break;
        }
    }
    printf("close loop\n");
    return 1;
}


//视频播放类
QVideoBrowser::QVideoBrowser(QWidget *parent,QString videopath) :
    QWidget(parent),
    mediaplayer(NULL)
{
    this->setFixedSize(VGA_WIDTH,VGA_HEIGHT);
    this->setWindowFlags(this->windowFlags()  | Qt::FramelessWindowHint);

    //视频路径
    this->videopath = videopath;

    //初始化窗体
    this->initWidget();

    //打开播放器
    this->startVideoPlayer();
}

QVideoBrowser::~QVideoBrowser()
{
    free(filename);
    free(headframe);

    freeSWSContext();

    qDebug()<<"释放内存";
    //释放内存
    delete mediaplayer;

}

//初始化窗体
void QVideoBrowser::initWidget()
{
    showvideo = new QShowVideoWidget(this);
    showvideo->lower();
    showvideo->move(0,0);

    playlogo = new QLabel(this);
    playlogo->resize(120,120);
    playlogo->move((this->width() - 150)/2,(this->height() - 150)/2);
    playlogo->hide();

    //分配视频头一帧
    headframe = (unsigned char *)malloc(VGA_WIDTH*VGA_HEIGHT*3/2);
}


void QVideoBrowser::closeVideoPlayer()
{
    if(mediaplayer != NULL)
    {
        //clean buff
        mediaplayer->video->videoq->clearQueue();
        mediaplayer->audio->audioq.clearQueue();
        mediaplayer->video->frameq.clearQueue();

        quit = true;//外部变量退出

        //SDL_PauseAudio(1);

        SDL_CloseAudio();//关闭音频

        SDL_Quit();//退出SDL

        qDebug()<<"退出SDL";

        this->close();
    }

}

//键盘事件
void QVideoBrowser::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_R)//返回
    {
        SDL_Event event;
        event.type = FF_QUIT_EVENT;
        SDL_PushEvent(&event);
        printf("退出讀码\n");
        return;
    }
    else if(e->key() == Qt::Key_Return)//暂停 播放
    {
        qDebug()<<"暂停 播放";
        if(mediaplayer != NULL)
        {
            if(mediaplayer->play_state == 0)
            {
                mediaplayer->play_state = 1;

                SDL_Event event;//暂停事件
                event.type = FF_PAUSE_EVENT;
                SDL_PushEvent(&event);

                playlogo->setStyleSheet("background-color:transparent;border-image: url(:/images/pause.png)");
                playlogo->show();
            }
            else if(mediaplayer->play_state == 1)
            {
                mediaplayer->play_state = 0;

                SDL_Event event;//播放事件
                event.type = FF_PLAY_EVENT;
                SDL_PushEvent(&event);

                playlogo->hide();
            }
        }
    }

    QWidget::keyPressEvent(e);
}

//获取事件
void QVideoBrowser::getKeyEvent(int key)
{
    if(mediaplayer == NULL)
        return;

    QKeyEvent rekey(QEvent::KeyPress,key,Qt::NoModifier);
    QCoreApplication::sendEvent(this,&rekey);
}

//更新视频开始帧
void  QVideoBrowser::updateVideoHeadFrame(unsigned char * yuv)
{
    //RGB 转 I420
    rgb24_to_yu12(headframe,yuv,VGA_WIDTH,VGA_HEIGHT);
    //显示视频帧
    showvideo->showFrame(headframe);
}

#if 0
//画布事件
void QVideoBrowser::paintEvent(QPaintEvent *e)
{
    QPainter p(this);

    p.drawPixmap(0,0,VGA_WIDTH,VGA_HEIGHT,backgroundpix);
    p.setRenderHint(QPainter::Antialiasing);
}
#endif

//打开视频播放器
void  QVideoBrowser::startVideoPlayer()
{
    av_register_all();//注册所有FFMPEG相关函数

    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER);//注册SDL相关类

    filename = (char*)malloc(100);//文件名
    strcpy(filename,videopath.toLatin1().data());

    mediaplayer = new MediaState((char *)filename);//实例化媒体类

    if (mediaplayer->openInput())//打开媒体文件
    {
        SDL_CreateThread(decode_thread,"", (void *)mediaplayer ); // 创建解码线程，读取packet到队列中缓存
    }
    else
    {
        SDL_Quit();
        this->close();
    }

    qDebug()<<"SDL_CreateThread";
    mediaplayer->audio->audio_play(); // 播放音频

    qDebug()<<"SDL_AudioThread";
    mediaplayer->showvideoW = showvideo;
    mediaplayer->video->video_play(mediaplayer); // 播放视频
    qDebug()<<"SDL_VideoThread";

#if 0
    AVStream *audio_stream = media->pFormatCtx->streams[media->audio->stream_index];
    AVStream *video_stream = media->pFormatCtx->streams[media->video->stream_index];

    double audio_duration = audio_stream->duration * av_q2d(audio_stream->time_base);
    double video_duration = video_stream->duration * av_q2d(video_stream->time_base);

    qDebug() << "audio：" << audio_duration ;
    qDebug() << "video：" << video_duration ;
#endif

    //开始播放
    mediaplayer->play_state = 0;

    //创建播放线程
    SDL_CreateThread(loop,"", (void *)this );

    qDebug()<<"创建播放线程";
}


//关闭事件
void QVideoBrowser::closeEvent(QCloseEvent *)
{
    emit closeWidget();
}


//延时微秒处理
void QVideoBrowser::delayMscTime(int msc)
{
    QTime t=QTime::currentTime().addMSecs(msc);
    while(QTime::currentTime()<t)
    {
        QApplication::processEvents(QEventLoop::AllEvents,100);
    }
}
