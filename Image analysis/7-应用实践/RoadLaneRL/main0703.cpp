///////////////////////////////////////////////////////////////////////////////////////
//
//  ������-����RL
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

	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0703\\Fig0702Gry.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	for (i = 0; i<width*height; i++) pGryImg[i] = min(250, pGryImg[i]);
	//����
	BYTE *pDbgImg = new BYTE[width*height];
	memset(pDbgImg, 0, width*height);
	// step.2-------------ͼ����--------------------------//
	int blockNum = 20; //�ֳ�20��ˮƽͼ���
	int estLaneW = 18; //�е��߿��18
	int blockHeight = height/blockNum; //ÿ��ͼ��ĸ߶�
	int pX1[32], pX2[32], N; //ÿ��ĺ�ѡ�߶μ����
	BYTE *pBlockImg; //��ͼ����ڴ��ַ
	//�����ʼ��
	gRoadLane.Initialize(width, blockHeight, estLaneW);
	//�ֿ��е��ߵ��߶μ��
	for (i = 0; i<blockNum; i++)
	{
		//�ֿ���
		pBlockImg = pGryImg+i*blockHeight*width;
		gRoadLane.DoNext(pBlockImg, i);
		//ȡ���е���λ��
		N = gRoadLane.GetRL(pX1, pX2, 32);
		//����:���е���λ�õ�ԭʼͼ����,���ڿ����������
		for (j = 0; j<N; j++)
		{
			memset( pBlockImg+(blockHeight/2)*width+pX1[j],
				    255, //���ɺ�ɫ�߶�
				    pX2[j]-pX1[j]+1
			      );
		}
	}
	//����
	Rmw_Write8BitImg2BmpFileMark(pGryImg, width, height, "..\\RmwCPP0703\\res_Fig0705.bmp");
	// step.3-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	delete pDbgImg; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}

void main()
{
 	TestRoadLane_ByClass();
}
