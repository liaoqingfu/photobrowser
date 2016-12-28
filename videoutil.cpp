#include "videoutil.h"
#include <stdio.h>


extern "C"
{
#include "libswscale/swscale.h"
#include "libavutil/opt.h"
#include "libavutil/imgutils.h"
#include "libavcodec/avcodec.h"
#include "libavutil/file.h"
#include "linux/videodev2.h"
#include "libavformat/avformat.h"
#include<stdlib.h>
#include<string.h>
#include <linux/videodev2.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <setjmp.h>
#include <sys/stat.h>
#include <assert.h>
};


//get frist frame of video
int get_Video_Frame(char *filename,uint8_t *rgbframe,int dstw,int dsth)
{
    AVFormatContext *pFormatCtx;
    AVCodecContext * pCodecCtx;

    pFormatCtx = avformat_alloc_context();

    av_register_all();

    if(avformat_open_input(&pFormatCtx, filename, NULL, NULL)!=0)
    {
        printf("error!\n");
        return -1;
    }

    if(avformat_find_stream_info(pFormatCtx,NULL)<0)
    {
        printf("error!\n");
        return -1;
    }

    int videoStream=-1;

    for(int i=0; i<pFormatCtx->nb_streams; i++)
        if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO)
        {
            videoStream=i;
            break;
        }
    if(videoStream==-1)
    {
        printf("error!\n");
    }// Didn't find a video stream

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

    AVPacket packet;
    AVFrame*   pFrame=avcodec_alloc_frame();
    AVFrame *pFrameRGB = avcodec_alloc_frame();
    //int numBytes=avpicture_get_size(AV_PIX_FMT_RGB24, pCodecCtx->width,pCodecCtx->height);
    //uint8_t *buffer=new uint8_t[numBytes];
    avpicture_fill((AVPicture *)pFrameRGB, rgbframe, AV_PIX_FMT_RGB24,dstw, dsth);
    SwsContext* pSWSCtx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, dstw,dsth, AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);

    int  i=0;
    int frameFinished = 0;
    while(av_read_frame(pFormatCtx,&packet)>=0)
    {

        if(packet.stream_index==videoStream)
        {
            avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished,&packet);
            //            if(frameFinished)
            //            {
            //                if(pFrame->key_frame==1) // 这就是关键帧
            //                {
            sws_scale(pSWSCtx, pFrame->data, pFrame->linesize,0, pCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize);
            //                    i++;
            //                }
            //            }
            break;
        }
        av_free_packet(&packet);
    }


    //free RGB
    av_free(pFrameRGB);
    //free YUV
    av_free(pFrame);

    sws_freeContext(pSWSCtx);


    // close（codec）
    avcodec_close(pCodecCtx);

    // close video file
    avformat_close_input(&pFormatCtx);

    return 1;

}



//crop yv12 frame
int crop_frame(uint8_t *dst,uint8_t *src,int top,int left,int srcwidth,int srcheight,int cropwidth,int cropheight)
{
    assert(src != NULL);
    assert(dst != NULL);

    AVPicture srcpic,dstpic;

    AVPixelFormat pixmat = AV_PIX_FMT_YUV420P;

    avpicture_fill(&srcpic,src,pixmat,srcwidth,srcheight);
    avpicture_fill(&dstpic,dst,pixmat,cropwidth,cropheight);

    int croplen = 0;
    int srclen = 0;
    //crop to y
    for(int y = 0;y < cropheight;y++)
    {
        memcpy(dstpic.data[0] + y*cropwidth,srcpic.data[0] + (y + top)*srcwidth + left,cropwidth);
    }

    croplen = cropwidth*cropheight;
    srclen = srcwidth*srcheight;
    //crop to u
    for(int y = 0;y < cropheight/2;y++)
    {
        memcpy(dstpic.data[0] + croplen + y*cropwidth/2,srcpic.data[0] + srclen + (y + top/2)*srcwidth/2 + left/2,cropwidth/2);
    }

    croplen += cropwidth*cropheight/4;
    srclen += srcwidth*srcheight/4;

    //crop to v
    for(int y = 0;y < cropheight/2;y++)
    {
        memcpy(dstpic.data[0] + croplen + y*cropwidth/2,srcpic.data[0] + srclen + (y + top/2)*srcwidth/2 + left/2,cropwidth/2);
    }

    SwsContext *pSWSCtx = sws_getContext(cropwidth,cropheight,pixmat,srcwidth,srcheight,AV_PIX_FMT_YUV420P,SWS_BICUBIC,NULL,NULL,NULL);

    //scale to srcsize
    if(pSWSCtx != NULL)
    {
        sws_scale(pSWSCtx,dstpic.data,dstpic.linesize,0,cropheight,srcpic.data,srcpic.linesize);
        if(pSWSCtx)
        {

            sws_freeContext(pSWSCtx);
            pSWSCtx = NULL;

        }
    }
    else
    {
        sws_freeContext(pSWSCtx);
        pSWSCtx = NULL;

    }

}

