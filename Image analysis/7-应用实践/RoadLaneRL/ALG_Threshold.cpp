///////////////////////////////////////////////////////////////////////////////////////////
//
//第五章-阈值化
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
//直方图
//
///////////////////////////////////////////////////////////////////////////////////////////
void RmwGetHistogram(BYTE *pGryImg, int width, int height, int *histogram)
{
	BYTE *pCur, *pEnd = pGryImg+width*height;

	// step.1-------------初始化----------------------------//
	//for (int g = 0; g<256; g++) histogram[g] = 0;
	memset(histogram, 0, sizeof(int)*256);
	// step.2-------------直方图统计------------------------//
	for (pCur = pGryImg; pCur<pEnd; ) histogram[*(pCur++)]++;
	// step.3-------------结束------------------------------//
	return;
}

void RmwGetHistogram( BYTE *pGryImg, int width, 
	                  int height, int x1, int x2, int y1, int y2, 
	                  int *histogram
                    )
{
	BYTE *pRow;
	int x, y;

	// step.1-------------初始化----------------------------//
	memset(histogram, 0, sizeof(int)*256);
	// step.2-------------直方图统计------------------------//
	for (y = y1, pRow = pGryImg+y*width; y<=y2; y++, pRow += width)
	{
		for (x = x1; x<=x2; x++)
		{
			histogram[*(pRow+x)]++;
		}
	}
	// step.3-------------结束------------------------------//
	return;
}

