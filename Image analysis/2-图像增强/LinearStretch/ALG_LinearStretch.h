#ifndef _RMW_ALG_LINEAR_STRETCH_H
#define _RMW_ALG_LINEAR_STRETCH_H

///////////////////////////////////////////////////////////////////////////////////////////
//
//   第二章-线性拉伸
//
///////////////////////////////////////////////////////////////////////////////////////////
void LinearStretchDemo1(BYTE *pGryImg, int width, int height, double k, double b, BYTE *pResImg);
void LinearStretchDemo2(BYTE *pGryImg, int width, int height, double k, double b, BYTE *pResImg);
void Color2GrayDemo1(BYTE *pColImg, int width, int height, BYTE *pResImg);
void Color2GrayDemo2(BYTE *pColImg, int width, int height, BYTE *pResImg);
 
#endif

