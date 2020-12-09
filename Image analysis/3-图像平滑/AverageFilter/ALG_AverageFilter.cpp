///////////////////////////////////////////////////////////////////////////////////////////
//
//   ������
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
//�����к͵Ŀ��پ�ֵ�˲�
//
///////////////////////////////////////////////////////////////////////////////////////
void RmwAvrFilterBySumCol( BYTE *pGryImg,  //ԭʼ�Ҷ�ͼ��
	                       int width, int height,//ͼ��Ŀ�Ⱥ͸߶�
	                       int M, int N, //�˲�����M��N��
	                       BYTE *pResImg //���ͼ��
                         )
{   //û�жԱ߽����������������ؽ��д������Բ��ñ䴰�ڵĲ���
	BYTE *pAdd, *pDel, *pRes;
	int halfx, halfy;
	int x, y;
	int sum,c;
	int sumCol[4096]; //Լ��ͼ���Ȳ�����4096

	// step.1------------��ʼ��--------------------------//
	M = M/2*2+1; //������
	N= N/2*2+1; //������
	halfx = M/2; //�˲����İ뾶x
	halfy = N/2; //�˲����İ뾶y
	c = (1<<23)/(M*N); //�˷�����
	memset(sumCol, 0, sizeof(int)*width);
	for (y = 0, pAdd = pGryImg; y<N; y++)
	{
		for (x = 0; x<width; x++) sumCol[x] += *(pAdd++);
	}
	// step.2------------�˲�----------------------------//
	for (y = halfy, pRes = pResImg+y*width,pDel=pGryImg; y<height-halfy; y++)
	{
		//��ֵ
		for (sum=0,x = 0; x<M; x++) sum += sumCol[x];
		//�˲�
		pRes += halfx; //�������
		for (x = halfx; x<width-halfx; x++)
		{
			//��ҶȾ�ֵ
			//*(pRes++)=sum/(N*M);
			*(pRes++) = (sum*c)>>23; //�������˷�����λ�������
			//����,���»ҶȺ�
			sum -= sumCol[x-halfx]; //�������
			sum += sumCol[x+halfx+1]; //���ұ���
		}
		pRes += halfx;//�����Ҳ�
		//����,����sumCol
		for (x = 0; x<width; x++)
		{
			sumCol[x] -= *(pDel++); //����һ��
			sumCol[x] += *(pAdd++); //����һ��
		}
	}
	// step.3------------����----------------------------//
	return;
}
///////////////////////////////////////////////////////////////////////////////////////
//
//�����к͵Ļ���ͼʵ��
//
///////////////////////////////////////////////////////////////////////////////////////
void RmwDoSumGryImg( BYTE *pGryImg,  //ԭʼ�Ҷ�ͼ��
	                 int width, //ͼ��Ŀ�� 
	                 int height,//ͼ��ĸ߶�
	                 int *pSumImg //����õ��Ļ���ͼ
                   )
{
	BYTE *pGry;
	int *pRes;
	int x, y;
	int sumCol[4096]; //Լ��ͼ���Ȳ�����4096

	memset(sumCol, 0, sizeof(int)*width);
	for (y = 0, pGry = pGryImg, pRes=pSumImg; y<height; y++)
	{
		//��������ص��ر���
		sumCol[0] += *(pGry++);
		*(pRes++) = sumCol[0];
		//��������
		for (x = 1; x<width; x++)
		{
			sumCol[x] += *(pGry++); //�����к�
			*(pRes++) = *(pRes-1)+sumCol[x];
		}
	}
	return;
}
///////////////////////////////////////////////////////////////////////////////////////
//
//����SSE�Ļ���ͼʵ��
//
///////////////////////////////////////////////////////////////////////////////////////
void RmwDoSumGryImg_SSE( BYTE *pGryImg,  //ԭʼ�Ҷ�ͼ��
	                     int width, //ͼ��Ŀ��,������4�ı���
	                     int height,//ͼ��ĸ߶�
	                     int *pSumImg //����õ��Ļ���ͼ
                       )
{   _declspec(align(16)) int sumCol[4096]; //Լ��ͼ���Ȳ�����4096,16�ֽڶ���
	__m128i *pSumSSE,A;
	BYTE *pGry;
	int *pRes;
	int x, y;

	memset(sumCol, 0, sizeof(int)*width);
	for (y = 0, pGry = pGryImg, pRes=pSumImg; y<height; y++)
	{
		//0:��Ҫ�ر���
		sumCol[0] += *(pGry++);
		*(pRes++) = sumCol[0];
		//1
		sumCol[1] += *(pGry++);
		*(pRes++) = *(pRes-1)+sumCol[1];
		//2
		sumCol[2] += *(pGry++);
		*(pRes++) = *(pRes-1)+sumCol[2];
		//3
		sumCol[3] += *(pGry++);
		*(pRes++) = *(pRes-1)+sumCol[3];
		//[4...width-1]
		for (x = 4, pSumSSE = (__m128i *)(sumCol+4); x<width; x+=4, pGry+=4)
		{
			//�ѱ����ĵ�32λ(��4��8λ�������)ת����32λ������
			A = _mm_cvtepu8_epi32(_mm_loadl_epi64((__m128i*)pGry));
			//4��32λ��������� 
			*(pSumSSE++) = _mm_add_epi32(*pSumSSE, A);
			//����
			*(pRes++) = *(pRes-1)+sumCol[x+0];
			*(pRes++) = *(pRes-1)+sumCol[x+1];
			*(pRes++) = *(pRes-1)+sumCol[x+2];
			*(pRes++) = *(pRes-1)+sumCol[x+3];
		}
	}
	return;
}
///////////////////////////////////////////////////////////////////////////////////////
//
//���ڻ���ͼ�Ŀ��پ�ֵ�˲�
//
///////////////////////////////////////////////////////////////////////////////////////
void RmwAvrFilterBySumImg( int *pSumImg, //����õ��Ļ���ͼ
	                       int width, int height,//ͼ��Ŀ�Ⱥ͸߶�
	                       int M, int N, //�˲�����M��N��
	                       BYTE *pResImg //���ͼ��
                         )
{   //û�жԱ߽����������������ؽ��д������Բ��ñ䴰�ڵĲ���
	int *pY1, *pY2;
	BYTE *pRes;
	int halfx, halfy;
	int y, x1, x2;
	int sum, c;

	// step.1------------��ʼ��--------------------------//
	M = M/2*2+1; //������
	N = N/2*2+1; //������
	halfx = M/2; //�˲����İ뾶x
	halfy = N/2; //�˲����İ뾶y
	c = (1<<23)/(M*N); //�˷�����
 	// step.2------------�˲�----------------------------//
	for ( y = halfy+1, pRes = pResImg+y*width, pY1 = pSumImg,pY2=pSumImg+N*width; 
		  y<height-halfy; 
		  y++,pY1+=width,pY2+=width
		)
	{
 		pRes += halfx+1; //�������
		//for (x = halfx+1,x1=0,x2=M; x<width-halfx; x++,x1++,x2++)
		for (x1=0,x2=M; x2<width; x1++,x2++) //���Լ���ˣ�����̫���׶�
		{
			sum = *(pY2+x2)-*(pY2+x1)-*(pY1+x2)+*(pY1+x1);
			*(pRes++) = (sum*c)>>23; //�������˷�����λ�������
 		}
		pRes += halfx;//�����Ҳ�
	}
	// step.3------------����----------------------------//
	return;
}
