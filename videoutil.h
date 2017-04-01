#ifndef VIDEOUTIL_H
#define VIDEOUTIL_H

/******************************************
 *
 *email: sunew130@gmail.com
 *
 * github:sunew130
 *
 *developer:sunew
 *
 *
 * *****************************************/

typedef unsigned char UCHAR;
typedef long LONG;
typedef double DOUBLE;
typedef float FLOAT;

typedef struct _VIdeoInfo{
    int width;
    int height;
}VIdeoInfo;


VIdeoInfo * init_Video_Frame(char *filename);
//get first frame of video
int  get_Video_Frame(char *filename,unsigned char *rgbframe,int dstw,int dsth);

void  get_Video_One_Frame(unsigned char *rgbframe,int dstw,int dsth);

void closeVideo();

//crop yv12 frame
int crop_frame(unsigned char *dst,unsigned char *src,int top,int left,int srcwidth,int srcheight,int cropwidth,int cropheight);

//scale yv12 frame
void scale_frame(unsigned char *dst ,unsigned char *src,int dstwidth,int dstheight,int srcwidth,int srcheight);

//rotate 180 frame
void rotate_180_frame(unsigned char *src,int srcwidth,int srcheight);


void rgb24_to_yu12(unsigned char  *out, unsigned char *in, int width, int height);

void YUV420ConvertImage(UCHAR *pYUV,LONG lYUVWidth,LONG lYUVHeight,LONG lBrightness,LONG lColorfulness,LONG lContrast);

//添加水印图片
int frameAddWaterMarks(unsigned char  *dstframe,unsigned char *srcframe,int width,int height);


#endif // VIDEOUTIL_H
