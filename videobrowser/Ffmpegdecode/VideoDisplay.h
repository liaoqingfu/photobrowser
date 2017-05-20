
#ifndef VIDEO_DISPLAY_H
#define VIDEO_DISPLAY_H

#include "Video.h"

// 延迟delay ms后刷新video帧
void schedule_refresh(MediaState *media, int delay);

uint32_t sdl_refresh_timer_cb(uint32_t interval, void *opaque);

void video_refresh_timer(void *userdata);

//void video_display(VideoState *video);
void freeSWSContext();

#endif
