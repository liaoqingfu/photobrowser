
#include "PacketQueue.h"
#include <iostream>
#include <QDebug>
extern bool quit;

PacketQueue::PacketQueue()
{
    nb_packets = 0;
    size       = 0;
    zero_frame_num = 0;
    mutex      = SDL_CreateMutex();
    cond       = SDL_CreateCond();
}


bool PacketQueue::enQueue(const AVPacket *packet)
{
    AVPacket *pkt = av_packet_alloc();
    if (av_packet_ref(pkt, packet) < 0)
        return false;

    SDL_LockMutex(mutex);
    queue.push(*pkt);

    size += pkt->size;
    nb_packets++;

    SDL_CondSignal(cond);
    SDL_UnlockMutex(mutex);
    return true;
}

bool PacketQueue::deQueue(AVPacket *packet, bool block)
{
    bool ret = false;

    SDL_LockMutex(mutex);
    while (!quit)
    {

        if (!queue.empty())
        {
            if (av_packet_ref(packet, &queue.front()) < 0)
            {
                ret = false;
                break;
            }
            AVPacket pkt = queue.front();

            queue.pop();
            av_packet_unref(&pkt);
            av_free_packet(&pkt);
            nb_packets--;
            size -= packet->size;

            ret = true;
            break;
        }
        else if (!block)
        {
            ret = false;
            break;
        }
        else
        {
            SDL_CondWait(cond, mutex);
            //SDL_CondWaitTimeout(cond,mutex,10);
        }
    }

    SDL_UnlockMutex(mutex);
    return ret;
}


bool PacketQueue::clearQueue()
{
     SDL_LockMutex(mutex);
    while(!queue.empty())
    {
        AVPacket pkt = queue.front();

        queue.pop();
        av_packet_unref(&pkt);
        av_free_packet(&pkt);
        nb_packets--;

        if(nb_packets == 0)
            break;
    }
     SDL_UnlockMutex(mutex);
    return true;
}
