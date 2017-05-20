#include "videoutil.h"
#include <stdio.h>

/**
 * 最简单的基于FFmpeg的AVFilter例子（叠加水印）
 * 本程序使用FFmpeg的AVfilter实现了视频的水印叠加功能。
 * 可以将一张PNG图片作为水印叠加到视频上。
 * 是最简单的FFmpeg的AVFilter方面的教程。
 * 适合FFmpeg的初学者。
 *
 * This software uses FFmpeg's AVFilter to add watermark in a video file.
 * It can add a PNG format picture as watermark to a video file.
 * It's the simplest example based on FFmpeg's AVFilter.
 * Suitable for beginner of FFmpeg
 *
 */

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfiltergraph.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
#include <SDL/SDL.h>
};

const char *filter_descr = "movie=logo.png[wm];[in][wm]overlay=5:5[out]";
static AVCodecContext *pCodecCtx;
AVFilterContext *buffersink_ctx;
AVFilterContext *buffersrc_ctx;
AVFilterGraph *filter_graph;

static int init_filters(const char *filters_descr)
{
    char args[512];
    int ret;
    AVFilter *buffersrc  = avfilter_get_by_name("buffer");
    AVFilter *buffersink = avfilter_get_by_name("buffersink");
    AVFilterInOut *outputs = avfilter_inout_alloc();
    AVFilterInOut *inputs  = avfilter_inout_alloc();
    enum AVPixelFormat pix_fmts[] = { AV_PIX_FMT_YUV420P,AV_PIX_FMT_NONE };
    AVBufferSinkParams *buffersink_params;

    filter_graph = avfilter_graph_alloc();

    /* buffer video source: the decoded frames from the decoder will be inserted here. */
    snprintf(args, sizeof(args),
             "video_size=1280x720:pix_fmt=0:time_base=1/50:pixel_aspect=0/1",
             AV_PIX_FMT_YUV420P);

    ret = avfilter_graph_create_filter(&buffersrc_ctx, buffersrc, "in",
                                       args, NULL, filter_graph);
    if (ret < 0) {
        printf("Cannot create buffer source\n");
        return ret;
    }

    /* buffer video sink: to terminate the filter chain. */
    buffersink_params = av_buffersink_params_alloc();
    buffersink_params->pixel_fmts = pix_fmts;
    ret = avfilter_graph_create_filter(&buffersink_ctx, buffersink, "out",
                                       NULL, buffersink_params, filter_graph);
    av_free(buffersink_params);
    if (ret < 0) {
        printf("Cannot create buffer sink\n");
        return ret;
    }

    /* Endpoints for the filter graph. */
    outputs->name       = av_strdup("in");
    outputs->filter_ctx = buffersrc_ctx;
    outputs->pad_idx    = 0;
    outputs->next       = NULL;

    inputs->name       = av_strdup("out");
    inputs->filter_ctx = buffersink_ctx;
    inputs->pad_idx    = 0;
    inputs->next       = NULL;

    if ((ret = avfilter_graph_parse_ptr(filter_graph, filters_descr,
                                        &inputs, &outputs, NULL)) < 0)
        return ret;

    if ((ret = avfilter_graph_config(filter_graph, NULL)) < 0)
        return ret;
    return 0;
}


//添加水印图片
int frameAddWaterMarks(uint8_t *dstframe,uint8_t *srcframe,int width,int height)
{
    int ret;
    AVFrame *pFrame;
    AVFrame *pFrame_out;

    av_register_all();
    avfilter_register_all();

    if ((ret = init_filters(filter_descr)) < 0)
    {
        printf( "Error while init_filters\n");
        goto end;
    }

    pFrame=av_frame_alloc();
    pFrame_out=av_frame_alloc();

   avpicture_fill((AVPicture *)pFrame, srcframe, AV_PIX_FMT_YUV420P,width, height);
   avpicture_fill((AVPicture *)pFrame_out, dstframe, AV_PIX_FMT_YUV420P,width, height);

    pFrame->width = width;
    pFrame->height = height;
    pFrame->format = AV_PIX_FMT_YUV420P;
//    pFrame->sample_aspect_ratio.den = 1;
//    pFrame->sample_aspect_ratio.num = 0;
//    pFrame->pkt_pts = 0;
//     pFrame->pkt_dts = 0;
//    pFrame->pts = 77;
//    pFrame->sample_rate = 77;
//    pFrame->key_frame = 1;
//    pFrame->pts = av_frame_get_best_effort_timestamp(pFrame);


//    pFrame_out->width = width;
//    pFrame_out->height = height;
//    pFrame_out->format = AV_PIX_FMT_YUV420P;
//    pFrame_out->sample_aspect_ratio.den = 1;
//    pFrame_out->sample_aspect_ratio.num = 0;
//    pFrame_out->pkt_pts = 0;
//     pFrame_out->pkt_dts = 0;
//    pFrame_out->pts = 77;
//    pFrame_out->sample_rate = 77;
//    pFrame_out->key_frame = 1;
//    pFrame_out->pts = av_frame_get_best_effort_timestamp(pFrame);

    /* push the decoded frame into the filtergraph */
    if (av_buffersrc_add_frame(buffersrc_ctx, pFrame) < 0)
    {
        printf( "Error while feeding the filtergraph\n");
    }

    /* pull filtered pictures from the filtergraph */

    if (av_buffersink_get_frame(buffersink_ctx, pFrame_out) < 0 )
    {
        printf( "Error while feeding the filtergraph\n");
    }


    if(pFrame_out->format == AV_PIX_FMT_YUV420P)
        printf( "memcpy done\n");

    av_frame_unref(pFrame_out);

    av_frame_unref(pFrame);

    //free RGB
    //av_free(pFrame_out);

    //av_free(pFrame);

end:
    avfilter_graph_free(&filter_graph);
    if (pCodecCtx)
        avcodec_close(pCodecCtx);

    if (ret < 0 && ret != AVERROR_EOF) {
        return -1;
    }

    return 0;
}
