#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "bmpFile.h"
#include "RmwALG_Threshold.h"
 
void testGlobalThre()
{
	unsigned char *pGryImg;
	int width, height;
	int histogram[256], thre;

	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0501\\Fig0504Gry.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	// step.2-------------����ͼ��--------------------------//
	RmwGetHistogram(pGryImg, width, height, histogram);
	thre = RmwGetOtsuThreshold(histogram, 256);
	RmwThreshold(pGryImg, width, height, thre);
	// step.3-------------����ͼ��--------------------------//
	Rmw_Write8BitImg2BmpFile(pGryImg, width, height, "..\\RmwCPP0501\\Fig0504Gry_1_ByGlobal.bmp");
	// step.4-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}

void testBlockThre()
{
	unsigned char *pGryImg;
	int width, height;

	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0501\\Fig0504Gry.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	// step.2-------------����ͼ��--------------------------//
	//�ֿ���ֵ
	RmwBlockThreshold(pGryImg, width, height, 4, 4);
	// step.3-------------����ͼ��--------------------------//
	Rmw_Write8BitImg2BmpFile(pGryImg, width, height, "..\\RmwCPP0501\\Fig0504Gry_2_ByBlock.bmp");
	// step.4-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}

void testPixThre()
{
	unsigned char *pGryImg;
	int width, height;

	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0501\\Fig0504Gry.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	// step.2-------------����ͼ��--------------------------//
	BYTE *pResImg = new BYTE[width*height];
	//82
	RmwPixelThreshold(pGryImg, width, height, width/8, height/2, pResImg);
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0501\\Fig0504Gry_3_ByPix82.bmp");
	//86
	RmwPixelThreshold(pGryImg, width, height, width/8, height/6, pResImg);
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0501\\Fig0504Gry_4_ByPix86.bmp");
	// step.4-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	delete pResImg; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}

void testSemiThre()
{
	unsigned char *pGryImg;
	int width, height;
	int histogram[256];
	int thre;

	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0501\\Fig0503Gry.bmp", &width, &height);  //104
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	// step.2-------------����ͼ��--------------------------//
	RmwGetHistogram(pGryImg, width, height, histogram);
	thre = RmwGetOtsuThreshold(histogram, 256);
	RmwSemiThreshold(pGryImg, width, height, thre);
	// step.3-------------����ͼ��--------------------------//
	Rmw_Write8BitImg2BmpFileMark(pGryImg, width, height, "..\\RmwCPP0501\\Fig0503Gry_BySemi.bmp");
	// step.4-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}
 
void main()
{
	testGlobalThre();
	testBlockThre();
	testPixThre();
 	testSemiThre();
}
