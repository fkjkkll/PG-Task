#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "bmpFile.h"
#include "RmwALG_ContourTrace.h"
#include "RmwALG_ContourFill.h"

///////////////////////////////////////////////////////////////////////////////////////
//
//  ȫ�ֱ���
//
///////////////////////////////////////////////////////////////////////////////////////
#define MAX_CONOUR_LENGTH  (512*512)
BYTE gChainCode[MAX_CONOUR_LENGTH];

void testTrace(BYTE*pBinImg,int width,int height, BYTE *pChainCode,BYTE *pDbgImg)
{
	BYTE *pCur;
	int x, y;
	int N,S;
	int x1, x2, y1, y2;

	//����
	memset(pDbgImg, 0, width*height);
	// step.1-----Ϊ�˱������ʱ�ĳ�����,���Ƚ�ͼ��߿�0-------//
	RmwSetImageBoundary(pBinImg, width, height, 0);
	// step.2-----��������,��������ɨ��ͼ��Ѱ���µ�����----------//
	for (y = 1, pCur = pBinImg+y*width; y<height-1; y++) //��������
	{
		pCur++; //����������
		for (x = 1; x<width-1; x++, pCur++) //��������
		{
			if ( (*pCur==255)&& (*(pCur-1)<=1) ) //����һ��������
			{   //�������������㷨,��������������
				N = RmwTraceContour( pBinImg, width, height,
					                 x, y, //�������
					                 true, //��������
					                 pChainCode, //����������������
					                 MAX_CONOUR_LENGTH //����Ĵ�С
				                   );
				//���
				S = RmwContourPixels(pChainCode, N);
				printf("\nS=%d", S);
				//��Ӿ���
				RmwContourRect(x, y, pChainCode, N, &x1, &x2, &y1, &y2);
				if ( (x1>1)&&(y1>1)&&(x2<width-2)&&(y2<height-2)&& //ȥ��������
					 (S>200)&& //ȥ����ȱ�� 
					 (S<400) //ȥ��ճ����
				   )
				{
					RmwFillContour( pDbgImg, width, height,//�����ͼ��
						            x, y, //�������
						            true, //�Ƿ���������
						            pChainCode, //����
						            N, //�������
						            255, //�����ڲ�����ɫ
						            255, //��������ɫ
						            250 //�����ڵ���ɫ
					              );
				}
				else RmwDrawContour(pDbgImg, width, x, y, pChainCode, N, 253);
			}
			else if ( (*pCur==0)&& (*(pCur-1)>=254) ) //����һ��������
			{   //�������������㷨,��������������
				N = RmwTraceContour( pBinImg, width, height,
					                 x-1, y,  //�������,ע����x-1
					                 false, //��������
					                 pChainCode, //����������������
					                 MAX_CONOUR_LENGTH //����Ĵ�С
				                   );
				//����
				RmwDrawContour(pDbgImg, width, x-1, y, pChainCode, N, 253);
			}
		}
		pCur++; //�������Ҳ��
	}
}

void main()
{
	unsigned char *pBinImg;
	int width, height;
	int i;

	// step.1-------------��ͼ���ļ�------------------------//
	pBinImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0603\\Fig0520h.bmp", &width, &height);
	if (!pBinImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	// step.2-------------����ͼ��--------------------------//
	BYTE *pDbgImg = new BYTE[width*height];
	testTrace(pBinImg, width, height, gChainCode, pDbgImg);
 	// step.3-------------����ͼ��--------------------------//
	Rmw_Write8BitImg2BmpFileMark(pDbgImg, width, height, "..\\RmwCPP0603\\Fig0520h_traceAndFill.bmp");
	//������3x3�Ŀ�������
	for (i = 0; i<width*height; i++)
	{
		pDbgImg[i] = (pBinImg[i]>200)*255;
	}
	Rmw_Write8BitImg2BmpFile(pDbgImg, width, height, "..\\RmwCPP0603\\Fig0520h_����.bmp");
	//������3x3�Ŀ��ٸ�ʴ
	for (i = 0; i<width*height; i++)
	{
		pDbgImg[i] = (pBinImg[i]==255)*255;
	}
	Rmw_Write8BitImg2BmpFile(pDbgImg, width, height, "..\\RmwCPP0603\\Fig0520h_��ʴ.bmp");
	// step.4-------------����------------------------------//
	delete pBinImg; //�ͷ��Լ�������ڴ�
	delete pDbgImg; //�ͷ��Լ�������ڴ�
	printf("\nPress any key to exit!\n");
	getchar();
	return;
}
