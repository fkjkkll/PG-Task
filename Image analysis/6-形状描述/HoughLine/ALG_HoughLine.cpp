///////////////////////////////////////////////////////////////////////////////////////////
//
//������-ֱ�߻���任
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

///////////////////////////////////////////////////////////////////////////////////////////
//
//��ֱ��
//
///////////////////////////////////////////////////////////////////////////////////////////
void RmwDrawKBLine(BYTE *pGryImg, int width, int height, bool isYKX, double K, double B, int color)
{
	int x, y;

	if (isYKX) // y=kx+b
	{
		for (x = 0; x<width; x++)
		{
			y = (int)(K*x+B+0.5);
			if ((y>=0)&&(y<height))
			{
				*(pGryImg+y*width+x) = color;
			}
		}
	}
	else //x=ky+b
	{
		for (y = 0; y<height; y++)
		{
			x = (int)(K*y+B+0.5);
			if ((x>=0)&&(x<width))
			{
				*(pGryImg+y*width+x) = color;
			}
		}
	}
	return;
}
void RmwDrawABCLine( BYTE *pGryImg, int width, int height,
	                 double A, double B, double C,
	                 int color
                   )
{
	int x, y;
 
	if (fabs(B)>fabs(A))
	{
		for (x = 0; x<width; x++)
		{
			y = (int)(-(C+A*x)/B+0.5);
			if ((y>=0)&&(y<height))  *(pGryImg+y*width+x) = color;
		}
	}
	else
	{
		for (y = 0; y<height; y++)
		{
			x = (int)(-(C+B*y)/A+0.5);
			if ((x>=0)&&(x<width))  *(pGryImg+y*width+x) = color;
		}
	}
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////
//
//ֱ�����
//
///////////////////////////////////////////////////////////////////////////////////////////
bool RmwFittingLine(int *x, int *y, int N, double *k, double *b)
{
	bool isYKX;
	double sumx, sumy, sumxy, avrx, avry;
	int i;

	// step.1-------------���������ֵ----------------------//
	sumx = sumy = 0.0;
	for (i = 0; i<N; i++)
	{
		sumx += x[i];
		sumy += y[i];
	}
	avrx = sumx/N;
	avry = sumy/N;
	// step.2-------------�������귽��----------------------//
	sumx = sumy = sumxy = 0;
	for (i = 0; i<N; i++)
	{
		sumx += (x[i]-avrx)*(x[i]-avrx);
		sumy += (y[i]-avry)*(y[i]-avry);
		sumxy += (x[i]-avrx)*(y[i]-avry);
	}
	// step.3-------------�ж�ֱ�����ͺ���kb----------------//
	if (sumx>sumy) //y=kx+b
	{
		isYKX = true;
		*k = sumxy/sumx;
		*b = avry-(*k)*avrx;
	}
	else //x=ky+b
	{
		isYKX = false;
		*k = sumxy/sumy;
		*b = avrx-(*k)*avry;
	}
	// step.4-------------����ֱ������----------------------//
	return isYKX;
}
void RmwFittingLine(int *x, int *y, int N, double *A, double *B, double *C)
{
	bool isYKX;
	double k, b;

	// step.1-------------ִ��б��ʽֱ�����-----------------//
	isYKX = RmwFittingLine(x, y, N, &k, &b);
	// step.2-------------��б��ʽת��Ϊһ��ʽ---------------//
	if (isYKX) //y=kx+b => kx-y+b=0
	{
		*A = k;
		*B = -1;
		*C = b;
	}
	else //x=ky+b => x-ky-b=0
	{
		*A = 1;
		*B = -k;
		*C = -b;
	}
	// step.3-------------����-----------------------------//
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////
//
//ֱ��HT�任
//
///////////////////////////////////////////////////////////////////////////////////////////
int RmwHoughLine( int width,int height,
	              int *x, int *y, int N,
	              double *A, double *B, double *C
                )
{
	int maxCount, bstTheta, bstThro;
	int *pCount,*pCenter,*pCur;
	int maxThro, cosV, sinV;
	int theta, thro, i;

	// step.1-------------����������ռ�---------------------//
	maxThro = (int)sqrt(1.0*width*width+height*height+0.5)+1;
	pCount = new int[(maxThro*2)*180]; //(-maxThro,maxThro)
	if (!pCount) return 0; //����pCount�ڸú���������
	// step.2-------------����任---------------------------//
	memset(pCount, 0, sizeof(int)*(maxThro*2)*180);
	for (theta = 0; theta<180; theta++) //����Ϊ1��
	{
		cosV = (int)(cos(theta*3.1415926/180)*8192); //�Ŵ�8192��
		sinV = (int)(sin(theta*3.1415926/180)*8192);
		pCenter = pCount+(maxThro*2)*theta+maxThro; //����ƫ��maxThro
		for (i = 0; i<N; i++)
		{
			thro = ((x[i]*cosV+y[i]*sinV)>>13); //��С8192��,thro�Ĳ���Ϊ1
 			*(pCenter+thro) += 1;
		}
	}
	// step.3-------------���ֵ����-------------------------//
	maxCount = bstTheta = bstThro = 0;
	for (theta = 0,pCur = pCount; theta<180; theta++)
	{
		for (thro = 0; thro<maxThro*2; thro++, pCur++)
		{
			if (*pCur>maxCount)
			{
				maxCount = *pCur;
				bstTheta = theta;
				bstThro = thro;
			}
		}
	}
	bstThro -= maxThro; //ȥ��ƫ��maxThro
	// step.4-------------��ֱ��Ax+By+C��ֵ-----------------//
	//x*cos(bstTheta)+y*sin(bstTheta)=bstThro => Ax+By+C=0
	*A = cos(bstTheta*3.1415926/180);
	*B = sin(bstTheta*3.1415926/180);
	*C = -bstThro;
	// step.5-------------���ؾ����ĵ���---------------------//
 	delete pCount; //�ͷ��Լ�������ڴ�,�ú����ڶ�̬������ͷŻ�����ڴ���Ƭ
	return maxCount;
}
