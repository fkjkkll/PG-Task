#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "bmpFile.h"
#include "RmwALG_LinearStretch.h"
  
void testKB()
{
	unsigned char *pGryImg;
	int width, height;
	clock_t t1, t2;
	int i;

	// step.1-------------读图像文件------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0201\\Fig0201Gry.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	BYTE *pResImg = new BYTE[width*height];
	// step.2-------------图像处理--------------------------//
	double K = 3.1875;
	double B = -43.75;
	//Demo2
	t1 = clock();
	for (i = 0; i<1000; i++)
	{
		LinearStretchDemo2(pGryImg, width, height, K, B, pResImg);
	}
	t2 = clock();
	printf("LinearStretchDemo2 = %d  ms\n", t2-t1);
	//Demo1
	t1 = clock();
	for (i = 0; i<1000; i++)
	{
		LinearStretchDemo1(pGryImg, width, height, K, B, pResImg);
	}
	t2 = clock();
	printf("LinearStretchDemo1 = %d  ms\n", t2-t1);
	// step.3-------------保存图像--------------------------//
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0201\\Fig0201Gry_LinearStretch.bmp");
	// step.4-------------结束------------------------------//
	delete pGryImg; //释放自己申请的内存
	delete pResImg; //释放自己申请的内存
	printf("Press any key to exit!\n");
	getchar();
	return;
}

void task1() {

	BYTE *pGryImg;
	clock_t t1, t2;
	int i;
	int width, height;
	// step.1-------------读图像文件------------------------//

	pGryImg = Rmw_Read24BitBmpFile2Img("..\\RmwCPP0201\\H0201Rgb.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	BYTE *pResImg = new BYTE[width*height];
	// step.2-------------图像处理--------------------------//
	t1 = clock();
	for (i = 0; i < 1000; i++) {
		Color2GrayDemo1(pGryImg, width, height, pResImg);
	}
	t2 = clock();
	printf("Color2GrayDemo1 = %d  ms\n", t2 - t1);


	t1 = clock();
	for (i = 0; i < 1000; i++) {
		Color2GrayDemo2(pGryImg, width, height, pResImg);
	}
	t2 = clock();
	printf("Color2GrayDemo2 = %d  ms\n", t2 - t1);
	// step.3-------------保存图像--------------------------//
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0201\\H0201gray.bmp");

	// step.4-------------结束------------------------------//
	delete pGryImg; //释放自己申请的内存
	delete pResImg; //释放自己申请的内存
	printf("Press any key to exit!\n");
	getchar();
	return;
}

void main()
{
	//testKB();
	task1();
}
