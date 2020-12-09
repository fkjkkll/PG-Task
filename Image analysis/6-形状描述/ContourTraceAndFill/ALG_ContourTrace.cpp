///////////////////////////////////////////////////////////////////////////////////////////
//
//������-Բ����任
//
///////////////////////////////////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <nmmintrin.h>

///////////////////////////////////////////////////////////////////////////////////////
//
//������
//
///////////////////////////////////////////////////////////////////////////////////////
void RmwDrawContour(BYTE *pGryImg, int width, int x, int y, BYTE *pCode, int N, BYTE color)
{
	static int dx[8] = { 1, 1, 0,-1,-1,-1, 0, 1 };
	static int dy[8] = { 0,-1,-1,-1, 0, 1, 1, 1 };
	int i, dADD[8];
	BYTE *pCur;

	*(pGryImg+y*width+x) = color; // Ϊ���ܹ������պϵ�����
	for (i = 0; i<8; i++) dADD[i] = dy[i]*width+dx[i];
	for (pCur = pGryImg+y*width+x, i = 0; i<N; i++)
	{
		pCur += dADD[pCode[i]];
		*pCur = color;
	}
	return;
}
///////////////////////////////////////////////////////////////////////////////////////
//
//��Ӿ���
//
///////////////////////////////////////////////////////////////////////////////////////
void RmwContourRect(int x0, int y0, BYTE *pCode, int N, int *x1, int *x2, int *y1, int *y2)
{
	static int dx[8] = { 1, 1, 0,-1,-1,-1, 0, 1 };
	static int dy[8] = { 0,-1,-1,-1, 0, 1, 1, 1 };
	int i;

	*y1 = *y2 = y0;
	*x1 = *x2 = x0;
	for (i = 0; i<N-1; i++)
	{
		x0 = x0+dx[pCode[i]];
		y0 = y0+dy[pCode[i]];

		if (x0>*x2) *x2 = x0;
		else if (x0<*x1) *x1 = x0;

		if (y0>*y2) *y2 = y0;
		else if (y0<*y1) *y1 = y0;
	}
	return;
}
///////////////////////////////////////////////////////////////////////////////////////
//
//�ܳ�
//
///////////////////////////////////////////////////////////////////////////////////////
double RmwContourPerimeter(BYTE *pChainCode, int n)
{
	int no, ne, i;

	no = 0;
	for (i = 0; i<n; i++)
	{
		no += pChainCode[i]&0x01; //���������
	}
	ne = (n-no); //ż�������
	return ne+no*sqrt(2.0);
}
///////////////////////////////////////////////////////////////////////////////////////
//
//���
//
///////////////////////////////////////////////////////////////////////////////////////
double RmwContourArea(BYTE *pChainCode, int n)
{
	static int dx[8] = { 1, 1, 0,-1,-1,-1, 0, 1 };
	static int dy[8] = { 0,-1,-1,-1, 0, 1, 1, 1 };
	int i,yi,ci;
	double S=0.0;

	yi = 0; //����ٶ�һ����ֵ����,��Ϊ���������޹�
	for (i = 0; i<n; i++)
	{
		ci = pChainCode[i];
		S += (yi+dy[ci]/2.0)*dx[ci];
		yi += dy[ci]; //��һ�������������
	}
	return fabs(S); //��������ʱ������ʱΪ��,˳ʱ������ʱΪ��,����ȡ����ֵ
}

