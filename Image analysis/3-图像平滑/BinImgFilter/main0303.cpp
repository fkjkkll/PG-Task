#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "bmpFile.h"
#include "RmwALG_BinImgFilter.h"
  
void testBinImgFilter()
{
	unsigned char *pGryImg;
	int width, height;

	// step.1-------------读图像文件------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0303\\Fig0301Gry.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	//二值化
	for (int i = 0; i<width*height; i++) pGryImg[i] = (pGryImg[i]>206)*255;
	Rmw_Write8BitImg2BmpFile(pGryImg, width, height, "..\\RmwCPP0303\\Fig0301BinImg.bmp");
	// step.2-------------图像处理--------------------------//
	BYTE *pResImg = new BYTE[width*height];
	memset(pResImg, 255, width*height);
	//MinFilter
	RmwBinImgFilter( pGryImg,  //原始二值图像
	                 width, height,//图像的宽度和高度
	                 11, 11, //滤波邻域：M列N行
	                 255, //灰度阈值,大于等于该值时结果赋255
	                 pResImg //结果图像
                   );
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0303\\Fig0301BinImg_MinFilter.bmp");
	//MaxFilter
	RmwBinImgFilter( pGryImg,  //原始二值图像
	                 width, height,//图像的宽度和高度
	                 11, 11, //滤波邻域：M列N行
	                 1, //灰度阈值,大于等于该值时结果赋255
	                 pResImg //结果图像
                   );
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0303\\Fig0301BinImg_MaxFilter.bmp");
	//MedianFilter
	RmwBinImgFilter( pGryImg,  //原始二值图像
	                 width, height,//图像的宽度和高度
	                 11, 11, //滤波邻域：M列N行
	                 128, //灰度阈值,大于等于该值时结果赋255
	                 pResImg //结果图像
                   );
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0303\\Fig0301BinImg_MedFilter.bmp");
	// step.4-------------结束------------------------------//
	delete pGryImg; //释放自己申请的内存
	delete pResImg; //释放自己申请的内存
	printf("Press any key to exit!\n");
	getchar();
	return;
}

void main()
{
	testBinImgFilter();
}

 