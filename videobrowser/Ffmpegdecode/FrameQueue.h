
#ifndef FRAME_QUEUE_H
#define FRAME_QUEUE_H

#include <SDL2/SDL_thread.h>
#include <queue>

#define MAX_VIDEO_QUEUE_NUM 64
#define MAX_AUDIO_QUEUE_NUM 128
#define ERROR_NULL_FRAME 50

extern "C"{

#include <libavcodec/avcodec.h>

}

class FrameQueue
{
public:
    FrameQueue();

	static const int capacity = 30;

	std::queue<AVFrame*> queue;

	uint32_t nb_frames;

	SDL_mutex* mutex;
	SDL_cond * cond;

	bool enQueue(const AVFrame* frame);
	bool deQueue(AVFrame **frame);
    bool clearQueue();
};



#endif
