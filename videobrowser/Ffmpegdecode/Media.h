
#ifndef MEDIA_H
#define MEDIA_H

#include <string>
#include "Audio.h"
#include "Video.h"
#include "photobrowser/videobrowser/qshowvideowidget.h"

extern "C" {

#include <libavformat/avformat.h>

}


int decode_thread(void *data);

class VideoState;

class MediaState
{
public:
    MediaState(char *filename);

    ~MediaState();

    AudioState *audio;
    VideoState *video;
    AVFormatContext *pFormatCtx;

    char* filename;
    //bool quit;

    bool openInput();

    QShowVideoWidget *showvideoW;

    int play_state;//0为播放 1为暂停 2为退出
};


#endif
