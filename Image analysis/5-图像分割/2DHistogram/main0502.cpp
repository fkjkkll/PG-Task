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

	// step.1-------------读图像文件------------------------//
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
	// step.2-------------对角线直方图-----------------------//
	num = RmwHistogramBy2D(pGryImg, pAvrImg, width, height, 4*2, histogram);
	RmwDrawHistogram2Img(histogram, 256, 1.0, pResImg, width, height);
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0502\\Fig0506Gry_1_2DHistogram.bmp");
	// step.3-------------二值化----------------------------//
	thre = RmwGetOtsuThreshold(histogram, 256);
	printf("\nnum=%d(%.3lf),thre=%d\n", num, num*1.0/(width*height), thre);
	//二值图像
	RmwThreshold(pGryImg, width, height, thre);
	Rmw_Write8BitImg2BmpFile(pGryImg, width, height, "..\\RmwCPP0502\\Fig0506Gry_2_Bin.bmp");
	// step.4-------------结束------------------------------//
	delete pGryImg; //释放自己申请的内存
	delete pAvrImg; //释放自己申请的内存
	delete pResImg; //释放自己申请的内存
	printf("Press any key to exit!\n");
	getchar();
	return;
}

void main()
{
	test2Dhistogram();
}

 