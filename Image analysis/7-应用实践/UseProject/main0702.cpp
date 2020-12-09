///////////////////////////////////////////////////////////////////////////////////////
//
//  ������-����ͶӰ
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
#include "RmwALG_EdgeDetector.h"

#include "RmwALG_Project.h"

void TestHor0701()
{
	unsigned char *pGryImg;
	int width, height;

	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0702\\Fig0701a.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	int *prjHor = new int[height];
	// step.2-------------ͼ����--------------------------//
	RmwGetProjectHor(pGryImg, width, height, prjHor);
	// step.3-------------����ͼ��--------------------------//
	RmwDrawPrjHor2Img(prjHor, 200, pGryImg, width, height, true);
	Rmw_Write8BitImg2BmpFile(pGryImg, width, height, "..\\RmwCPP0702\\Fig0701b.bmp");
	// step.4-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	delete prjHor; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}

void TestHor0706()
{
	unsigned char *pGryImg;
	int width, height;

	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0702\\Fig0706a.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	BYTE *pResImg = new BYTE[width*height];
	int *prjHor1 = new int[height];
	int *prjHor2 = new int[height];
	// step.2-------------ͼ����--------------------------//
	//ԭͼ��ͶӰ
	RmwGetProjectHor(pGryImg, width, height, prjHor1);
	//�ݶ�ͼ��ͶӰ
	RmwSobelGryImg(pGryImg, width, height, pResImg);
	RmwGetProjectHor(pResImg, width, height, prjHor2);
	// step.3-------------����ͼ��--------------------------//
	//�����ݶ�ͼ��
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0702\\Fig0706c.bmp");
	//����ԭͼ��ͶӰ
	RmwDrawPrjHor2Img(prjHor1, 220, pResImg, width, height, true);
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0702\\Fig0706b.bmp");
	//�����ݶ�ͼ��ͶӰ
	RmwDrawPrjHor2Img(prjHor2, 120, pResImg, width, height, true);
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0702\\Fig0706d.bmp");
	// step.4-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	delete pResImg; //�ͷ��Լ�������ڴ�
	delete prjHor1; //�ͷ��Լ�������ڴ�
	delete prjHor2; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}

void TestHor0707()
{
	unsigned char *pGryImg;
	int width, height;

	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0702\\Fig0707a.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	BYTE *pResImg = new BYTE[width*height];
	int *prjHor1 = new int[height];
	int *prjHor2 = new int[height];
	// step.2-------------ͼ����--------------------------//
	//ԭͼ��ͶӰ
	RmwGetProjectHor(pGryImg, width, height, prjHor1);
	//�ݶ�ͼ��ͶӰ
	RmwSobelGryImg(pGryImg, width, height, pResImg);
	RmwGetProjectHor(pResImg, width, height, prjHor2);
	// step.3-------------����ͼ��--------------------------//
	//�����ݶ�ͼ��
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0702\\Fig0707c.bmp");
	//����ԭͼ��ͶӰ
	RmwDrawPrjHor2Img(prjHor1, 220, pResImg, width, height, true);
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0702\\Fig0707b.bmp");
	//�����ݶ�ͼ��ͶӰ
	RmwDrawPrjHor2Img(prjHor2, 120, pResImg, width, height, true);
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0702\\Fig0707d.bmp");
	// step.4-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	delete pResImg; //�ͷ��Լ�������ڴ�
	delete prjHor1; //�ͷ��Լ�������ڴ�
	delete prjHor2; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}

void TestHor0708()
{
	unsigned char *pGryImg;
	int width, height;

	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0702\\Fig0708a.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	BYTE *pResImg = new BYTE[width*height];
	int *prjHor1 = new int[height];
	int *prjHor2 = new int[height];
	// step.2-------------ͼ����--------------------------//
	//ԭͼ��ͶӰ
	RmwGetProjectHor(pGryImg, width, height, prjHor1);
	//�ݶ�ͼ��ͶӰ
	RmwSobelGryImg(pGryImg, width, height, pResImg);
	RmwGetProjectHor(pResImg, width, height, prjHor2);
	// step.3-------------����ͼ��--------------------------//
	//�����ݶ�ͼ��
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0702\\Fig0708c1.bmp");
	//����ԭͼ��ͶӰ
	RmwDrawPrjHor2Img(prjHor1, 220, pResImg, width, height, true);
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0702\\Fig0708b.bmp");
	//�����ݶ�ͼ��ͶӰ
	RmwDrawPrjHor2Img(prjHor2, 120, pResImg, width, height, true);
	Rmw_Write8BitImg2BmpFile(pResImg, width, height, "..\\RmwCPP0702\\Fig0708c.bmp");
	// step.4-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	delete pResImg; //�ͷ��Լ�������ڴ�
	delete prjHor1; //�ͷ��Լ�������ڴ�
	delete prjHor2; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}

void main()
{
	TestHor0701();
	TestHor0706();
	TestHor0707();
	TestHor0708();
}
