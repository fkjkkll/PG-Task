#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "bmpFile.h"
#include "RmwALG_EdgeDetector.h"
 
void testGradient()
{
	unsigned char *pGryImg;
	int width, height;

	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0401\\Fig0413Gry.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	BYTE *pResImg = new BYTE[width*height];
	// step.2-------------����ͼ��--------------------------//
 	RmwGradientGryImg(pGryImg, width, height, pResImg);
	// step.3-------------����ͼ��--------------------------//
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0401\\Fig0413Gry_1_Gradient.bmp");
	// step.4-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	delete pResImg; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}
 
void testRoberts()
{
	unsigned char *pGryImg;
	int width, height;

	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0401\\Fig0413Gry.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	BYTE *pResImg = new BYTE[width*height];
	// step.2-------------����ͼ��--------------------------//
	RmwRobertsGryImg(pGryImg, width, height, pResImg);
	// step.3-------------����ͼ��--------------------------//
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0401\\Fig0413Gry_2_Roberts.bmp");
	// step.4-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	delete pResImg; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}

void testSobel()
{
	unsigned char *pGryImg;
	int width, height;

	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0401\\Fig0413Gry.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	BYTE *pResImg = new BYTE[width*height];
	// step.2-------------����ͼ��--------------------------//
	RmwSobelGryImg(pGryImg, width, height, pResImg);
	// step.3-------------����ͼ��--------------------------//
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0401\\Fig0413Gry_3_Sobel.bmp");
	// step.4-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	delete pResImg; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}

void testPrewitt()
{
	unsigned char *pGryImg;
	int width, height;

	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0401\\Fig0413Gry.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	BYTE *pResImg = new BYTE[width*height];
	// step.2-------------����ͼ��--------------------------//
	RmwPrewittGryImg(pGryImg, width, height, pResImg);
	// step.3-------------����ͼ��--------------------------//
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0401\\Fig0413Gry_4_Prewitt.bmp");
	// step.4-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	delete pResImg; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}

void testShenJun()
{
	unsigned char *pGryImg;
	int width, height;

	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0401\\Fig0413Gry.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	BYTE *pResImg = new BYTE[width*height];
	BYTE *pTmpImg = new BYTE[width*height];
	// step.2-------------����ͼ��--------------------------//
	//0.05
	RmwShenJunGryImg(pGryImg, pTmpImg, width, height, 0.05, pResImg);
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0401\\Fig0413Gry_5_ShenJun_0.05.bmp");
	//0.5
	RmwShenJunGryImg(pGryImg, pTmpImg, width, height, 0.5, pResImg);
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0401\\Fig0413Gry_6_ShenJun_0.5.bmp");
	//0.95
	RmwShenJunGryImg(pGryImg, pTmpImg, width, height, 0.75, pResImg);
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0401\\Fig0413Gry_7_ShenJun_0.75.bmp");
	// step.4-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	delete pResImg; //�ͷ��Լ�������ڴ�
	delete pTmpImg; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}

void testExtractRiceEdge()
{
	unsigned char *pGryImg;
	int width, height;

	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0401\\Fig0431Gry.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	BYTE *pResImg = new BYTE[width*height];
	BYTE *pTmpImg = new BYTE[width*height];
	// step.2-------------����ͼ��--------------------------//
	RmwExtractRiceEdge(pGryImg, pTmpImg, width, height, 0.5, 32*2, pResImg);
	// step.3-------------����ͼ��--------------------------//
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0401\\Fig0431Gry_1_Edge.bmp");
	for (int i = 0; i<width*height; i++)
	{
		*(pGryImg+i) = max(pGryImg[i], pResImg[i]);
	}
	Rmw_Write8BitImg2BmpFileMark(pGryImg, width, height, "..\\RmwCPP0401\\Fig0431Gry_2_Overlay.bmp");
	// step.4-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	delete pResImg; //�ͷ��Լ�������ڴ�
	delete pTmpImg; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}
void main()
{
	testGradient();
	testRoberts();
	testSobel();
	testPrewitt();
	testShenJun();
	testExtractRiceEdge();
}

 