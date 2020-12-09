///////////////////////////////////////////////////////////////////////////////////////////
//
//������
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
//��ֵ��
//
///////////////////////////////////////////////////////////////////////////////////////////
void RmwThreshold(BYTE *pGryImg, int width, int height, int thre)
{
	BYTE *pCur, *pEnd = pGryImg+width*height;

	for (pCur = pGryImg; pCur<pEnd; pCur++) *pCur = (*pCur>=thre)*255;
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////
//
//Otsu��ֵ
//
///////////////////////////////////////////////////////////////////////////////////////////
int RmwGetOtsuThreshold(int *histogram, int nSize)
{
	int thre;
	int i, gmin, gmax;
	double dist, f, max;
	int s1, s2, n1, n2, n;

	// step.1-----ȷ��������Χ:��Сֵ----------------------------//
	gmin = 0;
	while (histogram[gmin]==0) ++gmin;
	// step.2-----ȷ��������Χ:���ֵ----------------------------//
	gmax = nSize-1;
	while (histogram[gmax]==0) --gmax;
	// step.3-----���������ֵ-----------------------------------//
	if (gmin==gmax) return gmin; //���������ֲ�
	max = 0;
	thre = 0;
	//��ʼ��u1
	s1 = n1 = 0;
	//��ʼ��u2
	for (s2 = n2 = 0, i = gmin; i<=gmax; i++)
	{
		s2 += histogram[i]*i;
		n2 += histogram[i];
	}
	//����
	for (i = gmin, n = n2; i<gmax; i++)
	{   //����
		if (!histogram[i]) continue;
		//����s1s2
		s1 += histogram[i]*i;
		s2 -= histogram[i]*i;
		//����n1n2
		n1 += histogram[i];
		n2 -= histogram[i];
		//���ۺ���
		dist = (s1*1.0/n1-s2*1.0/n2);
		f = dist*dist*(n1*1.0/n)*(n2*1.0/n);
		if (f>max)
		{
			max = f;
			thre = i;
		}
	}
	// step.4-----����-------------------------------------------//
	return thre+1; //��ֵ��ʱ����>=thre,����Ҫ+1
}
///////////////////////////////////////////////////////////////////////////////////////////
//
//��ֱ��ͼ
//
///////////////////////////////////////////////////////////////////////////////////////////
void RmwDrawHistogram2Img( int *histogram, int histSize, 
	                       double maxScale,
	                       BYTE *pGryImg, int width, int height
                         )
{   //ֱ��ͼ���ݻ�Ϊ��ɫ
	int x1, x2, y1;
	int maxV, i;
	int x, y;

	// step.1-------------��ʼ��----------------------------//
	memset(pGryImg, 225, width*height);
	// step.2-------------Ѱ�����ֵ------------------------//
	maxV = 0;
	for (i = 0; i<histSize; i++)
	{
		maxV = max(maxV, histogram[i]);
	}
	maxV = (int)(maxV*maxScale);
	// step.3-------------�������--------------------------//
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
	// step.4-------------����------------------------------//
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////
//
//��άֱ��ͼ
//
///////////////////////////////////////////////////////////////////////////////////////////
void Rmw2DHistogram(BYTE *pGryImg, BYTE *pAvrImg, int width, int height, int *histogram2D)
{   
	int *pos;

	memset(histogram2D, 0, sizeof(int)*256*256);
	for (int i = 0; i<width*height; i++)
	{
		pos= histogram2D+pGryImg[i]*256+pAvrImg[i];
		*pos = *pos + 1;
	}
	return;
}

int RmwHistogramBy2D( BYTE *pGryImg, BYTE *pAvrImg, 
	                  int width, int height, 
	                  int dist, //���ֵ��ƫ��
	                  int *histogram
                    )
{
	int g, u;
	int num = 0;

	memset(histogram, 0, sizeof(int)*256);
	for (int i = 0; i<width*height; i++)
	{
		u = pAvrImg[i];
		g = pGryImg[i];
		if (abs(g-u)<=dist) //��ͳ���ھ�ֵ������
		{
			histogram[g]++;
			num++;
			//pGryImg[i] = 255; //����
		}
	}
	return num;
}