double RmwContourPerimeterAndArea(BYTE *pChainCode, int n,double *Perimeter)
{
	static int dx[8] = { 1, 1, 0,-1,-1,-1, 0, 1 };
	static int dy[8] = { 0,-1,-1,-1, 0, 1, 1, 1 };
	int i, yi, ci, no;
	double S = 0.0;

	no = 0;
	yi = 0; //����ٶ�һ����ֵ����,��Ϊ���������޹�
	for (i = 0; i<n; i++)
	{
		ci = pChainCode[i];
		//�������
		S += (yi+dy[ci]/2.0)*dx[ci];
		yi += dy[ci]; //��һ�������������
		//�ܳ�����
		no += ci&0x01; //���������
	}
	//�ܳ�
	*Perimeter= (n-no)+no*sqrt(2.0);
	//ʹ���ܳ������������
	return fabs(S)+(*Perimeter/2);
}
///////////////////////////////////////////////////////////////////////////////////////
//
//������Χ�����ظ���
//
///////////////////////////////////////////////////////////////////////////////////////
int RmwContourPixels(BYTE *pChainCode, int n)
{
	static int dy[8] = { 0,-1,-1,-1, 0, 1, 1, 1 };
	static int typeLUT[8][8] = { 1,1,1,3,3,2,2,1,//0
		                         1,1,1,3,3,3,2,1,//1
		                         2,2,2,0,0,0,0,2,//2
		                         2,2,2,0,0,0,0,3,//3
		                         3,2,2,0,0,0,0,3,//4
		                         3,3,2,0,0,0,0,3,//5
		                         1,1,1,2,2,2,2,1,//6
		                         1,1,1,3,2,2,2,1 //7
	                           };
	int i, ci_1,ci,type,yi;
	int num = 0;

	num = 0;
	yi = 0; //����ٶ�һ����ֵ����,��Ϊ���������޹�
	ci_1 = pChainCode[n-1]; //���Ľ�������(����ĩβ������)
	for (i = 0; i<n; i++)
	{
		ci = pChainCode[i]; //��������
		type = typeLUT[ci_1][ci]; //����������
		if (type==0) num -= yi; 
		else if (type==1) num += yi+1;
		else if (type==3) num += 1;
		yi += dy[ci]; //��һ�������������
		ci_1 = ci; //��һ��������Ľ�������
	}
	return abs(num); //��������ʱ������ʱΪ��,˳ʱ������ʱΪ��,����ȡ����ֵ
}
///////////////////////////////////////////////////////////////////////////////////////
//
//�����Ƿ�������Χ
//
///////////////////////////////////////////////////////////////////////////////////////
bool RmwIsPixelInContour(int x0, int y0, BYTE *pChainCode, int n, int x, int y)
{
	static int dx[8] = { 1, 1, 0,-1,-1,-1, 0, 1 };
	static int dy[8] = { 0,-1,-1,-1, 0, 1, 1, 1 };
	int DY[8][8] = { 0 ,1 ,1 ,1 ,1 ,0 ,0 ,0,
		             0 ,1 ,1 ,1 ,1 ,0 ,0 ,0,
		             0 ,1 ,1 ,1 ,1 ,0 ,0 ,0,
		             0 ,1 ,1 ,1 ,1 ,0 ,0 ,0,
		            -1, 0 ,0 ,0 ,0,-1,-1,-1,
		            -1, 0 ,0 ,0 ,0,-1,-1,-1,
		            -1, 0 ,0 ,0 ,0,-1,-1,-1,
		            -1, 0 ,0 ,0 ,0,-1,-1,-1,
	              };
	int CY[8][8] = { 0, 0, 0, 0, 0, 0, 0, 0,
		             0, 0, 0, 0, 0, 1, 0, 0,
		             0, 0, 0, 0, 0, 1, 1, 0,
		             0, 0, 0, 0, 0, 1, 1, 1,
		             1, 0, 0, 0, 0, 1, 1, 1,
		             1, 1, 0, 0, 0, 1, 1, 1,
		             1, 1, 1, 0, 0, 1, 1, 1,
		             1, 1, 1, 1, 0, 1, 1, 1,
	               };
	int sum, i, V, J;
	int pre, cur;

	pre = pChainCode[n-1];
	for (sum = 0, i = 0; i<n; i++)
	{
		cur = pChainCode[i];
		V = ((x0-x)>=0) && ((y0-y)==0);
		J = ((x0-x)==0) && ((y0-y)==0);
		sum += V*DY[pre][cur]+J*CY[pre][cur];
		//Next
		x0 = x0+dx[cur];
		y0 = y0+dy[cur];
		pre=cur;
	}
	//������ʱ,������������ڰ�Χ���ص�sumֵ����1,���������ص�sumֵΪ0
	//������ʱ,����������������ص�sum��ֵ����0,�����ڰ�Χ���ص�sumֵ��-1
	return (sum!=0);
}
///////////////////////////////////////////////////////////////////////////////////////
//
//ͼ��߿�����
//
///////////////////////////////////////////////////////////////////////////////////////
void RmwSetImageBoundary(BYTE *pBinImg, int width, int height, BYTE color)
{
	BYTE *pRow;
	int y;

	memset(pBinImg, color, width);
	memset(pBinImg+(height-1)*width, color, width);
	for (y = 0, pRow = pBinImg; y<height; y++, pRow += width)
	{
		*pRow = 0;
		*(pRow+width-1) = 0;
	}
	return;
}
///////////////////////////////////////////////////////////////////////////////////////
//
//���������㷨
//
///////////////////////////////////////////////////////////////////////////////////////
int RmwTraceContour( BYTE *pBinImg, int width, int height,//��ֵͼ��
	                 int x0, int y0, //�������
	                 bool isOuter, //�Ƿ���������
	                 BYTE *pChainCode, //�ⲿ����õ�һ���ڴ�ռ䣬�����������
	                 int maxChainCodeNum //���Դ�ŵ�����������
                   )
{
	static int dx[8] = { 1, 1, 0,-1,-1,-1, 0, 1 };
	static int dy[8] = { 0,-1,-1,-1, 0, 1, 1, 1 };
	static int initCode[8] = { 7, 7, 1, 1, 3, 3, 5, 5 };
	int dADD[8]; //��ַƫ����
	BYTE *pBegin, *pCur, *pTst; //�������,��ǰ��,����
	int code, beginCode, returnCode, i;
	int N;

	// step.1-----��ʼ��----------------------------------------//
	//��ͬ�����Ӧ�ĵ�ַƫ����
	for(code = 0;code<8;code++) dADD[code]=dy[code]*width+dx[code];
	pBegin = pBinImg+y0*width+x0; //�������ĵ�ַ
	pCur = pBegin; //��ǰ�����õ��������
	if (isOuter) //������ʱ�ĳ�ʼ��
	{
		*(pCur-1) = 1;  //����Ǳ�����,���Ϊ�Ҷ�ֵ1
		code = 7; //��ʼ��Ϊ7
	}
	else //������ʱ�ĳ�ʼ��
	{
		*(pCur+1) = 1; //�Ҳ��Ǳ�����,���Ϊ�Ҷ�ֵ1
		code = 3; //��ʼ��Ϊ3
	}
	beginCode = initCode[code]; //�������ĵ�һ������
 	// step.2-----��������--------------------------------------//
	N = 0; //���������ʼ��Ϊ0
	do {
		*pCur = 254; //��������,���Ϊ�Ҷ�ֵ254
		for (i = 0, code = initCode[code]; i<7; i++, code = (code+1)%8)
		{
			pTst = pCur+dADD[code]; //�õ�Ҫ����������ĵ�ַ
			if (*pTst<2) *pTst = 1; //�Ǳ�����,���Ϊ�Ҷ�ֵ1
			else //��������
			{
				if (N<maxChainCodeNum) pChainCode[N++] = code; //��������
				if (pTst==pBegin) //�ص����Ĵ���
				{
					//�ҳ�ʣ���������ʼ���
					returnCode=(code+4)%8; //ת��Ϊ�ڴ��������Ѿ�����������
					for(i=0,code=beginCode;i<7;i++,code=(code+1)%8)
					{
						if (code==returnCode)
						{
							i++; //ʣ���������ʼ���
							code = (code+1)%8; //ʣ���������ʼֵ
							break;
						}
					}
					//���ʣ������
					for (; i<7; i++, code = (code+1)%8)
					{
						pTst = pBegin+dADD[code];
						if (*pTst<2) *pTst = 1; //�Ǳ�����,���Ϊ�Ҷ�ֵ1
						else
						{   //��������
							if (N<maxChainCodeNum) pChainCode[N++] = code;
							break; //����㿪ʼ,�ҵ����µ�������pTst
						}
					}
				}
				break; //�ҵ����µ�������pTst
			}
		}
		pCur = pTst; //��ǰ���ƶ����µ�������pTst
	} while (i<7); //�ҵ�������ʱһ����i<7
	// step.3-----����-----------------------------------------//
	return N; //�����������
}