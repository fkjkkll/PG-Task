#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "bmpFile.h"
#include "RmwALG_AverageFilter.h"
  
void testAvrFilterBySumCol()
{
	unsigned char *pGryImg;
	int width, height;

	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0301\\Fig0301Gry.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	BYTE *pResImg = new BYTE[width*height];
	// step.2-------------ͼ����--------------------------//
 	RmwAvrFilterBySumCol(pGryImg, width, height, 9, 9, pResImg);
 	// step.3-------------����ͼ��--------------------------//
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0301\\Fig0301Gry_AvrFilterBySumCol.bmp");
	// step.4-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	delete pResImg; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}

void testSumGryImg()
{
	unsigned char *pGryImg;
	int width, height;
	clock_t t1, t2;
	int i;

	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0301\\Fig0301Gry.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	_declspec(align(16)) int *pSumImg = new int[width*height];
	// step.2-------------ͼ����--------------------------//
	//RmwDoSumGryImg()
	t1 = clock();
	for (i = 0; i<1000; i++)
	{
		//�����к͵Ļ���ͼʵ��
		RmwDoSumGryImg(pGryImg, width, height, pSumImg);
	}
	t2 = clock();
	printf("RmwDoSumGryImg() = %d  ms\n", t2-t1);
	//RmwDoSumGryImg_SSE()
	t1 = clock();
	for (i = 0; i<1000; i++)
	{
		//����SSE�Ļ���ͼʵ��
		RmwDoSumGryImg_SSE(pGryImg, width, height, pSumImg);
	}
	t2 = clock();
	printf("RmwDoSumGryImg_SSE() = %d  ms\n", t2-t1);
	// step.4-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	delete pSumImg; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}
void testAvrFilterBySumImg()
{
	unsigned char *pGryImg;
	int width, height;

	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0301\\Fig0301Gry.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	BYTE *pResImg = new BYTE[width*height];
	_declspec(align(16)) int *pSumImg = new int[width*height];
	// step.2-------------ͼ����--------------------------//
	//����SSE�Ļ���ͼʵ��
	RmwDoSumGryImg_SSE(pGryImg, width, height, pSumImg);
	//���ڻ���ͼ�Ŀ��پ�ֵ�˲�
	RmwAvrFilterBySumImg(pSumImg, width, height, 9, 9, pResImg);
	// step.3-------------����ͼ��--------------------------//
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0301\\Fig0301Gry_AvrFilterBySumImg.bmp");
	// step.4-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	delete pResImg; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}

void main()
{
	testAvrFilterBySumCol();
	testSumGryImg();
	testAvrFilterBySumImg();
}

 