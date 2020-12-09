#ifndef _RMW_ALG_BINIMG_FILTER_H
#define _RMW_ALG_BINIMG_FILTER_H

///////////////////////////////////////////////////////////////////////////////////////
//
//二值图像滤波
//
///////////////////////////////////////////////////////////////////////////////////////
void RmwBinImgFilter( BYTE *pBinImg,  //原始二值图像
	                  int width, int height,//图像的宽度和高度
	                  int M, int N, //滤波邻域：M列N行
	                  int threshold, //灰度阈值,大于等于该值时结果赋255
	                  BYTE *pResImg //结果图像
                    );

#endif

