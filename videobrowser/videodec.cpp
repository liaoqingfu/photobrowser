#include "videodec.h"

//QMutex mutex;//生成缩略图线程锁

VideoDec::VideoDec(QObject *parent, QString name) :
    QThread(parent)
{
    //注册类型
    qRegisterMetaType<PlayState >("PlayState");
    out_buffer = (uint8_t *)malloc(CAM_WIDTH * CAM_HEIGHT * 3 / 2);
    FileName = name ;
}


VideoDec::~VideoDec()
{
    free(out_buffer);
}


void VideoDec::initVideo()
{

    pFormatCtx = avformat_alloc_context();

    av_register_all();

    if(avformat_open_input(&pFormatCtx, FileName.toLatin1().data(), NULL, NULL)!=0)
    {
        printf("error!\n");
        return ;
    }

    if(avformat_find_stream_info(pFormatCtx,NULL)<0)
    {
        printf("error!\n");
        return ;
    }

    if(pFormatCtx->duration != AV_NOPTS_VALUE){
        int hours, mins, secs, us;
        int64_t duration = pFormatCtx->duration + 5000;
        secs = duration / AV_TIME_BASE;
        us = duration % AV_TIME_BASE;
        mins = secs / 60;
        secs %= 60;
        hours = mins/ 60;
        mins %= 60;
        qDebug("%02d:%02d:%02d.%02d\n", hours, mins, secs, (100 * us) / AV_TIME_BASE);

    }

    for(int i=0; i<pFormatCtx->nb_streams; i++)
        if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO)
        {
            videoStream=i;
            break;
        }
    if(videoStream==-1)
    {
        printf("error!\n");
    }

    // get context point
    pCodecCtx=pFormatCtx->streams[videoStream]->codec;

    AVCodec *pCodec;

    //  search decoder
    pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec==NULL)
        printf("error!\n");// 找不到解码器

    // open decoder
    if(avcodec_open2(pCodecCtx, pCodec,NULL)<0)
        printf("error!\n"); // 打不开解码器


    pFrame=av_frame_alloc();
    pFrameRGB =av_frame_alloc();

    avpicture_fill((AVPicture *)pFrameRGB, out_buffer, AV_PIX_FMT_YUV420P,CAM_WIDTH, CAM_HEIGHT);
    pSWSCtx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, CAM_WIDTH,CAM_HEIGHT, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

    isPlay = true;
    isPause = false;

    play();
}
void VideoDec::play()
{
    int frameFinished = 1;

    qDebug()<<"start play index"<<videoStream;

    while(isPlay)
    {
        QThread::msleep(1);\

        if(isPause)
        {
            continue;
        }

        if(!(av_read_frame(pFormatCtx,&packet)>=0))
        {
            isPlay = false;
            break;
        }

        if(packet.stream_index==videoStream)
        {
            avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished,&packet);

            sws_scale(pSWSCtx, pFrame->data, pFrame->linesize,0, pCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize);

            emit sendFrame(out_buffer);
        }
    }
    av_free_packet(&packet);

    //free RGB
    av_free(pFrameRGB);
    //free YUV
    av_free(pFrame);

    sws_freeContext(pSWSCtx);

    // close（codec）
    avcodec_close(pCodecCtx);

    // close video file
    avformat_close_input(&pFormatCtx);

    emit sendEndFlag(Finish);
}


void VideoDec::run()
{
    initVideo();
}