//scale yv12 frame
void scale_frame(uint8_t *dst ,uint8_t *src,int dstwidth,int dstheight,int srcwidth,int srcheight)
{
    assert(src != NULL);
    assert(dst != NULL);

    AVPicture srcpic,dstpic;

    AVPixelFormat pixmat = AV_PIX_FMT_YUV420P;

    avpicture_fill(&srcpic,src,pixmat,srcwidth,srcheight);
    avpicture_fill(&dstpic,dst,pixmat,dstwidth,dstheight);

    SwsContext *pSWSCtx = sws_getContext(srcwidth,srcheight,pixmat,dstwidth,dstheight,AV_PIX_FMT_YUV420P,SWS_BICUBIC,NULL,NULL,NULL);

    //scale to srcsize
    if(pSWSCtx != NULL)
    {
        sws_scale(pSWSCtx,srcpic.data,srcpic.linesize,0,srcwidth,dstpic.data,dstpic.linesize);
        if(pSWSCtx)
        {
            avpicture_free(&srcpic);
            avpicture_free(&dstpic);
            sws_freeContext(pSWSCtx);
            pSWSCtx = NULL;
        }
    }
    else
    {
        avpicture_free(&srcpic);
        avpicture_free(&dstpic);
        sws_freeContext(pSWSCtx);
        pSWSCtx = NULL;

    }
}

void rotate_180_frame(uint8_t *src,int srcwidth,int srcheight)
{
    assert(src != NULL);
    uint8_t *pi = src; //begin of first y line
    uint8_t *pf = pi + srcwidth * srcheight - 1; //begin of last y line

    for(int y = 0;y < srcheight/2; y++)
    {
        for(int x = 0;x < srcwidth;x++)
        {
            uint8_t tmp;
            memcpy(&tmp,pf--,1);
            *(pf+1) = *pi;
            *pi++ = tmp;
        }
    }

    uint8_t *u = (uint8_t *)malloc(srcwidth*srcheight/4);
    pi = src + srcwidth*srcheight;
    memcpy(u,pi,srcwidth*srcheight/4);

    for(int y = srcheight/2 - 1;y >= 0;y--)
    {
        for(int x = srcwidth/2;x > 0;x--)
        {
            *pi++ = u[srcwidth/2*y + x];
        }
    }


    uint8_t *v = (uint8_t *)malloc(srcwidth*srcheight/4);
    pi = src + srcwidth*srcheight + srcwidth*srcheight/4 ;
    memcpy(v,pi,srcwidth*srcheight/4);

    for(int y = srcheight/2 - 1;y >= 0;y--)
    {
        for(int x = srcwidth/2;x > 0;x--)
        {
            *pi++ = v[srcwidth/2*y + x];
        }
    }
    free(u);
    free(v);

}

double min(double a,double b)
{
    return a > b ? b : a;
}


double max(double a,double b)
{
    return a > b ? a : b;
}


