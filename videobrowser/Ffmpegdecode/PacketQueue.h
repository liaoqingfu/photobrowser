
#ifndef PACKET_QUEUE_H
#define PACKET_QUEUE_H

#include <queue>

#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>

extern "C"{

#include <libavcodec/avcodec.h>

}

class PacketQueue
{
public:
    PacketQueue();

    std::queue<AVPacket> queue;

	Uint32    nb_packets;
	Uint32    size;
	SDL_mutex *mutex;
	SDL_cond  *cond;

    int zero_frame_num;

	bool enQueue(const AVPacket *packet);
	bool deQueue(AVPacket *packet, bool block);
    bool clearQueue();
};

#endif
