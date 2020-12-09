///////////////////////////////////////////////////////////////////////////////////////
//
//  第七章-测试RL
//
///////////////////////////////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "bmpFile.h"
#include "RmwRoadLaneRL.h"

class RmwRoadLaneRL gRoadLane;
 
void TestRoadLane_ByClass()
{
	unsigned char *pGryImg;
	int width, height;
	int i, j;

	// step.1-------------读图像文件------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0703\\Fig0702Gry.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	for (i = 0; i<width*height; i++) pGryImg[i] = min(250, pGryImg[i]);
	//调试
	BYTE *pDbgImg = new BYTE[width*height];
	memset(pDbgImg, 0, width*height);
	// step.2-------------图像处理--------------------------//
	int blockNum = 20; //分成20个水平图像块
	int estLaneW = 18; //行道线宽度18
	int blockHeight = height/blockNum; //每块图像的高度
	int pX1[32], pX2[32], N; //每块的候选线段检测结果
	BYTE *pBlockImg; //块图像的内存地址
	//对象初始化
	gRoadLane.Initialize(width, blockHeight, estLaneW);
	//分块行道线的线段检测
	for (i = 0; i<blockNum; i++)
	{
		//分块检测
		pBlockImg = pGryImg+i*blockHeight*width;
		gRoadLane.DoNext(pBlockImg, i);
		//取出行道线位置
		N = gRoadLane.GetRL(pX1, pX2, 32);
		//调试:画行道线位置到原始图像中,画在块的中心行上
		for (j = 0; j<N; j++)
		{
			memset( pBlockImg+(blockHeight/2)*width+pX1[j],
				    255, //画成红色线段
				    pX2[j]-pX1[j]+1
			      );
		}
	}
	//调试
	Rmw_Write8BitImg2BmpFileMark(pGryImg, width, height, "..\\RmwCPP0703\\res_Fig0705.bmp");
	// step.3-------------结束------------------------------//
	delete pGryImg; //释放自己申请的内存
	delete pDbgImg; //释放自己申请的内存
	printf("Press any key to exit!\n");
	getchar();
	return;
}

void main()
{
 	TestRoadLane_ByClass();
}
