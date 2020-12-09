///////////////////////////////////////////////////////////////////////////////////////////
//
//������-�������
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
//��������㷨
//
///////////////////////////////////////////////////////////////////////////////////////
void RmwFillContour( BYTE *pGryImg, int width, int height,//�����ͼ��
	                 int x0, int y0, //�������
	                 bool isOuter, //�Ƿ���������
	                 BYTE *pCode, //����
	                 int N, //�������
	                 int regionC, //�����ڲ�����ɫ
	                 int contourC, //��������ɫ
	                 int nonC //�����ڵ���ɫ
                   )
{
	static int dx[8] = { 1, 1, 0,-1,-1,-1, 0, 1 };
	static int dy[8] = { 0,-1,-1,-1, 0, 1, 1, 1 };
	int dADD[8]; //��ַƫ����
	BYTE *pBegin, *pCur,*pTst; //�������,��ǰ��,����
	int inCode, outCode, i;
	int sumC, sumDy, direction;

	// step.1-----��ʼ��----------------------------------------//
	pBegin = pGryImg+y0*width+x0; //�������ĵ�ַ
	//��ͬ�����Ӧ�ĵ�ַƫ����
	for (i = 0; i<8; i++) dADD[i] = dy[i]*width+dx[i];
	// step.2-----���������������������仯��֮�͸���ֵ----------//
	for (sumC = 2; sumC<=253; sumC++) //��sumC
	{
		if ((regionC>=sumC-2)&&(regionC<=sumC+2)) continue;
		if ((nonC>=sumC-2)&&(nonC<=sumC+2)) continue;
		break;
	}
	for(i=0,pCur=pBegin;i<N;pCur+=dADD[pCode[i]],i++) *pCur = sumC;
	// step.3-----�������������������������仯��֮��------------//
	inCode = pCode[N-1]; //����������������
	for (i = 0, pCur = pBegin; i<N; i++)
	{
		outCode = pCode[i]; //�Ӹ����������������
		*pCur += dy[inCode]+dy[outCode]; //���صĻҶ�ֵ���Ǳ仯��֮�� 
		inCode = outCode; //��ǰ������ĳ����������һ��������Ľ�����
		pCur += dADD[outCode]; //ָ����һ��������ĵ�ַ
	}
	// step.4-----�������������յ���б��--------------------//
	for (i = 0, pCur = pBegin; i<N; pCur += dADD[pCode[i]], i++)
	{
		sumDy = *pCur;
		if ((sumDy==sumC+1)||(sumDy==sumC+2)) *pCur = regionC; //���Ϊ������
		else if ((sumDy==sumC-1)||(sumDy==sumC-2)) *pCur = nonC; //���Ϊ����յ�
	}
	// step.5-----�����������������յ�֮��������------------//
	direction = isOuter ? 1 : -1; //�������Ǵ�������,������+1;��������֮
	for (i = 0, pCur = pBegin; i<N; pCur += dADD[pCode[i]], i++)
	{
		if (*pCur==regionC) //�ҵ�һ��������
		{
			pTst = pCur;
			while (*pTst!=nonC) //һֱ��䵽�յ�
			{
				*pTst = regionC;
				pTst += direction;
			}
			*pCur = nonC; //��ˮƽ���Ѿ�������,�����ظ����
		}
	}
	// step.6-----�����������ɫ���и�ֵ-------------------------//
	for(i=0,pCur=pBegin;i<N;pCur+=dADD[pCode[i]],i++) *pCur = contourC;
	// step.7-----����-----------------------------------------//
	return;
}