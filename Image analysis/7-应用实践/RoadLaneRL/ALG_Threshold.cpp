///////////////////////////////////////////////////////////////////////////////////////////
//
//������-��ֵ��
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
//ֱ��ͼ
//
///////////////////////////////////////////////////////////////////////////////////////////
void RmwGetHistogram(BYTE *pGryImg, int width, int height, int *histogram)
{
	BYTE *pCur, *pEnd = pGryImg+width*height;

	// step.1-------------��ʼ��----------------------------//
	//for (int g = 0; g<256; g++) histogram[g] = 0;
	memset(histogram, 0, sizeof(int)*256);
	// step.2-------------ֱ��ͼͳ��------------------------//
	for (pCur = pGryImg; pCur<pEnd; ) histogram[*(pCur++)]++;
	// step.3-------------����------------------------------//
	return;
}

void RmwGetHistogram( BYTE *pGryImg, int width, 
	                  int height, int x1, int x2, int y1, int y2, 
	                  int *histogram
                    )
{
	BYTE *pRow;
	int x, y;

	// step.1-------------��ʼ��----------------------------//
	memset(histogram, 0, sizeof(int)*256);
	// step.2-------------ֱ��ͼͳ��------------------------//
	for (y = y1, pRow = pGryImg+y*width; y<=y2; y++, pRow += width)
	{
		for (x = x1; x<=x2; x++)
		{
			histogram[*(pRow+x)]++;
		}
	}
	// step.3-------------����------------------------------//
	return;
}

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
//��ֵ��
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
//�ֿ���ֵ
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
			//�������ֱ��ͼ
			RmwGetHistogram(pGryImg, width, height, x1, x2-1, y1, y2-1, histogram);
			//�����ֵ
			thre = RmwGetOtsuThreshold(histogram, 256);
			//�����ֵ��
			RmwThreshold(pGryImg, width, height, x1, x2-1, y1, y2-1, thre);
		}
	}
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////
//
//�����ֵ
//
///////////////////////////////////////////////////////////////////////////////////////////
void RmwPixelThreshold( BYTE *pGryImg, int width, int height,
	                    int M, int N, //�˲�����M��N��
	                    BYTE *pResImg
                      )
{   //�ó���û�п����Ż�,ֻ������ʵ����
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
		//���㴰��������
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
		//���з��� 
		for (x = 0; x<width; x++, pCur++, pRes++)
		{
			//���㴰�ں�����
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
			//�������ֱ��ͼ
			RmwGetHistogram(pGryImg, width, height, j1, j2, i1, i2, histogram);
			//�����ֵ
			thre = RmwGetOtsuThreshold(histogram, 256);
			//��ֵ��
			*pRes = (*pCur>=thre)*255;
		}
	}
	return;
}
