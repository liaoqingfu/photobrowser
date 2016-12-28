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


//get first frame of video
int get_Video_Frame(char* filename,unsigned char *rgbframe,int dstw,int dsth);

//crop yv12 frame
int crop_frame(unsigned char *dst,unsigned char *src,int top,int left,int srcwidth,int srcheight,int cropwidth,int cropheight);

//scale yv12 frame
void scale_frame(unsigned char *dst ,unsigned char *src,int dstwidth,int dstheight,int srcwidth,int srcheight);

//rotate 180 frame
void rotate_180_frame(unsigned char *src,int srcwidth,int srcheight);


void YUV420ConvertImage(UCHAR *pYUV,LONG lYUVWidth,LONG lYUVHeight,LONG lBrightness,LONG lColorfulness,LONG lContrast);


void yv12_change_color_rgb24 (UCHAR *out, UCHAR *in, int width, int height,LONG lBrightness,LONG lColorfulness,LONG lContrast);
#endif // VIDEOUTIL_H
