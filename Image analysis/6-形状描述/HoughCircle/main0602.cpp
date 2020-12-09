#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "bmpFile.h"
#include "RmwALG_HoughCircle.h"

///////////////////////////////////////////////////////////////////////////////////////
//
//  ȫ�ֱ���
//
///////////////////////////////////////////////////////////////////////////////////////
#define  MAX_POINT_NUM   (512*512)
static int gN;
static int gX[MAX_POINT_NUM];
static int gY[MAX_POINT_NUM];
///////////////////////////////////////////////////////////////////////////////////////
//
//ԲHT�任
//
///////////////////////////////////////////////////////////////////////////////////////
void testCircleHT()
{
	unsigned char *pGryImg;
	int width, height;
	int R=14; //�뾶

	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0602\\Fig0616b.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
 	// step.2-------------�ռ���Ե��------------------------//
 	BYTE *pCur;
	int x, y;
	gN = 0;
	for (y = 0, pCur = pGryImg; y<height; y++)
	{
		for (x = 0; x<width; x++, pCur++)
		{
			if (*pCur)
			{
				*pCur = 0;
				if (gN<MAX_POINT_NUM-1)
				{
					gX[gN] = x;
					gY[gN] = y;
					gN++;
				}
			}
			else *pCur = 200;
		}
	}
 	// step.3-------------����任--------------------------//
	int resX0, resY0; //Բ��λ��
	int *pCount = new int[width*height];
	RmwHoughCircle(pCount, width, height, R, gX, gY, gN, &resX0, &resY0);
	//���Բ
	RmwDrawCircle(pGryImg, width, height, resX0, resY0, R, 255);
	Rmw_Write8BitImg2BmpFileMark(pGryImg, width, height, "..\\RmwCPP0602\\Fig0616d.bmp");
	//Բ�ļ���ֵ
	for (int i = 0; i<width*height; i++) pGryImg[i] = min(255, pCount[i]*2);
	Rmw_Write8BitImg2BmpFile(pGryImg, width, height, "..\\RmwCPP0602\\Fig0616c.bmp");
	// step.4-------------����-----------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	delete pCount; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}
///////////////////////////////////////////////////////////////////////////////////////
//
//Բ���η�
//
///////////////////////////////////////////////////////////////////////////////////////
void testCircleX0Y0()
{
	unsigned char *pGryImg;
	int width, height;

	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0602\\Fig0617a.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	// step.2-------------ͼ����--------------------------//
	int x0, y0, count;
	count = RmwCircleX0(pGryImg, width, height, &x0);
	count = RmwCircleY0(pGryImg, width, height, &y0);
	// step.3-------------����------------------------------//
	for (int i = 0; i<width*height; i++) pGryImg[i] = min(250, pGryImg[i]);
	*(pGryImg+y0*width+x0-1) = 255;
	*(pGryImg+y0*width+x0) = 255;
	*(pGryImg+y0*width+x0+1) = 255;

	*(pGryImg+(y0-1)*width+x0) = 255;
	*(pGryImg+y0*width+x0) = 255;
	*(pGryImg+(y0+1)*width+x0) = 255;

	Rmw_Write8BitImg2BmpFileMark(pGryImg, width, height, "..\\RmwCPP0602\\Fig0617a_x0y0.bmp");
	// step.4-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}

void main()
{
	testCircleHT();
	testCircleX0Y0();
}