void YUV420ConvertImage(UCHAR *pYUV,LONG lYUVWidth,LONG lYUVHeight,LONG lBrightness,LONG lColorfulness,LONG lContrast)
{
    UCHAR *pY = pYUV;
    UCHAR *pU = pYUV + lYUVWidth * lYUVHeight;
    UCHAR *pV = pYUV + lYUVWidth * lYUVHeight * 5 / 4;

    LONG width = ((lYUVWidth * 3 + 3) / 4) * 4;

    LONG uvHeight = lYUVHeight / 2;
    LONG uvWidth = lYUVWidth / 2;
    for (LONG i=0; i<lYUVHeight; i++)
    {
        LONG yi = lYUVHeight - 1 - i;
        LONG uvi = yi / 2;
        for (LONG j=0; j<lYUVWidth; j++)
        {
            LONG uvj = j / 2;

            UCHAR iY = *(pY + yi * lYUVWidth + j);
            UCHAR iU = *(pU + uvi * uvWidth + uvj);
            UCHAR iV = *(pV + uvi * uvWidth + uvj);

            //YUV×ª»»ÎªRGB
            DOUBLE dR = (iY+0) + 1.402*(iV-128);
            DOUBLE dG = (iY+0) - 0.344*(iU-128) - 0.714*(iV-128);
            DOUBLE dB = (iY+0) + 1.772*(iU-128);

            dR = min(max(0, dR), 255);
            dG = min(max(0, dG), 255);
            dB = min(max(0, dB), 255);

/************************************************************************/
/*          ÒÔÏÂŽúÂë¶ÔÁÁ¶ÈœøÐÐÐÞžÄ£¬ÐÞžÄÍê³ÉÖØÐÂŒÆËãYUVÖµ               */
/************************************************************************/
            if(dR + lBrightness > 255)
            {
                dR = 255;
            }
            else if(dR + lBrightness < 0)
            {
                dR = 0;
            }
            else
            {
                dR += lBrightness;
            }

            if(dG + lBrightness > 255)
            {
                dG = 255;
            }
            else if(dG + lBrightness < 0)
            {
                dG = 0;
            }
            else
            {
                dG += lBrightness;
            }

            if(dB + lBrightness > 255)
            {
                dB = 255;
            }
            else if(dB + lBrightness < 0)
            {
                dB = 0;
            }
            else
            {
                dB += lBrightness;
            }

//ÒÔÏÂÈýÐÐÊÇ×ÔŒº»»ËãµÄ¹«Êœ£¬»­ÃæÐ§¹ûÏà²îœÏŽó
//              UCHAR lY = UCHAR(iY - 0.108 * iV + lBrightness + 13.75744) + 1;
//              UCHAR lU = UCHAR(iU + 0.061 * iV -7.683) - 1;
//              UCHAR lV = UCHAR(1.076 * iV -9.761) - 1;

/************************************************************************/
/*                    ÒÔÏÂŽúÂë¶Ô¶Ô±È¶ÈœøÐÐÐÞžÄ                            */
/************************************************************************/
            FLOAT fContrast = (FLOAT)lContrast;
            fContrast = 1.0;

            dR = (dR - 127) * fContrast + 127;
            if(dR > 255)
            {
                dR = 255;
            }

            dG = (dG - 127) * fContrast + 127;
            if(dG > 255)
            {
                dG = 255;
            }

            dB = (dB - 127) * fContrast + 127;
            if(dB > 255)
            {
                dB= 255;
            }

/************************************************************************/
/*    ÒÔÏÂŽúÂë°ÑÍŒÏñ×ª»»Îª»ÒÉ«ÍŒÏñ£¬²»¹ýÊýŸÝÁ¿²»±äµÄÇé¿öÏÂ£¬ËÆºõÃ»Ê²ÃŽÒâÒå*/
/*
            iY = UCHAR(0.299 * dR + 0.587 * dG + 0.114 * dB);
            dR = iY;
            dG = iY;
            dB = iY;
*/
/************************************************************************/

            //RGB×ª»»ÎªYUV
            iY = UCHAR(0.299 * dR + 0.587 * dG + 0.114 * dB);
            iU = UCHAR(0.564 * (dB - iY) + 128);
            iV = UCHAR(0.713 * (dR - iY) + 128);

            *(pY + yi * lYUVWidth + j) = iY;
            *(pU + uvi * uvWidth + uvj) = iU;
            *(pV + uvi * uvWidth + uvj) = iV;
        }
    }
    return ;
}
