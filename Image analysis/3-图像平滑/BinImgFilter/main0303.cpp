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

	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0303\\Fig0301Gry.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	//��ֵ��
	for (int i = 0; i<width*height; i++) pGryImg[i] = (pGryImg[i]>206)*255;
	Rmw_Write8BitImg2BmpFile(pGryImg, width, height, "..\\RmwCPP0303\\Fig0301BinImg.bmp");
	// step.2-------------ͼ����--------------------------//
	BYTE *pResImg = new BYTE[width*height];
	memset(pResImg, 255, width*height);
	//MinFilter
	RmwBinImgFilter( pGryImg,  //ԭʼ��ֵͼ��
	                 width, height,//ͼ��Ŀ�Ⱥ͸߶�
	                 11, 11, //�˲�����M��N��
	                 255, //�Ҷ���ֵ,���ڵ��ڸ�ֵʱ�����255
	                 pResImg //���ͼ��
                   );
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0303\\Fig0301BinImg_MinFilter.bmp");
	//MaxFilter
	RmwBinImgFilter( pGryImg,  //ԭʼ��ֵͼ��
	                 width, height,//ͼ��Ŀ�Ⱥ͸߶�
	                 11, 11, //�˲�����M��N��
	                 1, //�Ҷ���ֵ,���ڵ��ڸ�ֵʱ�����255
	                 pResImg //���ͼ��
                   );
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0303\\Fig0301BinImg_MaxFilter.bmp");
	//MedianFilter
	RmwBinImgFilter( pGryImg,  //ԭʼ��ֵͼ��
	                 width, height,//ͼ��Ŀ�Ⱥ͸߶�
	                 11, 11, //�˲�����M��N��
	                 128, //�Ҷ���ֵ,���ڵ��ڸ�ֵʱ�����255
	                 pResImg //���ͼ��
                   );
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0303\\Fig0301BinImg_MedFilter.bmp");
	// step.4-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	delete pResImg; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}

void main()
{
	testBinImgFilter();
}

 