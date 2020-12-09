#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "bmpFile.h"
#include "RmwALGTest.h"

void testInvertGryImg()
{
	unsigned char *pGryImg;
	int width, height;

	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0101\\0101Gry.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	// step.2-------------ͼ����--------------------------//
	RmwInvertGryImg7(pGryImg, width, height);
	// step.3-------------����ͼ��--------------------------//
	Rmw_Write8BitImg2BmpFile(pGryImg, width, height, "..\\RmwCPP0101\\0101Gry_invert.bmp");
	Rmw_Write8BitImg2BmpFileRed(pGryImg, width, height, "..\\RmwCPP0101\\0101Gry_invert_Red.bmp");
	// step.4-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}

void testInvertRgbImg()
{
	unsigned char *pRgbImg;
	int width, height;

	// step.1-------------��ͼ���ļ�------------------------//
	pRgbImg = Rmw_Read24BitBmpFile2Img("..\\RmwCPP0101\\0101Rgb.bmp", &width, &height);
	if (!pRgbImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	// step.2-------------ͼ����--------------------------//
	RmwInvertRgbImg(pRgbImg, width, height);
	// step.3-------------����ͼ��--------------------------//
	Rmw_Write24BitImg2BmpFile(pRgbImg, width, height, "..\\RmwCPP0101\\0101Rgb_invert.bmp");
	// step.4-------------����------------------------------//
	delete pRgbImg; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}
 

void main()
{
	testInvertGryImg();
	testInvertRgbImg();
}