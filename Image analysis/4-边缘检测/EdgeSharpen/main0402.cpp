#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "bmpFile.h"
#include "RmwALG_EdgeSharpen.h"
 
void testEdgeSharpen()
{
	unsigned char *pGryImg;
	int width, height;

	// step.1-------------读图像文件------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0402\\Fig0424Gry.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	BYTE *pResImg = new BYTE[width*height];
	// step.2-------------处理图像--------------------------//
 	//0.5
	RmwEdgeShapen(pGryImg, width, height, 0.5, pResImg);
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0402\\Fig0424Gry_1_EdgeShapen_0.5.bmp");
	//2
	RmwEdgeShapen(pGryImg, width, height, 2, pResImg);
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0402\\Fig0424Gry_2_EdgeShapen_2.bmp");
	//4
	RmwEdgeShapen(pGryImg, width, height, 4, pResImg);
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0402\\Fig0424Gry_3_EdgeShapen_4.bmp");
	//8
	RmwEdgeShapen(pGryImg, width, height, 8, pResImg);
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0402\\Fig0424Gry_4_EdgeShapen_8.bmp");
	// step.4-------------结束------------------------------//
	delete pGryImg; //释放自己申请的内存
	delete pResImg; //释放自己申请的内存
	printf("Press any key to exit!\n");
	getchar();
	return;
}
  
void main()
{
	testEdgeSharpen();
}
