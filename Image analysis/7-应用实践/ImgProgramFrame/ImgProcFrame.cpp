/////////////////////////////////////////////////////////////////////
//
// ImgProcFrame.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "ImgProcFrame.h"
/////////////////////////////////////////////////////////////////////
//
// ����/��������
//
/////////////////////////////////////////////////////////////////////
ImgProcFrame::ImgProcFrame()
{  	//�ڹ��캯���а����еĳ�Ա��������ֵ
	//�Ƿ��ʼ���ɹ�
	m_isInitedOK=false;
	//ͼ������
    m_isRGB=false;
	m_width=0;
	m_height=0;
    //�ڴ�ռ�
	m_pRGBImg=NULL;
	m_pGryImg=NULL;
	m_pGrdImg=NULL;
	m_pTmpImg=NULL;
	m_pResImg=NULL;
	m_memSize=0; //��������ڴ��С(�ο���׼)
	//ͼ��֡��
	m_frameID=0;
}
	
ImgProcFrame::~ImgProcFrame()
{	//�������������ͷ����ж�̬������ڴ�
	Dump();
}

void ImgProcFrame::Dump()
{   //дһ��ר�ŵ������ͷ��ڴ�ĺ���,��Ϊ�ڴ���ܻ��ж�ε��������ͷ�
	if (m_pRGBImg) { delete m_pRGBImg; m_pRGBImg=NULL; }
	if (m_pGryImg) { delete m_pGryImg; m_pGryImg=NULL; }
	if (m_pGrdImg) { delete m_pGrdImg; m_pGrdImg=NULL; }
	if (m_pTmpImg) { delete m_pTmpImg; m_pTmpImg=NULL; }
	if (m_pResImg) { delete m_pResImg; m_pResImg=NULL; }
	m_memSize=0;
}
/////////////////////////////////////////////////////////////////////
//
// ��ʼ��
//
/////////////////////////////////////////////////////////////////////
bool ImgProcFrame::Initialize(bool isRGB,int width,int height)
{   //�ڳ�ʼ�������ж����еĳ�Ա��������ֵ
	//��̬�����ڴ�������ڳ�ʼ��������
	//���Ѿ�������ڴ���ڱ�����Ҫ���ڴ棬��������
	//��ʼ���������Ա���ε��ã�������Ϊ��λ����ʹ��
	
	//step.1------ͼ������------------------------//
	m_isRGB=isRGB;
	m_width=width;
	m_height=height;
	//step.2------�ڴ�����------------------------//
	if (m_width*m_height>m_memSize)
	{
		//���ͷ�
		Dump();
		//������
		m_pRGBImg=new BYTE [m_width*m_height*3]; //��ɫͼ��
		m_pGryImg=new BYTE [m_width*m_height];  //�Ҷ�ͼ��
		m_pGrdImg=new BYTE [m_width*m_height]; //�ݶ�ͼ��
		m_pTmpImg=new BYTE [m_width*m_height]; //��ʱͼ��
		m_pResImg=new BYTE [m_width*m_height]; //���ͼ��
        //��¼��������ڴ��С(�ο���׼)
		m_memSize=m_width*m_height;
	}
	//step.3------��ʼ���ɹ���־------------------//
	m_isInitedOK = m_pRGBImg && 
		         m_pGryImg && 
		         m_pGrdImg && 
		         m_pTmpImg && 
		         m_pResImg;
	return m_isInitedOK;
}
/////////////////////////////////////////////////////////////////////
//
// ִ��
//
/////////////////////////////////////////////////////////////////////
bool ImgProcFrame::DoNext(BYTE *pImgData,int frameID)
{ 	
	//step.1------��ȫ��֤------------------------//
	if (!m_isInitedOK) return false;
	m_frameID= frameID; //��¼ͼ��֡��
	//step.2------�㷨����------------------------//
	//...
	//...
	//step.3------�����뷵��----------------------//
	//ĳ���ض�֡�ĵ���,����10
	if (m_frameID==10)
	{
		Debug();
	}
	//����
	return true;
}
/////////////////////////////////////////////////////////////////////
//
// ����ӿ�
//
/////////////////////////////////////////////////////////////////////
BYTE *ImgProcFrame::GetResImg()
{   //���ؽ��ͼ��
	return m_pResImg;
}
int ImgProcFrame::GetResWidth()
{   //���ؽ��ͼ��Ŀ��
	return m_width;
}
int ImgProcFrame::GetResHeight()
{   //���ؽ��ͼ��ĸ߶�
	return m_height;
}
/////////////////////////////////////////////////////////////////////
//
// ����
//
/////////////////////////////////////////////////////////////////////
void ImgProcFrame::Debug()
{   //������ڲ�����,��Ļ������ļ������
	return;
}
