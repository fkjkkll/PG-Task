///////////////////////////////////////////////////////////////////////////////////////////
//
//   �ڶ���
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
//ֱ��ͼ�������
//
///////////////////////////////////////////////////////////////////////////////////////
void RmwGetHistogram(BYTE *pGryImg, int width, int height, int *histogram)
{
	BYTE *pCur, *pEnd = pGryImg+width*height;

	// step.1-------------��ʼ��----------------------------//
	//for (int g = 0; g<256; g++) histogram[g] = 0;
	memset(histogram, 0, sizeof(int)*256);
	// step.2-------------ֱ��ͼͳ��------------------------//
	for (pCur = pGryImg; pCur<pEnd; ) histogram[*(pCur++)]++;
	// step.3-------------����------------------------------//
	return;
}

void GetMinMaxGry(int *histogram, int *minGry, int *maxGry)
{
	int g;

	// step.1-------------����Сֵ--------------------------//
	for (g = 0; g<256; g++)
	{
		if (histogram[g]) break;
	}
	*minGry = g;
	// step.2-------------�����ֵ--------------------------//
	for (g = 255; g>=0; g--)
	{
		if (histogram[g]) break;
	}
	*maxGry = g;
	// step.3-------------����------------------------------//
	return;
}

void GetBrightContrast(int *histogram, double *bright, double *contrast)
{
	int g;
	int sum, num;
	double fsum;

	// step.1-------------������----------------------------//
	for (sum = num = 0, g = 0; g<256; g++)
	{
		sum += histogram[g]*g;
		num += histogram[g];
	}
	*bright = sum*1.0/num;
	// step.2-------------��Աȶ�--------------------------//
	for (fsum = 0.0, g = 0; g<256; g++)
	{
		fsum += histogram[g]*(g-*bright)*(g-*bright);
	}
	*contrast = sqrt(fsum/(num-1));
	// step.3-------------����------------------------------//
	return;
}
///////////////////////////////////////////////////////////////////////////////////////
//
//ֱ��ͼ���⻯
//
///////////////////////////////////////////////////////////////////////////////////////
void RmwHistogramEqualize1(BYTE *pGryImg, int width, int height)
{
	BYTE *pCur, *pEnd = pGryImg+width*height;
	int histogram[256], A[256], LUT[256], g;

	// step.1-------------��ֱ��ͼ--------------------------//
	memset(histogram, 0, sizeof(int)*256);
	for (pCur = pGryImg; pCur<pEnd;) histogram[*(pCur++)]++;
	// step.2-------------��A[g],N-------------------------//
	for (g = 1, A[0] = histogram[0]; g<256; g++)
	{
		A[g] = A[g-1]+histogram[g];
	}
	// step.3-------------��LUT[g]-------------------------//
	for (g = 0; g<256; g++) LUT[g] = 255*A[g]/(width*height);
	// step.4-------------���------------------------------//
	for (pCur = pGryImg; pCur<pEnd;) *(pCur++) = LUT[*pCur];
	// step.5-------------����------------------------------//
	return;
}

