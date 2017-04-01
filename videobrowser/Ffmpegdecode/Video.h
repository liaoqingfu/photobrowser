
#ifndef VIDEO_H
#define VIDEO_H

#define FF_REFRESH_EVENT (SDL_USEREVENT)
#define FF_QUIT_EVENT (SDL_USEREVENT + 1)
#define FF_PAUSE_EVENT (SDL_USEREVENT + 2)
#define FF_PLAY_EVENT (SDL_USEREVENT + 3)

#include "PacketQueue.h"
#include "FrameQueue.h"
#include "Media.h"
#include <SDL2/SDL_video.h>

class  MediaState;
/**
 * ������Ƶ��������ݷ�װ
 */
class VideoState
{
public:
    VideoState();

    ~VideoState();

    PacketQueue* videoq;        // �����video packet�Ķ��л���

    int stream_index;           // index of video stream
    AVCodecContext *video_ctx;  // have already be opened by avcodec_open2
    AVStream *stream;           // video stream

    FrameQueue frameq;          // ���������ԭʼ֡����
    AVFrame *frame;
    AVFrame *displayFrame;

    double frame_timer;         // Sync fields
    double frame_last_pts;
    double frame_last_delay;
    double video_clock;

#if 0
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *bmp;
    SDL_Rect rect;
#endif

    uint8_t *buffer;
    void video_play(MediaState *media);

    double synchronize(AVFrame *srcFrame, double pts);

};


int decode(void *arg); // ��packet���룬����������Frame����FrameQueue������


#endif
