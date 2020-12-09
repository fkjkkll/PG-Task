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
//��ֵͼ���˲�
//
///////////////////////////////////////////////////////////////////////////////////////
void RmwBinImgFilter( BYTE *pBinImg,  //ԭʼ��ֵͼ��
	                  int width, int height,//ͼ��Ŀ�Ⱥ͸߶�
	                  int M, int N, //�˲�����M��N��
	                  int threshold, //�Ҷ���ֵ,���ڵ��ڸ�ֵʱ�����255
	                  BYTE *pResImg //���ͼ��
                    )
{   //û�жԱ߽����������������ؽ��д������Բ��ñ䴰�ڵĲ���
	BYTE *pAdd, *pDel, *pRes;
	int halfx, halfy;
	int x, y, sum;
	int sumCol[4096]; //Լ��ͼ���Ȳ�����4096

	// step.1------------��ʼ��--------------------------//
	M = M/2*2+1; //������
	N= N/2*2+1; //������
	halfx = M/2; //�˲�����x�뾶
	halfy = N/2; //�˲�����y�뾶
	threshold = threshold *M*N; //ת���������ڻҶ�ֵ֮�͵���ֵ
	memset(sumCol, 0, sizeof(int)*width);
	for (y = 0, pAdd = pBinImg; y<N; y++)
	{
		for (x = 0; x<width; x++) sumCol[x] += *(pAdd++);
	}
	// step.2------------�˲�----------------------------//
	for (y = halfy, pRes = pResImg+y*width,pDel= pBinImg; y<height-halfy; y++)
	{
		//��ֵ
		for (sum=0,x = 0; x<M; x++) sum += sumCol[x];
		//�˲�
		pRes += halfx; //�������
		for (x = halfx; x<width-halfx; x++)
		{
			//��ҶȾ�ֵ
			*(pRes++) = (sum>=threshold)*255; //�����������ʽ�ĺ���
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