/////////////////////////////////////////////////////////////////////
//
// RmwRoadLaneRL.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "bmpFile.h"
#include "RmwALG_Project.h"
#include "RmwALG_Threshold.h"

#include "RmwRoadLaneRL.h"

//����Ӧ��ֵ
int RmwGetDifAdaptiveThreshold(int *prjOrg, int *prjAvr, int width)
{   //Լ��prjOrg,prjAvr��ֵ��С��256
	int histogram[256];
	int x, dif;

	memset(histogram, 0, sizeof(int)*256);
	for (x = 0; x<width; x++)
	{
		dif = abs(prjOrg[x]-prjAvr[x]);

		histogram[dif]++;
	}
	return RmwGetOtsuThreshold(histogram, 256);
}
/////////////////////////////////////////////////////////////////////
//
// ����/��������
//
/////////////////////////////////////////////////////////////////////
RmwRoadLaneRL::RmwRoadLaneRL()
{  	//�ڹ��캯���а����еĳ�Ա��������ֵ
	//�Ƿ��ʼ���ɹ�
	m_isInitedOK=false;
	//ͼ������
	m_width=0;
	m_height=0;
	//���Ƶ��е��߿��
	m_estLaneW = 0;
    //�ڴ�ռ�
	m_prjVerOrg = NULL;
	m_prjVerAvr = NULL;
	m_memImgSize = 0; //��������ڴ��С(�ο���׼)
	//���
	m_threshold = 0;
	m_nRL=0;
	//ͼ��֡��
	m_frameID=0;
}
	
RmwRoadLaneRL::~RmwRoadLaneRL()
{	//�������������ͷ����ж�̬������ڴ�
	Dump();
}

