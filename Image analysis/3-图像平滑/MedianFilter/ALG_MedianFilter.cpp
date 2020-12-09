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
//��ֵ�˲�
//
///////////////////////////////////////////////////////////////////////////////////////
void GetMedianGry(int *histogram, int N, int *medGry)
{
	int g;
	int num;

	// step.1-------------��Ҷ���ֵ------------------------//
	num = 0;
	for (g = 0; g<256; g++)
	{
		num += histogram[g];
		if (2*num>N) break;  //num>N/2
	}
	*medGry = g;
	// step.2-------------����------------------------------//
	return;
}

double RmwMedianFilter( BYTE *pGryImg, int width, int height,
	                    int M, int N, //�˲�����M��N��
	                    BYTE *pResImg
                      )
{   //��ÿ�н�һ��ֱ��ͼ,û�н���������ֱ��ͼ���ƺ�ͼ��߽�䴰�ڵ��Ż�����
	BYTE *pCur, *pRes;
	int halfx,halfy,x, y, i, j, y1, y2;
	int histogram[256];
	int wSize, j1, j2;
	int num, med, v;
	int dbgCmpTimes = 0; //������ֵ����Ƚϴ����ĵ���

	M = M/2*2+1; //������
	N = N/2*2+1; //������
	halfx = M/2; //x�뾶
	halfy = N/2; //y�뾶
	wSize = (halfx*2+1)*(halfy*2+1); //�����������ܸ���
	for (y = halfy, pRes = pResImg+y*width; y<height-halfy; y++)
	{
		//step.1----��ʼ��ֱ��ͼ
		y1 = y-halfy;
		y2 = y+halfy;
		memset(histogram, 0, sizeof(int)*256);
		for (i = y1, pCur = pGryImg+i*width; i<=y2; i++, pCur += width)
		{
			for (j = 0; j<halfx*2+1; j++)
			{
				histogram[*(pCur+j)]++;
			}
		}
		//step.2-----��ʼ����ֵ
		num = 0; //��¼�ŻҶ�ֵ��0����ֵ�ĸ���
		for (i = 0; i<256; i++)
		{
			num += histogram[i];
			if (num*2>wSize)
			{
				med = i;
				break;
			}
		}
		//�˲�
		pRes += halfx; //û�д���ͼ����߽�������
		for (x = halfx; x<width-halfx; x++)
		{
			//��ֵ
			*(pRes++) = med;
			//step.3-----ֱ��ͼ����: ��ȥ��ǰ��������ߵ�һ��,��������Ҳ��һ������
			j1 = x-halfx; //�������
			j2 = x+halfx+1; //�ұߵ�����
			for (i = y1, pCur = pGryImg+i*width; i<=y2; i++, pCur += width)
			{
				//��ȥ�������
				v = *(pCur+j1);
				histogram[v]--;  //����ֱ��ͼ
				if (v<=med) num--; //����num
				//����ұߵ�����
				v = *(pCur+j2);
				histogram[v]++; //����ֱ��ͼ
				if (v<=med) num++; //����num
			}
			//step.4-----������ֵ
			if (num*2<wSize) //���ϴ���ֵmed�ĸ���������,��medҪ���
			{
				for (med = med+1; med<256; med++)
				{
					dbgCmpTimes += 2; //�ܵıȽϴ���,������
					num += histogram[med];
					if (num*2>wSize) break;
				}
				dbgCmpTimes += 1; //�ܵıȽϴ���,������
			}
			else //���ϴ���ֵmed�ĸ�������,��medҪ��С
			{
				while ((num-histogram[med])*2>wSize)//����ȥ��,�Ա�С
				{
					dbgCmpTimes++; //�ܵıȽϴ���,������
					num -= histogram[med];
					med--;
				}
				dbgCmpTimes += 2; //�ܵıȽϴ���,������
			}
		}
		pRes += halfx;//û�д���ͼ���ұ߽�������
	}
	//����������ֵ��Ҫ��ƽ���Ƚϴ���
	return dbgCmpTimes*1.0/((width-halfx*2)*(height-halfy*2));
}