#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "bmpFile.h"
#include "RmwALG_2DHistogram.h"
 
void test2Dhistogram()
{
	unsigned char *pGryImg;
	unsigned char *pAvrImg;
	int width, height;
	int histogram[256];
	int thre, num;

	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0502\\Fig0506Gry.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	pAvrImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0502\\Fig0506Gry_avr.bmp", &width, &height);;
	if (!pAvrImg)
	{
		delete pGryImg;
		printf("*file open err!\n");
		getchar();
		return;
	}
	BYTE *pResImg = new BYTE[width*height];
	// step.2-------------�Խ���ֱ��ͼ-----------------------//
	num = RmwHistogramBy2D(pGryImg, pAvrImg, width, height, 4*2, histogram);
	RmwDrawHistogram2Img(histogram, 256, 1.0, pResImg, width, height);
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0502\\Fig0506Gry_1_2DHistogram.bmp");
	// step.3-------------��ֵ��----------------------------//
	thre = RmwGetOtsuThreshold(histogram, 256);
	printf("\nnum=%d(%.3lf),thre=%d\n", num, num*1.0/(width*height), thre);
	//��ֵͼ��
	RmwThreshold(pGryImg, width, height, thre);
	Rmw_Write8BitImg2BmpFile(pGryImg, width, height, "..\\RmwCPP0502\\Fig0506Gry_2_Bin.bmp");
	// step.4-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	delete pAvrImg; //�ͷ��Լ�������ڴ�
	delete pResImg; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}

void main()
{
	test2Dhistogram();
}

 