void RmwRoadLaneRL::Dump()
{   //дһ��ר�ŵ������ͷ��ڴ�ĺ���,��Ϊ�ڴ���ܻ��ж�ε��������ͷ�
	if (m_prjVerOrg) { delete m_prjVerOrg; m_prjVerOrg = NULL; }
	if (m_prjVerAvr) { delete m_prjVerAvr; m_prjVerAvr = NULL; }
	m_memImgSize = 0;
}
/////////////////////////////////////////////////////////////////////
//
// ��ʼ��
//
/////////////////////////////////////////////////////////////////////
bool RmwRoadLaneRL::Initialize( int width, //����ͼ��Ŀ��
	                            int height, //����ͼ��ĸ߶�
	                            int estLaneW //���Ƶ��е��߿��
                              )
{   //�ڳ�ʼ�������ж����еĳ�Ա��������ֵ
	//��̬�����ڴ�������ڳ�ʼ��������
	//���Ѿ�������ڴ���ڱ�����Ҫ���ڴ棬��������
	//��ʼ���������Ա���ε��ã�������Ϊ��λ����ʹ��

	//step.1------ͼ������--------------------------//
	m_width=width;
	m_height=height;
	m_estLaneW = estLaneW;
	//step.2------�ڴ�����--------------------------//
	if (m_width>m_memImgSize)
	{
		//���ͷ�
		Dump();
		//������
		m_prjVerOrg = new int[m_width];
		m_prjVerAvr = new int[m_width];
		//��¼����Ĵ�С
		m_memImgSize = m_width;
	}
	//step.3------��ʼ���ɹ���־--------------------//
	m_isInitedOK= m_prjVerOrg && m_prjVerAvr;
	return m_isInitedOK;
}
/////////////////////////////////////////////////////////////////////
//
// ִ��
//
/////////////////////////////////////////////////////////////////////
bool RmwRoadLaneRL::DoNext(BYTE *pGryImg, int frameID)
{ 	
	//step.0------��ȫ��֤-------------------------//
	if (!m_isInitedOK) return false;
	m_frameID = frameID; //��¼ͼ��֡��
 	//step.1------ͶӰ�ָ�-------------------------//
	PrjAndSegment(pGryImg);
	//step.2------���������-----------------------//
	Debug(pGryImg); //ÿ֡������
	return true;
}
/////////////////////////////////////////////////////////////////////
//
// ���-�õ�����������յ�
//
/////////////////////////////////////////////////////////////////////
int RmwRoadLaneRL::GetRL(int *pX1,int *pX2,int N)
{
	for (int i = 0; i<min(N, m_nRL); i++)
	{
		pX1[i] = m_pX1[i];
		pX2[i] = m_pX2[i];
	}
	return min(N, m_nRL);
}
/////////////////////////////////////////////////////////////////////
//
// ���-���ش�ֱͶӰ����
//
/////////////////////////////////////////////////////////////////////
int *RmwRoadLaneRL::GetPrjVer()
{	
	return m_prjVerOrg;
}
/////////////////////////////////////////////////////////////////////
//
// step.2------ͶӰ�ָ�
//
/////////////////////////////////////////////////////////////////////
void RmwRoadLaneRL::PrjAndSegment(BYTE *pGryImg)
{
	int x,dif;
	bool isBegin;

	//step.1------���㴹ֱͶӰ----------------------//
	RmwGetProjectVer(pGryImg, m_width, m_height, m_prjVerOrg);
	//��һ������,��֤������255
	for (x = 0; x<m_width; x++) m_prjVerOrg[x] /= m_height;
	//step.2------�Դ�ֱͶӰ���о�ֵ�˲�-------------//
	Rmw1DAverageFilter( m_prjVerOrg,
		                m_width, 
		                m_estLaneW, //���е��߿����Ϊ�˲��뾶
		                m_prjVerAvr
	                  );
	//step.3------��������Ӧ��ֵ--------------------//
	m_threshold = RmwGetDifAdaptiveThreshold(m_prjVerOrg, m_prjVerAvr, m_width);
	m_threshold = max(4, m_threshold); //���ԼӸ���ֵԼ��,���ٴ���4
	//step.4------��¼RL,ÿ������������յ�--------//
	m_nRL = 0;
	for (isBegin = true,x = 0; x<m_width; x++)
	{
		dif = m_prjVerOrg[x]-m_prjVerAvr[x]; //��Ϊ�е����ǰ�ɫ��,�϶����ڱ���
		if (dif<m_threshold)
		{
			if (!isBegin)
			{
				//��¼�յ�
				if (m_nRL<RMW_MAX_RL_NUM)
				{
					if (x-m_pX1[m_nRL]>3) //����ȥ��̫�̵�RL,����С�ڵ���3�� 
					{
						m_pX2[m_nRL] = x-1;
						m_nRL++;
					}
				}
			}
			isBegin = true;
		}
		else
		{
			//��¼���
			if (isBegin)
			{
				m_pX1[m_nRL] = x; 
				isBegin = false;
			}
		}
	}
	//step.5------����-----------------------------//
 	return;
}
/////////////////////////////////////////////////////////////////////
//
// ����
//
/////////////////////////////////////////////////////////////////////
void RmwRoadLaneRL::Debug(BYTE *pGryImg)
{   //������ڲ�����,��Ļ������ļ������
	int i;
	BYTE *pDbgImg,*pRow;
	char fileName[255];

	//step.1------����ͼ��------- ------------------//
	pDbgImg = new BYTE[m_width*m_height*3];
	memset(pDbgImg, 0, m_width*m_height*3);
	for (i = 0; i<m_width*m_height; i++) 
	{
		*(pDbgImg+i) = min(250, *(pGryImg+i));
	}
	//step.2------��ˮƽ�߶�,������������------------//
	pRow = pDbgImg+(m_height/2)*m_width;
	for (i = 0; i<m_nRL; i++)
	{
		memset(pRow+m_pX1[i], 0, m_pX2[i]-m_pX1[i]+1);
	}
	//step.3------��ͶӰ����------------------------//
	RmwDrawPrjVer2Img( pDbgImg+m_width*m_height, m_width,m_height,
		               m_prjVerOrg, m_prjVerAvr, m_width,
		               255, //ԭֵ-��ɫ
		               254  //��ֵ-��ɫ
	                 );
	//step.4------�����ļ�-------------------------//
	sprintf( fileName, "..\\RmwCPP0703\\frameID=%04d_threshold=%d_nRL=%d.bmp", 
		     m_frameID, 
		     m_threshold, m_nRL
	       );
	Rmw_Write8BitImg2BmpFileMark(pDbgImg, m_width,m_height*3, fileName);
	//step.5------����-----------------------------//
	delete pDbgImg;
	return;
}

