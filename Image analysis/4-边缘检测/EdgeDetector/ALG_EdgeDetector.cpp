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

///////////////////////////////////////////////////////////////////////////////////////////
//
//Gradient����
//
///////////////////////////////////////////////////////////////////////////////////////////
void RmwGradientGryImg(BYTE *pGryImg, int width, int height, BYTE *pGrdImg)
{
	BYTE *pGry, *pGrd;
	int dx, dy;
	int x, y;

	for (y = 0, pGry = pGryImg, pGrd = pGrdImg; y<height-1; y++)
	{
		for (x = 0; x<width-1; x++, pGry++)
		{
			dx = *pGry-*(pGry+1);
			dy = *pGry-*(pGry+width);
			*(pGrd++) = min(255, (int)(sqrt(dx*dx*1.0+dy*dy)));
		}
		*(pGrd++) = 0; //β�в���,��Եǿ�ȸ�0
		pGry++;
	}
	memset(pGrd, 0, width); //β�в���,��Եǿ�ȸ�0
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////
//
//Robert����
//
///////////////////////////////////////////////////////////////////////////////////////////
void RmwRobertsGryImg(BYTE *pGryImg, int width, int height, BYTE *pRbtImg)
{
	BYTE *pGry, *pRbt;
	int dx, dy;
	int x, y;

	for (y = 0, pGry = pGryImg, pRbt = pRbtImg; y<height-1; y++)
	{
		for (x = 0; x<width-1; x++, pGry++)
		{
			dx = *pGry-*(pGry+width+1);
			dy = *(pGry+1)-*(pGry+width);
			*(pRbt++) = max(abs(dx), abs(dy));
		}
		*(pRbt++) = 0; //β�в���,��Եǿ�ȸ�0
		pGry++;
	}
	memset(pRbt, 0, width); //β�в���,��Եǿ�ȸ�0
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////
//
//Sobel����
//
///////////////////////////////////////////////////////////////////////////////////////////
void RmwSobelGryImg(BYTE *pGryImg, int width, int height, BYTE *pSbImg)
{
	BYTE *pGry, *pSb;
	int dx, dy;
	int x, y;

	memset(pSbImg, 0, width); //���в���,��Եǿ�ȸ�0
	for (y = 1, pGry = pGryImg+width, pSb = pSbImg+width; y<height-1; y++)
	{
		*(pSb++) = 0;  //���в���,��Եǿ�ȸ�0
		pGry++;
		for (x = 1; x<width-1; x++,pGry++)
		{
			//��dx
			dx = *(pGry-1-width)+(*(pGry-1)*2)+*(pGry-1+width);
			dx -= *(pGry+1-width)+(*(pGry+1)*2)+*(pGry+1+width);
			//��dy
			dy = *(pGry-width-1)+(*(pGry-width)*2)+*(pGry-width+1);
			dy -= *(pGry+width-1)+(*(pGry+width)*2)+*(pGry+width+1);
			//���
			*(pSb++) = min(255, abs(dx)+abs(dy));
		}
		*(pSb++) = 0; //β�в���,��Եǿ�ȸ�0
		pGry++;
	}
	memset(pSb, 0, width); //β�в���,��Եǿ�ȸ�0
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////
//
//Prewitt����
//
///////////////////////////////////////////////////////////////////////////////////////////
void RmwPrewittGryImg(BYTE *pGryImg, int width, int height, BYTE *pPRTImg)
{
	BYTE *pGry, *pPRT;
	int dx, dy,d45,d135,v1,v2;
	int x, y;

	memset(pPRTImg, 0, width); //���в���,��Եǿ�ȸ�0
	for (y = 1, pGry = pGryImg+width, pPRT = pPRTImg+width; y<height-1; y++)
	{
		*(pPRT++) = 0;  //���в���,��Եǿ�ȸ�0
		pGry++;
		for (x = 1; x<width-1; x++, pGry++)
		{
			//��dx
			dx = *(pGry-1-width)+*(pGry-1)+*(pGry-1+width);
			dx -= *(pGry+1-width)+*(pGry+1)+*(pGry+1+width);
			//��dy
			dy = *(pGry-width-1)+*(pGry-width)+*(pGry-width+1);
			dy -= *(pGry+width-1)+*(pGry+width)+*(pGry+width+1);
			//��45��
			d45 = *(pGry-width-1)+*(pGry-width)+*(pGry-1);
			d45 -= *(pGry+width+1)+*(pGry+width)+*(pGry+1);
			//��135��
			d135 = *(pGry-width)+*(pGry-width+1)+*(pGry+1);
			d135 -= *(pGry+width-1)+*(pGry+width)+*(pGry-1);
			//���
			v1 = max(abs(dx), abs(dy));
			v2 = max(abs(d45), abs(d135));
			*(pPRT++) = min(255, max(v1, v2));
		}
		*(pPRT++) = 0; //β�в���,��Եǿ�ȸ�0
		pGry++;
	}
	memset(pPRT, 0, width); //β�в���,��Եǿ�ȸ�0
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////
//
//������
//
///////////////////////////////////////////////////////////////////////////////////////////
void RmwShenJunGryImg( BYTE *pGryImg, //ԭʼ�Ҷ�ͼ�� 
	                   BYTE *pTmpImg, //����ͼ��
	                   int width,int height,
	                   double a0, 
	                   BYTE *pSJImg
                     )
{ 	
	BYTE *pGry,*pCur,*pSJ,*pEnd;
	int LUT[512], *ALUT; //a0���ұ�
	int x,y,pre,dif;

	// step.1------------��ʼ�����ұ�---------------------//
	a0=min(max(0.01,a0),0.99); //��ȫ�Լ��
	//a0���ұ�,��������������
	ALUT=LUT+256;
	for(ALUT[0]=0,dif=1;dif<256;dif++)
	{
		ALUT[dif]=(int)(dif*a0+0.5);
		ALUT[-dif]=(int)(-dif*a0-0.5);
	}
	// step.2------------����ʵ��ָ���˲�-----------------//
	//�����˲�
	for (y = 0, pGry = pGryImg, pCur = pTmpImg; y<height; y++)
	{
		//1.��������: p1(y,x)=p1(y,x-1)+a*[p(y,x)-p1(y,x-1)]
		*(pCur++) = pre = *(pGry++);
		for (x = 1; x<width; x++, pGry++) *(pCur++) = pre = pre+ALUT[*pGry-pre];
		pCur--; //�ص���β
		//2.��������: p2(y,x)=p2(y,x+1)-a*[p1(y,x)-p2(y,x+1)]
		for (x = width-2, pCur = pCur-1; x>=0; x--) *(pCur--) = pre = pre+ALUT[*pCur-pre];
		pCur += (width+1); //�ص���һ�еĿ�ʼ
	}
	//�����˲�
	for (x = 0, pCur = pTmpImg; x<width; x++, pCur = pTmpImg+x)
	{
		//3.��������: p3(y,x)=p3(y-1,x)+a*[p2(y,x)-p3(y-1,x)]
		pre = *pCur;
		for (y = 1, pCur += width; y<height; y++, pCur += width) *pCur = pre = pre+ALUT[*pCur-pre];
		pCur -= width; //�ص���β
		//4.��������: p4(i,j)=p4(i+1,j)+a*[p3(i,j)-p4(i+1,j)]
		for (y = height-2,pCur -= width; y>=0; y--, pCur -= width) *pCur = pre = pre+ALUT[*pCur-pre];
	}
	// step.3------------������=1,������Ϊ0,0����Ҳ��0----//
	pEnd = pTmpImg+width*height;
	for (pCur = pTmpImg, pGry = pGryImg; pCur<pEnd;pGry++)
	{
		*(pCur++) = (*pCur>*pGry);
	}
	// step.4------------�������----------------------//
	memset(pSJImg, 0, width*height); //��Եǿ�ȸ�0
 	pSJ = pSJImg+width; pCur = pTmpImg+width; //���в��� 
	for (y = 1; y<height-1; y++)
	{
		pSJ++; pCur++;  //���в���
		for (x = 1; x<width-1; x++, pGry++, pCur++, pSJ++)
		{
			if (*pCur)  //������
			{   //����ʹ��4����,��ԵΪ8��ͨ,���ܱ�֤4��ͨ;ʹ��8������ܱ�֤��Ե4��ͨ
				if ( (!*(pCur-1))|| //��,����<=0,����<0
					 (!*(pCur+1))|| //��,����<=0,����<0
					 (!*(pCur-width))|| //��,����<=0,����<0
					 (!*(pCur+width)) //��,����<=0,����<0
				   )
				{
					*pSJ = 255; //��Χ�е���С�ڵ���0
				}
			}
		}
		pSJ++; pCur++;  //β�в���
	}
	// step.5------------����---------------------------//
	return;
}

///////////////////////////////////////////////////////////////////////////////////////////
//
//Ӧ��ʾ����������Ե���
//
///////////////////////////////////////////////////////////////////////////////////////////
void RmwExtractRiceEdge( BYTE *pGryImg, //ԭʼ�Ҷ�ͼ�� 
	                     BYTE *pTmpImg, //����ͼ��
	                     int width,
	                     int height,
	                     double a0, 
	                     int grdThre,
	                     BYTE *pEdgeImg
                       )
{
	// step.1------------������-----------------------//
	RmwShenJunGryImg(pGryImg, pTmpImg, width, height, a0, pEdgeImg);
	// step.2------------Sobel����----------------------//
	RmwSobelGryImg(pGryImg, width, height, pTmpImg);
	// step.3------------�����ں�-----------------------//
	for (int i = 0; i<width*height; i++)
	{
		*(pEdgeImg+i) = (pEdgeImg[i]&&(pTmpImg[i]>grdThre))*255;
	}
	// step.4------------����---------------------------//
	return;
}
