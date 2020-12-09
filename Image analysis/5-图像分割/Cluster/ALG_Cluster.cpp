///////////////////////////////////////////////////////////////////////////////////////////
//
//第五章-聚类分割
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
//聚类分割
//
///////////////////////////////////////////////////////////////////////////////////////////
bool RmwClusterHistogram(int *histogram, int nClass, double decision, double *center)
{
	double center_pre[256];
	double sum[256];
	int num[256];
	double step, dist, minDist;
	int i, min, max;
	int j, type;

	// step.1-------------初始化----------------------------//
	if (nClass<2) return false;
	// step.2-------------有效灰度的最大和最小值-------------//
	for (min = 0; min<256; min++) 
	{
		if (histogram[min]) break;
	}
	for (max = 255; max>=0; max--)
	{
		if (histogram[max]) break;
	}
	if (max<=min) return false;
	// step.3-------------初始化聚类中心:等间隔--------------//
	center[0] = min;
	step= (max-min)/(nClass-1);
	for (i = 1; i<nClass; i++) center[i] = center[i-1]+step;
	// step.4-------------迭代聚类--------------------------//
	while (1)
	{
		for (i = 0; i<nClass; i++)
		{
			printf("%.3lf    ", center[i]);
		}
		printf("\n");
		//初始化
		for (i = 0; i<nClass; i++)
		{
			sum[i] = num[i] = 0;
			center_pre[i] = center[i];
		}
		//聚类
		for (i = 0; i<256; i++)
		{
			if (histogram[i]!=0L)
			{
				//最佳类别
				minDist = 256;
				type = nClass-1;
				for (j = 0; j<nClass; j++)
				{
					dist = fabs(center[j]-i);
					if (dist<minDist)
					{
						minDist = dist;
						type = j;
					}
				}
				//更新中心
				num[type] += histogram[i];
				sum[type] += histogram[i]*i;
				center[type] = sum[type]/num[type];
			}
		}
		//判定
		for (i = 0; i<nClass; i++)
		{
			if (num[i]==0) continue;
			if (fabs(center[i]-center_pre[i])>decision) break;
		}
		if (i>=nClass) break;
	}
	// step.5-------------返回-----------------------------//
	return true;
}
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