void RmwHistogramEqualize(BYTE *pGryImg, int width, int height)
{
	BYTE *pCur, *pEnd = pGryImg+width*height;
	int histogram[256], LUT[256], A, g;

	// step.1-------------��ֱ��ͼ--------------------------//
	memset(histogram, 0, sizeof(int)*256);
	for (pCur = pGryImg; pCur<pEnd;) histogram[*(pCur++)]++;
	// step.2-------------��LUT[g]-------------------------//
	A = histogram[0];
	LUT[0] = 255*A/(width*height);
	for (g = 1; g<256; g++)
	{
		A += histogram[g];
		LUT[g] = 255*A/(width*height);
	}
	// step.3-------------���------------------------------//
	for (pCur = pGryImg; pCur<pEnd;) *(pCur++) = LUT[*pCur];
	// step.4-------------����------------------------------//
	return;
}
///////////////////////////////////////////////////////////////////////////////////////
//
//�����任
//
///////////////////////////////////////////////////////////////////////////////////////
void RmwLogTransform(BYTE *pGryImg, int width, int height)
{
	BYTE *pCur, *pEnd = pGryImg+width*height;
	int histogram[256], LUT[256], gmax, g;
	double c;

	// step.1-------------��ֱ��ͼ--------------------------//
	memset(histogram, 0, sizeof(int)*256);
	for (pCur = pGryImg; pCur<pEnd;) histogram[*(pCur++)]++;
	// step.2-------------���ֵ---------------------------//
	for (gmax = 255; gmax>=0; gmax++) if (histogram[gmax]) break;
	// step.3-------------��LUT[g]-------------------------//
	c = 255.0/log(1+gmax);
	for (g = 0; g<256; g++)
	{
		LUT[g] = (int)(c*log(1+g));
	}
	// step.4-------------���------------------------------//
	for (pCur = pGryImg; pCur<pEnd;) *(pCur++) = LUT[*pCur];
	// step.5-------------����------------------------------//
	return;
}
///////////////////////////////////////////////////////////////////////////////////////
//
//���ֱ��ͼ���⻯
//
///////////////////////////////////////////////////////////////////////////////////////
void RmwHistogramEqualizePixel(BYTE *pGryImg, int width, int height, int N, int M, BYTE *pResImg)
{   //���ֱ��ͼ���⻯
	//�ó���û�п����Ż�,ֻ������ʵ����
	BYTE *pCur, *pEnd = pGryImg+width*height;
	BYTE *pRes;
	int x, y, sum;
	int halfN, halfM;
	int i1, i2, j1, j2, i, j;

	N = N/2*2+1;  halfN = N/2;
	M = M/2*2+1;  halfM = M/2;
	for (y = 0, pCur = pGryImg, pRes = pResImg; y<height; y++)
	{
		printf("%d\n", y);
		//���㴰��������
		i1 = y-halfN;
		if (i1<0)
		{
			i1 = 0;
			i2 = i1+N-1;
		}
		else
		{
			i2 = y+halfN;
			if (i2>height-1)
			{
				i2 = height-1;
				i1 = i2-N+1;
			}
		}
		//���з��� 
		for (x = 0; x<width; x++, pCur++, pRes++)
		{
			//���㴰�ں�����
			j1 = x-halfM;
			if (j1<0)
			{
				j1 = 0;
				j2 = j1+M-1;
			}
			else
			{
				j2 = x+halfM;
				if (j2>width-1)
				{
					j2 = width-1;
					j1 = j2-M+1;
				}
			}
			//��������sum
			sum = 0;
			for (i = i1; i<=i2; i++)
			{
				for (j = j1; j<=j2; j++)
				{
					if (*(pGryImg+i*width+j)<=*pCur)
					{
						sum++;
					}
				}
			}
			//��ֵ,�����ι�һ��
			*pRes = 255*sum/(N*M);
		}
	}
	return;
}
///////////////////////////////////////////////////////////////////////////////////////
//
//14Bitֱ��ͼ���⻯
//
///////////////////////////////////////////////////////////////////////////////////////
int RmwRead14BitImgFile(short int *p14Img, int width, int height, char *fileName)
{
	FILE *fp;

	fp = fopen(fileName, "rb");
	if (!fp) return 1;
	fread(p14Img, sizeof(short int), width*height, fp);
	fclose(fp);
	return 0;
}

void RmwHistogramEqualize(short int *pGry14BitImg, int width, int height, BYTE *pResImg)
{
	int histogram[1<<14];
	int LUT[1<<14];
	int A, i, g;

	// step.1-------------ͳ��ֱ��ͼ------------------------//
	memset(histogram, 0, sizeof(int)*(1<<14));
	for (i = 0; i<width*height; i++) histogram[*(pGry14BitImg+i)]++;
	// step.2-------------ֱ��ͼ���⻯----------------------//
	A = histogram[0];
	LUT[0] = 255*A/(width*height);
	for (g = 1; g<(1<<14); g++)
	{
		A += histogram[g];
		LUT[g] = 255*A/(width*height);
	}
	for (i = 0; i<width*height; i++)
	{
		pResImg[i] = LUT[*(pGry14BitImg+i)];
	}
	// step.3-------------����-----------------------------//
	return;
}
