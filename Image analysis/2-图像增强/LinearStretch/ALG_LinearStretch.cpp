///////////////////////////////////////////////////////////////////////////////////////////
//
//   第二章
//
///////////////////////////////////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

///////////////////////////////////////////////////////////////////////////////////////
//
//线性拉伸
//
///////////////////////////////////////////////////////////////////////////////////////
void LinearStretchDemo1(BYTE *pGryImg, int width, int height, double k, double b, BYTE *pResImg)
{
	BYTE *pCur, *pEnd, *pRes;

	pEnd = pGryImg+width*height;
	for (pCur = pGryImg, pRes = pResImg; pCur<pEnd; pCur++)
	{
		*(pRes++) = max(0, min(255, (int)(k*(*pCur)+b))); //要对k*(*pCur)+b执行饱和运算;
	}
	return;
}

void LinearStretchDemo2(BYTE *pGryImg, int width, int height, double k, double b, BYTE *pResImg)
{
	BYTE *pCur, *pEnd, *pRes;
	//int LUT[256]; //因为只有[0,255]256个灰度值
	BYTE LUT[256]; //因为只有[0,255]256个灰度值

	// step.1-------------生成查找表------------------------//
	for (int g = 0; g<256; g++)
	{
		LUT[g] = max(0, min(255, (int)(k*g+b))); //勿忘饱和运算
	}
	// step.2-------------进行变换--------------------------//
	pEnd = pGryImg+width*height;
	for (pCur = pGryImg, pRes = pResImg; pCur<pEnd; pCur++)
	{
		*(pRes++) = LUT[*pCur];
	}
	// step.3-------------结束------------------------------//
	return;
}


void Color2GrayDemo1(BYTE *pColImg, int width, int height, BYTE *pResImg) {
	BYTE *pCur, *pEnd, *pRes;
	pEnd = pColImg + width * height * 3;
	for (pCur = pColImg, pRes = pResImg; pCur < pEnd; pCur+=3) {
		*(pRes++) = int(*(pCur)*0.299 + *(pCur + 1)*0.587 + *(pCur + 2)*0.114);
	}
	return;
}

void Color2GrayDemo2(BYTE *pColImg, int width, int height, BYTE *pResImg) {
	BYTE *pCur, *pEnd, *pRes;
	pEnd = pColImg + width * height * 3;
	BYTE LUT[3][256];
	double para[3] = { 0.299, 0.587, 0.114 };
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 256; j++) {
			LUT[i][j] = int(j*para[i]);
		}
	}
	for (pCur = pColImg, pRes = pResImg; pCur < pEnd; pCur += 3) {
		*(pRes++) = LUT[0][*(pCur)] + LUT[1][*(pCur+1)] + LUT[2][*(pCur+2)];
	}
	return;
}










