#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "bmpFile.h"
#include "RmwALG_Histogram.h"
  
void testHistogramEqualize()
{
	unsigned char *pGryImg;
	int width, height;

	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0202\\H0203Girl.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	// step.2-------------����ͼ��--------------------------//
	RmwHistogramEqualize(pGryImg, width, height);
	// step.3-------------����ͼ��--------------------------//
	Rmw_Write8BitImg2BmpFile(pGryImg, width, height, "..\\RmwCPP0202\\H0203Girl_histEq.bmp");
	// step.4-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}

void testHistogramPixel()
{
	unsigned char *pGryImg;
	int width, height;

	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0202\\Fig0217.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	BYTE *pResImg = new BYTE[width*height];
	// step.2-------------����ͼ��--------------------------//
	RmwHistogramEqualizePixel(pGryImg, width, height, 128, 128, pResImg);
	// step.3-------------����ͼ��--------------------------//
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0202\\Fig0217_Pix.bmp");
	// step.4-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	delete pResImg; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}


void testHistogramEqualize14Bit()
{
	// step.1-------------��ͼ���ļ�------------------------//
	short int *pGryImg;
	int width = 640, height = 480;

	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = new short int[width*height];
	FILE *fp = fopen("..\\RmwCPP0202\\H0204IR14bit.raw", "rb");
	fread(pGryImg, sizeof(short int), width*height, fp);
	fclose(fp);
	// step.2-------------ֱ��ͼ���⻯----------------------//
	BYTE *pResImg = new BYTE[width*height];
	RmwHistogramEqualize(pGryImg, width, height, pResImg);
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0202\\H0204IR14bit_histEq.bmp");
	// step.3-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	delete pResImg; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}


void main()
{
	testHistogramEqualize();
	//testHistogramPixel();
	//testHistogramEqualize14Bit();
}
