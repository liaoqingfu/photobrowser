
#include "Media.h"
#include <iostream>
#include <QDebug>
extern "C"{
#include <libavutil/time.h>
}
extern bool quit;
int errorflag;
MediaState::MediaState(char* input_file)
{
    filename = input_file;
    pFormatCtx = NULL;

    audio = new AudioState();

    video = new VideoState();
    quit = false;
}

MediaState::~MediaState()
{
    if(audio)
    {
        delete audio;
        qDebug()<<"delete audio";
    }


    if (video)
    {
        delete video;
        qDebug()<<"delete video";
    }

    avformat_close_input(&pFormatCtx);
    avformat_free_context(pFormatCtx);

    pFormatCtx = NULL;
}

bool MediaState::openInput()
{
    // Open input file
    if (avformat_open_input(&pFormatCtx, filename, NULL, NULL) < 0)
        return false;

    if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
        return false;

    // Output the stream info to standard
    av_dump_format(pFormatCtx, 0, filename, 0);

    for (uint32_t i = 0; i < pFormatCtx->nb_streams; i++)
    {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO && audio->stream_index < 0)
            audio->stream_index = i;

        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO && video->stream_index < 0)
            video->stream_index = i;
    }

    if (audio->stream_index < 0 || video->stream_index < 0)
        return false;

    // Fill audio state
    AVCodec *pCodec = avcodec_find_decoder(pFormatCtx->streams[audio->stream_index]->codec->codec_id);
    if (!pCodec)
        return false;

    audio->stream = pFormatCtx->streams[audio->stream_index];

    audio->audio_ctx = avcodec_alloc_context3(pCodec);
    if (avcodec_copy_context(audio->audio_ctx, pFormatCtx->streams[audio->stream_index]->codec) != 0)
        return false;

    avcodec_open2(audio->audio_ctx, pCodec, NULL);

    // Fill video state
    AVCodec *pVCodec = avcodec_find_decoder(pFormatCtx->streams[video->stream_index]->codec->codec_id);
    if (!pVCodec)
        return false;

    video->stream = pFormatCtx->streams[video->stream_index];

    video->video_ctx = avcodec_alloc_context3(pVCodec);
    if (avcodec_copy_context(video->video_ctx, pFormatCtx->streams[video->stream_index]->codec) != 0)
        return false;

    avcodec_open2(video->video_ctx, pVCodec, NULL);

    video->frame_timer = static_cast<double>(av_gettime()) / 1000000.0;
    video->frame_last_delay = 40e-3;

    return true;
}

int decode_thread(void *data)
{
    MediaState *media = (MediaState*)data;

    AVPacket *packet = av_packet_alloc();

    SDL_Delay(100);

    while (!quit)
    {
#if 1
        if(media->video->videoq->size > MAX_VIDEO_QUEUE_NUM || \
                media->audio->audioq.size > MAX_AUDIO_QUEUE_NUM)
        {
            // printf("缓冲帧数过多\n");
            SDL_Delay(10);
            continue;
        }
#endif

        int ret = av_read_frame(media->pFormatCtx, packet);

#if 0
        if (ret < 0)
        {
            if ((media->video->videoq->size  == 0) &&  (media->audio->audioq.size == 0 ) && (media->video->frameq\
                                                                                             .nb_frames == 0) )
            {
                errorflag = -1;//检测空帧
            }

            //errorflag = -1;

            if (errorflag  ==  -1)
            {
                SDL_Event event;
                event.type = FF_QUIT_EVENT;
                SDL_PushEvent(&event);

                printf("退出讀码\n");
                break;
            }

            if (media->pFormatCtx->pb->error == 0) // No error,wait for user input
            {
                SDL_Delay(100);
                continue;
            }
            else
                break;
        }
#else

        if(ret < 0)
        {
            if(ret == AVERROR_EOF || url_feof(media->pFormatCtx->pb))
            {
                if(media->video->videoq->size == 0 || media->audio->audioq.size == 0)
                {
                    SDL_Event event;
                    event.type = FF_QUIT_EVENT;
                    SDL_PushEvent(&event);
                    printf("退出讀码\n");
                    qDebug()<<media->video->videoq->size << ":"  <<media->audio->audioq.size<< ":" <<  media->video->frameq.nb_frames;
                    qDebug("quit1!\n");
                    break;
                }
            }

            if(media->pFormatCtx->pb && media->pFormatCtx->pb->error)
            {
                if(media->video->videoq->size == 0 || media->audio->audioq.size == 0)
                {
                    SDL_Event event;
                    event.type = FF_QUIT_EVENT;
                    SDL_PushEvent(&event);
                    printf("退出讀码\n");
                    qDebug("quit2!\n");
                    break;
                }
            }
            continue;
        }

#endif

        errorflag = 0 ;

        if (packet->stream_index == media->audio->stream_index) // audio stream
        {
            media->audio->audioq.enQueue(packet);
            av_packet_unref(packet);
        }
        else if (packet->stream_index == media->video->stream_index) // video stream
        {
            media->video->videoq->enQueue(packet);
            av_packet_unref(packet);
        }
        else
            av_packet_unref(packet);
    }

    qDebug()<<"close decode_thread";
    av_packet_free(&packet);

    return 0;
}
