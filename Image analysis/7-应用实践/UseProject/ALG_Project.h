///////////////////////////////////////////////////////////////////////////////////////////
//
//   第七章-投影
//
///////////////////////////////////////////////////////////////////////////////////////////
#ifndef _RMW_ALG_PROJECT_H
#define _RMW_ALG_PROJECT_H

///////////////////////////////////////////////////////////////////////////////////////////
//
//计算水平投影
//
///////////////////////////////////////////////////////////////////////////////////////////
void RmwGetProjectHor(BYTE *pGryImg, int width, int height, int *prjHor);
///////////////////////////////////////////////////////////////////////////////////////////
//
//计算垂直投影
//
///////////////////////////////////////////////////////////////////////////////////////////
void RmwGetProjectVer(BYTE *pGryImg, int width, int height, int *prjVer);
///////////////////////////////////////////////////////////////////////////////////////////
//
//画水平投影
//
///////////////////////////////////////////////////////////////////////////////////////////
void RmwDrawPrjHor2Img(  int *prjHor,
	                     int nScale,
	                     BYTE *pGryImg, int width, int height,
	                     bool doReset
                      );
///////////////////////////////////////////////////////////////////////////////////////////
//
//画垂直投影
//
///////////////////////////////////////////////////////////////////////////////////////////
void RmwDrawPrjVer2Img(  int *prjVer,
	                     int nScale,
	                     BYTE *pGryImg, int width, int height,
	                     bool doReset
                      );
 
void RmwDrawPrjVer2Img(  int *prjVer,
	                     BYTE *pGryImg, int width, int height,
	                     int color
                      );
//同时画2个垂直投影  
void RmwDrawPrjVer2Img( BYTE *pGry2Img, int width, int height,
	                    int *pPrj1, int *pPrj2, int nPrj,
	                    int color1, int color2
                      );
///////////////////////////////////////////////////////////////////////////////////////////
//
//1D滤波
//
///////////////////////////////////////////////////////////////////////////////////////////
void Rmw1DAverageFilter(int *pData, int width, int halfw, int *pResData);

#endif

 