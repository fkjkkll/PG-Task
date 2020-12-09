#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "bmpFile.h"
#include "RmwALG_MedianFilter.h"
  
void testMedianFilter()
{
	unsigned char *pGryImg;
	int width, height;
	double avrCmpTimes;
	clock_t t1, t2;

	// step.1-------------读图像文件------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0302\\Fig0301Gry.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	BYTE *pResImg = new BYTE[width*height];
	// step.2-------------图像处理--------------------------//
	t1 = clock();
	avrCmpTimes = 0;
	memset(pResImg, 255, width*height);
	for (int i = 0; i<1000; i++);
	{
		avrCmpTimes = RmwMedianFilter(pGryImg, width, height, 13, 13, pResImg);
	}
	t2 = clock();
	printf("%d  ms，avrCmpTimes=%lf\n", t2-t1, avrCmpTimes);
 	// step.3-------------保存图像--------------------------//
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0302\\Fig0301Gry_MedianFilter.bmp");
	// step.4-------------结束------------------------------//
	delete pGryImg; //释放自己申请的内存
	delete pResImg; //释放自己申请的内存
	printf("Press any key to exit!\n");
	getchar();
	return;
}

void main()
{
	testMedianFilter();
}

 