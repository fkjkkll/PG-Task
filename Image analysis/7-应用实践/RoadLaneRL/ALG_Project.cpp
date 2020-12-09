///////////////////////////////////////////////////////////////////////////////////////////
//
//   第七章-投影
//
///////////////////////////////////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <nmmintrin.h>

///////////////////////////////////////////////////////////////////////////////////////////
//
//计算水平投影
//
///////////////////////////////////////////////////////////////////////////////////////////
void RmwGetProjectHor(BYTE *pGryImg, int width, int height, int *prjHor)
{
	BYTE *pCur;
	int x, y;

	for (y = 0, pCur = pGryImg; y<height; y++)
	{
		prjHor[y] = 0;
		for (x = 0; x<width; x++) prjHor[y] += *(pCur++);
	}
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////
//
//计算垂直投影
//
///////////////////////////////////////////////////////////////////////////////////////////
void RmwGetProjectVer(BYTE *pGryImg, int width, int height, int *prjVer)
{
	BYTE *pCur;
	int x, y;

	memset(prjVer, 0, sizeof(int)*width);
	for (y = 0, pCur = pGryImg; y<height; y++)
	{
		for (x = 0; x<width; x++) prjVer[x] += *(pCur++);
	}
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////
//
//画水平投影
//
///////////////////////////////////////////////////////////////////////////////////////////
void RmwDrawPrjHor2Img(  int *prjHor,
	                     int nScale,
	                     BYTE *pGryImg, int width, int height,
	                     bool doReset
                      )
{   //画垂直投影
	int x,y;

	// step.1-------------初始化----------------------------//
	if (doReset)
	{
		memset(pGryImg, 25, width*height);
	}
	// step.2-------------像素填充--------------------------//
	for (y = 0; y<height; y++)
	{
		if (!prjHor[y]) continue;
		x = prjHor[y]/nScale;
		x = min(width, x);
		memset(pGryImg+y*width, 225, x);
	}
	// step.3-------------结束------------------------------//
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////
//
//画垂直投影
//
///////////////////////////////////////////////////////////////////////////////////////////
void RmwDrawPrjVer2Img(  int *prjVer,
	                     int nScale,
	                     BYTE *pGryImg, int width, int height,
	                     bool doReset
                      )
{   //画垂直投影
	int v, x;
	int y1, y;

	// step.1-------------初始化----------------------------//
	if (doReset)
	{
		memset(pGryImg, 225, width*height);
	}
	// step.2-------------像素填充--------------------------//
	for (x = 0; x<width; x++)
	{
		if (!prjVer[x]) continue;
		v = prjVer[x]/nScale;
		y1 = (height-1)-v;
		y1 = max(0, y1);
		for (y = height-1; y>y1; y--) *(pGryImg+y*width+x) = 25;
	}
	// step.3-------------结束------------------------------//
	return;
}

void RmwDrawPrjVer2Img(  int *prjVer,
	                     BYTE *pGryImg, int width, int height,
	                     int color
                      )
{   //画垂直投影
	int minV, maxV;
	int v, x;
	int y1, y;
	double ratio;

	// step.1-------------初始化----------------------------//
	memset(pGryImg, 25, width*height);
	// step.2-------------求比例因子------------------------//
	maxV = minV = prjVer[0];
	for (x = 0; x<width; x++)
	{
		minV = min(minV, prjVer[x]);
		maxV = max(maxV, prjVer[x]);
	}
	ratio = (maxV-minV+2.0)/height;
	// step.2-------------像素填充--------------------------//
	for (x = 0; x<width; x++)
	{
		if (!prjVer[x]) continue;
		v = (int)((prjVer[x]-minV)/ratio+0.5)+2;
		y1 = (height-1)-v;
		y1 = max(0, y1);
		for (y = height-1; y>y1; y--) *(pGryImg+y*width+x) = color;
	}
	// step.3-------------结束------------------------------//
	return;
}
 
void RmwDrawPrjVer2Img( BYTE *pGry2Img, int width, int height,
	                    int *pPrj1, int *pPrj2, int nPrj,
	                    int color1, int color2
                      )
{
	//同时画2个垂直投影
	int x, y, minV, maxV;
	int i, k;
	double ratio;

	//k
	maxV = minV=pPrj1[0];
	for (x = 0; x<nPrj; x++)
	{
		minV = min(minV, pPrj1[x]);
		maxV = max(maxV, pPrj1[x]);

		minV = min(minV, pPrj2[x]);
		maxV = max(maxV, pPrj2[x]);
	}
	ratio = (maxV-minV+2.0)/height;
	//draw
	for (x = 0; x<min(nPrj,width); x++)
	{
		//1
		i = (int)((pPrj1[x]-minV)/ratio+0.5);
		for (y = height-1, k = 0; y>0; y--)
		{
			k++;
			if (k>i) break;
			*(pGry2Img+y*width+x) = color1;
		}
		//2
		i = (int)((pPrj2[x]-minV)/ratio+0.5);
		for (y = height-1, k = 0; y>0; y--)
		{
			k++;
			if (k>i) break;
			*(pGry2Img+width*height+y*width+x) = color2;
		}
	}
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////
//
//1D滤波
//
///////////////////////////////////////////////////////////////////////////////////////////
void Rmw1DAverageFilter(int *pData, int width, int halfw, int *pResData)
{
	int x, sum, wSize;
	int j, j1, j2;

	//[0,halfX): 确保对称即可: 0: 0(j=1)->sum=0,1,2; 1:(j=3)->sum:0,1,2,3,4
	wSize = 1;
	for (sum = pData[0], x = 0, j = 1; x<halfw; x++, j += 2)
	{
		pResData[x] = sum/wSize;
		sum += pData[j]+pData[j+1];
		wSize += 2;
	}
	//[halfX,width-halfx): width=10 ==> 9(x),8(x),7(x),6,5.
	wSize = (2*halfw+1);
	for (x = halfw, j1 = 0, j2 = x+halfw+1; x<width-halfw-1; x++, j1++, j2++)  //3,4,5,6,7(x),8(x),9(x)
	{
		pResData[x] = sum/wSize;
		sum = sum-pData[j1]+pData[j2]; //sum=0,1,2(c),3,4:->0(del,j1=0),1,2,3(nxt),4,5(add,j2=2+2+1)
	}
	//[width-halfX-1,width)
	for (x = width-halfw-1, j = x-halfw; x<width; x++, j += 2)
	{
		pResData[x] = sum/wSize;
		sum -= (pData[j]+pData[j+1]);// 9,8,7(c),6,5=>nxt: 8, -5(j=7-2),-6.
		wSize -= 2;
	}
	return;
}