void RmwDrawHistogram2Img( int *histogram, int histSize, 
	                       double maxScale,
	                       BYTE *pGryImg, int width, int height
                         )
{   //直方图数据画为黑色
	int x1, x2, y1;
	int maxV, i;
	int x, y;

	// step.1-------------初始化----------------------------//
	memset(pGryImg, 225, width*height);
	// step.2-------------寻找最大值------------------------//
	maxV = 0;
	for (i = 0; i<histSize; i++)
	{
		maxV = max(maxV, histogram[i]);
	}
	maxV = (int)(maxV*maxScale);
	// step.3-------------像素填充--------------------------//
	for (i = 0; i<histSize; i++)
	{
		y1 = (height-1)-histogram[i]*(height-1)/(maxV+1);
		y1 = max(0, y1);
		x1 = i*width/histSize;
		x2 = (i+1)*width/histSize;
		for (x = x1; x<x2; x++)
		{
			for (y = height-1; y>y1; y--) *(pGryImg+y*width+x) = 0;
		}
	}
	// step.4-------------结束------------------------------//
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////
//
//二值化
//
///////////////////////////////////////////////////////////////////////////////////////////
void RmwThreshold(BYTE *pGryImg, int width, int height, int thre)
{
	BYTE *pCur, *pEnd = pGryImg+width*height;

	for (pCur=pGryImg; pCur<pEnd; pCur++) *pCur = (*pCur>=thre)*255;
	return;
}

void RmwThreshold(BYTE *pGryImg, int width, int height, int thre,BYTE *pBinImg)
{
	BYTE *pCur, *pEnd = pGryImg+width*height;
	BYTE *pBin = pBinImg;

	for (pCur = pGryImg; pCur<pEnd; pCur++) *(pBin++) = (*pCur>=thre)*255;
	return;
}

void RmwSemiThreshold(BYTE *pGryImg, int width, int height, int thre)
{
	BYTE *pCur, *pEnd = pGryImg+width*height;

	for (pCur = pGryImg; pCur<pEnd; pCur++)
	{
		if (*pCur>=thre) *pCur = 255;
	}
	return;
}

void RmwThreshold(BYTE *pGryImg, int width, int height, int x1, int x2, int y1, int y2, int thre)
{
	BYTE *pRow;
	int x, y;

	for (y = y1, pRow = pGryImg+y*width; y<=y2; y++, pRow += width)
	{
		for (x = x1; x<=x2; x++)
		{
			*(pRow+x) = (*(pRow+x)>=thre)*255;
		}
	}
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////
//
//Otsu阈值
//
///////////////////////////////////////////////////////////////////////////////////////////
int RmwGetOtsuThreshold(int *histogram, int nSize)
{
	int thre;
	int i, gmin, gmax;
	double dist, f, max;
	int s1, s2, n1, n2, n;

	// step.1-----确定搜索范围:最小值----------------------------//
	gmin = 0;
	while (histogram[gmin]==0) ++gmin;
	// step.2-----确定搜索范围:最大值----------------------------//
	gmax = nSize-1;
	while (histogram[gmax]==0) --gmax;
	// step.3-----搜索最佳阈值-----------------------------------//
	if (gmin==gmax) return gmin; //不满足二类分布
	max = 0;
	thre = 0;
	//初始化u1
	s1 = n1 = 0;
	//初始化u2
	for (s2 = n2 = 0, i = gmin; i<=gmax; i++)
	{
		s2 += histogram[i]*i;
		n2 += histogram[i];
	}
	//搜索
	for (i = gmin, n = n2; i<gmax; i++)
	{   //加速
		if (!histogram[i]) continue;
		//更新s1s2
		s1 += histogram[i]*i;
		s2 -= histogram[i]*i;
		//更新n1n2
		n1 += histogram[i];
		n2 -= histogram[i];
		//评价函数
		dist = (s1*1.0/n1-s2*1.0/n2);
		f = dist*dist*(n1*1.0/n)*(n2*1.0/n);
		if (f>max)
		{
			max = f;
			thre = i;
		}
	}
	// step.4-----返回-------------------------------------------//
	return thre+1; //二值化时是用>=thre,所以要+1
}
///////////////////////////////////////////////////////////////////////////////////////////
//
//分块阈值
//
///////////////////////////////////////////////////////////////////////////////////////////
void RmwBlockThreshold(BYTE *pGryImg, int width, int height, int nBlockX, int nBlockY)
{
	int histogram[256], thre;
	int stepx, stepy;
	int x, y, x1, x2, y1, y2;

	stepx = width/nBlockX;
	stepy = height/nBlockY;
	for (y = 0; y<height; y+=stepy)
	{
		y1 = y;
		y2 = y+stepy;
		if (height-y2<stepy)
		{
			y2 = height;
		}
		for (x = 0; x<width; x+=stepx)
		{
			x1 = x;
			x2 = x+stepx;
			if (width-x2<stepx)
			{
				x2 = width;
			}
			//计算块内直方图
			RmwGetHistogram(pGryImg, width, height, x1, x2-1, y1, y2-1, histogram);
			//求块阈值
			thre = RmwGetOtsuThreshold(histogram, 256);
			//按块二值化
			RmwThreshold(pGryImg, width, height, x1, x2-1, y1, y2-1, thre);
		}
	}
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////
//
//逐点阈值
//
///////////////////////////////////////////////////////////////////////////////////////////
void RmwPixelThreshold( BYTE *pGryImg, int width, int height,
	                    int M, int N, //滤波邻域：M列N行
	                    BYTE *pResImg
                      )
{   //该程序没有考虑优化,只是做个实验结果
	BYTE *pCur, *pEnd = pGryImg+width*height;
	BYTE *pRes;
	int histogram[256], thre;
	int x, y;
	int halfN, halfM;
	int i1, i2, j1, j2;

	N = N/2*2+1;  halfN = N/2;
	M = M/2*2+1;  halfM = M/2;
	for (y = 0, pCur = pGryImg, pRes = pResImg; y<height; y++)
	{
		printf("%d\n", y);
		//计算窗口纵坐标
		i1 = y-halfN;
		if (i1<0)
		{
			i1 = 0;
			i2 = i1+N-1;
		}
		else
		{
			i2 = y+halfN;
			if (i2>height-1)
			{
				i2 = height-1;
				i1 = i2-N+1;
			}
		}
		//按行访问 
		for (x = 0; x<width; x++, pCur++, pRes++)
		{
			//计算窗口横坐标
			j1 = x-halfM;
			if (j1<0)
			{
				j1 = 0;
				j2 = j1+M-1;
			}
			else
			{
				j2 = x+halfM;
				if (j2>width-1)
				{
					j2 = width-1;
					j1 = j2-M+1;
				}
			}
			//计算块内直方图
			RmwGetHistogram(pGryImg, width, height, j1, j2, i1, i2, histogram);
			//求块阈值
			thre = RmwGetOtsuThreshold(histogram, 256);
			//二值化
			*pRes = (*pCur>=thre)*255;
		}
	}
	return;
}
