#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "bmpFile.h"
#include "RmwALG_Threshold.h"
 
void testDocumentSegmentation()
{
	unsigned char *pGryImg;
	unsigned char *pBkgImg;
	unsigned char *pResImg;
	int width, height;
	int histogram[256];
	int v, thre;

	// step.1-------------��ͼ���ļ�------------------------//
	//ԭʼͼ��
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0504\\Fig0519a.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	//��������ͼ��
	pBkgImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0504\\Fig0519c_Bkg.bmp", &width, &height);
	if (!pBkgImg)
	{
		delete pGryImg;
		printf("*file open err!\n");
		getchar();
		return;
	}
	//Ŀ��ͼ��
	pResImg = new BYTE[width*height];
	// step.2-------------��������--------------------------//
	for (int i = 0; i<width*height; i++)
	{
		v = pBkgImg[i]-pGryImg[i];
		pResImg[i] = max(0, v);
	}
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0504\\Fig0519d_1_dif.bmp");
	// step.3-------------��ֵ��----------------------------//
	RmwGetHistogram(pResImg, width, height, histogram);
	thre = RmwGetOtsuThreshold(histogram, 256);
	RmwThreshold(pResImg, width, height, thre);
	// step.4-------------����ͼ��--------------------------//
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0504\\Fig0519f_2_bin.bmp");
	// step.5-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	delete pBkgImg; //�ͷ��Լ�������ڴ�
	delete pResImg; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}

void testRiceSegmentation()
{
	unsigned char *pGryImg;
	unsigned char *pBkgImg;
	unsigned char *pResImg;
	int width, height;
	int histogram[256];
	int v, thre;

	// step.1-------------��ͼ���ļ�------------------------//
	//ԭʼͼ��
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0504\\Fig0520a.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	//��������ͼ��
	pBkgImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0504\\Fig0520d_Bkg.bmp", &width, &height);
	if (!pBkgImg)
	{
		delete pGryImg;
		printf("*file open err!\n");
		getchar();
		return;
	}
	//Ŀ��ͼ��
	pResImg = new BYTE[width*height];
	// step.2-------------��������--------------------------//
	for (int i = 0; i<width*height; i++)
	{
		v = pGryImg[i]-pBkgImg[i];
		pResImg[i] = max(0, v);
	}
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0504\\Fig0520f_1_dif.bmp");
	// step.3-------------��ֵ��----------------------------//
	RmwGetHistogram(pResImg, width, height, histogram);
	thre = RmwGetOtsuThreshold(histogram, 256);
	RmwThreshold(pResImg, width, height, thre);
	// step.4-------------����ͼ��--------------------------//
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0504\\Fig0520h_2_bin.bmp");
	// step.5-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	delete pBkgImg; //�ͷ��Լ�������ڴ�
	delete pResImg; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}

void main()
{
	testDocumentSegmentation();
	testRiceSegmentation();
}
