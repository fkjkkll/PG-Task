#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "bmpFile.h"
#include "RmwALG_HoughLine.h"
///////////////////////////////////////////////////////////////////////////////////////
//
//  全局变量
//
///////////////////////////////////////////////////////////////////////////////////////
#define  MAX_POINT_NUM   (512*512)
static int gN;
static int gX[MAX_POINT_NUM];
static int gY[MAX_POINT_NUM];
///////////////////////////////////////////////////////////////////////////////////////
//
//  直线拟合
//
///////////////////////////////////////////////////////////////////////////////////////
void testFittingLine()
{
	unsigned char *pGryImg;
	int width, height;

	// step.1-------------读图像文件------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0601\\Fig0606Bin.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	// step.2-------------收集边缘点------------------------//
	int x, y;
	gN = 0;
	for (y = 0; y<height; y++)
	{
		for (x = 0; x<width; x++)
		{
			if (*(pGryImg+y*width+x))
			{
				*(pGryImg+y*width+x) = 0;
				gX[gN] = x;
				gY[gN] = y;
				gN++;
			}
			else *(pGryImg+y*width+x) = 200;
		}
	}
	Rmw_Write8BitImg2BmpFile(pGryImg, width, height, "..\\RmwCPP0601\\Fig0606Bin_point.bmp");
	// step.3-------------直线拟合--------------------------//
	double A, B, C;
	RmwFittingLine(gX, gY, gN, &A, &B, &C);
	RmwDrawABCLine(pGryImg, width, height, A, B, C, 254);
	Rmw_Write8BitImg2BmpFileMark(pGryImg, width, height, "..\\RmwCPP0601\\Fig0606Bin_FittingLine.bmp");
	// step.4-------------结束------------------------------//
	delete pGryImg; //释放自己申请的内存
	printf("Press any key to exit!\n");
	getchar();
	return;
}
///////////////////////////////////////////////////////////////////////////////////////
//
//  HT直线
//
///////////////////////////////////////////////////////////////////////////////////////
void testHoughLine()
{
	unsigned char *pGryImg;
	int width, height;
	int x, y;

	// step.1-------------读图像文件------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0601\\Fig0606Bin.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	// step.2-------------收集边缘点------------------------//
	gN = 0;
	for (y = 0; y<height; y++)
	{
		for (x = 0; x<width; x++)
		{
			if (*(pGryImg+y*width+x))
			{
				*(pGryImg+y*width+x) = 0;
				gX[gN] = x;
				gY[gN] = y;
				gN++;
			}
			else *(pGryImg+y*width+x) = 200;
		}
	}
	Rmw_Write8BitImg2BmpFileMark(pGryImg, width, height, "..\\RmwCPP0601\\Fig0606Bin_point.bmp");
	// step.3-------------霍夫变换--------------------------//
	double A, B, C;
	RmwHoughLine(width, height, gX, gY, gN, &A, &B, &C);
	RmwDrawABCLine(pGryImg, width, height, A, B, C, 255);
	Rmw_Write8BitImg2BmpFileMark(pGryImg, width, height, "..\\RmwCPP0601\\Fig0606Bin_HoughLine.bmp");
	// step.4-------------结束------------------------------//
	delete pGryImg; //释放自己申请的内存
	printf("Press any key to exit!\n");
	getchar();
	return;
}

void main()
{
	testFittingLine();
	testHoughLine();
}
