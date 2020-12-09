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

//自适应阈值
int RmwGetDifAdaptiveThreshold(int *prjOrg, int *prjAvr, int width)
{   //约定prjOrg,prjAvr的值都小于256
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
// 构造/析构函数
//
/////////////////////////////////////////////////////////////////////
RmwRoadLaneRL::RmwRoadLaneRL()
{  	//在构造函数中把所有的成员变量赋零值
	//是否初始化成功
	m_isInitedOK=false;
	//图像属性
	m_width=0;
	m_height=0;
	//估计的行道线宽度
	m_estLaneW = 0;
    //内存空间
	m_prjVerOrg = NULL;
	m_prjVerAvr = NULL;
	m_memImgSize = 0; //已申请的内存大小(参考标准)
	//结果
	m_threshold = 0;
	m_nRL=0;
	//图像帧号
	m_frameID=0;
}
	
RmwRoadLaneRL::~RmwRoadLaneRL()
{	//在析构函数中释放类中动态申请的内存
	Dump();
}

void RmwRoadLaneRL::Dump()
{   //写一个专门的用作释放内存的函数,因为内存可能会有多次的申请与释放
	if (m_prjVerOrg) { delete m_prjVerOrg; m_prjVerOrg = NULL; }
	if (m_prjVerAvr) { delete m_prjVerAvr; m_prjVerAvr = NULL; }
	m_memImgSize = 0;
}
/////////////////////////////////////////////////////////////////////
//
// 初始化
//
/////////////////////////////////////////////////////////////////////
bool RmwRoadLaneRL::Initialize( int width, //输入图像的宽度
	                            int height, //输入图像的高度
	                            int estLaneW //估计的行道线宽度
                              )
{   //在初始化函数中对所有的成员变量赋初值
	//动态申请内存仅发生在初始化函数中
	//若已经申请的内存大于本次需要的内存，则不再申请
	//初始化函数可以被多次调用，可以作为复位函数使用

	//step.1------图像属性--------------------------//
	m_width=width;
	m_height=height;
	m_estLaneW = estLaneW;
	//step.2------内存申请--------------------------//
	if (m_width>m_memImgSize)
	{
		//先释放
		Dump();
		//后申请
		m_prjVerOrg = new int[m_width];
		m_prjVerAvr = new int[m_width];
		//记录申请的大小
		m_memImgSize = m_width;
	}
	//step.3------初始化成功标志--------------------//
	m_isInitedOK= m_prjVerOrg && m_prjVerAvr;
	return m_isInitedOK;
}
/////////////////////////////////////////////////////////////////////
//
// 执行
//
/////////////////////////////////////////////////////////////////////
bool RmwRoadLaneRL::DoNext(BYTE *pGryImg, int frameID)
{ 	
	//step.0------安全验证-------------------------//
	if (!m_isInitedOK) return false;
	m_frameID = frameID; //记录图像帧号
 	//step.1------投影分割-------------------------//
	PrjAndSegment(pGryImg);
	//step.2------返回与调试-----------------------//
	Debug(pGryImg); //每帧都调试
	return true;
}
/////////////////////////////////////////////////////////////////////
//
// 结果-得到波峰的起点和终点
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
// 结果-返回垂直投影数据
//
/////////////////////////////////////////////////////////////////////
int *RmwRoadLaneRL::GetPrjVer()
{	
	return m_prjVerOrg;
}
/////////////////////////////////////////////////////////////////////
//
// step.2------投影分割
//
/////////////////////////////////////////////////////////////////////
void RmwRoadLaneRL::PrjAndSegment(BYTE *pGryImg)
{
	int x,dif;
	bool isBegin;

	//step.1------计算垂直投影----------------------//
	RmwGetProjectVer(pGryImg, m_width, m_height, m_prjVerOrg);
	//归一化处理,保证不大于255
	for (x = 0; x<m_width; x++) m_prjVerOrg[x] /= m_height;
	//step.2------对垂直投影进行均值滤波-------------//
	Rmw1DAverageFilter( m_prjVerOrg,
		                m_width, 
		                m_estLaneW, //以行道线宽度作为滤波半径
		                m_prjVerAvr
	                  );
	//step.3------计算自适应阈值--------------------//
	m_threshold = RmwGetDifAdaptiveThreshold(m_prjVerOrg, m_prjVerAvr, m_width);
	m_threshold = max(4, m_threshold); //可以加个阈值约束,至少大于4
	//step.4------记录RL,每个波峰的起点和终点--------//
	m_nRL = 0;
	for (isBegin = true,x = 0; x<m_width; x++)
	{
		dif = m_prjVerOrg[x]-m_prjVerAvr[x]; //因为行道线是白色的,肯定大于背景
		if (dif<m_threshold)
		{
			if (!isBegin)
			{
				//记录终点
				if (m_nRL<RMW_MAX_RL_NUM)
				{
					if (x-m_pX1[m_nRL]>3) //可以去掉太短的RL,比如小于等于3的 
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
			//记录起点
			if (isBegin)
			{
				m_pX1[m_nRL] = x; 
				isBegin = false;
			}
		}
	}
	//step.5------返回-----------------------------//
 	return;
}
/////////////////////////////////////////////////////////////////////
//
// 调试
//
/////////////////////////////////////////////////////////////////////
void RmwRoadLaneRL::Debug(BYTE *pGryImg)
{   //本类的内部调试,屏幕输出、文件输出等
	int i;
	BYTE *pDbgImg,*pRow;
	char fileName[255];

	//step.1------调试图像------- ------------------//
	pDbgImg = new BYTE[m_width*m_height*3];
	memset(pDbgImg, 0, m_width*m_height*3);
	for (i = 0; i<m_width*m_height; i++) 
	{
		*(pDbgImg+i) = min(250, *(pGryImg+i));
	}
	//step.2------画水平线段,画在中心行上------------//
	pRow = pDbgImg+(m_height/2)*m_width;
	for (i = 0; i<m_nRL; i++)
	{
		memset(pRow+m_pX1[i], 0, m_pX2[i]-m_pX1[i]+1);
	}
	//step.3------画投影曲线------------------------//
	RmwDrawPrjVer2Img( pDbgImg+m_width*m_height, m_width,m_height,
		               m_prjVerOrg, m_prjVerAvr, m_width,
		               255, //原值-红色
		               254  //均值-绿色
	                 );
	//step.4------保存文件-------------------------//
	sprintf( fileName, "..\\RmwCPP0703\\frameID=%04d_threshold=%d_nRL=%d.bmp", 
		     m_frameID, 
		     m_threshold, m_nRL
	       );
	Rmw_Write8BitImg2BmpFileMark(pDbgImg, m_width,m_height*3, fileName);
	//step.5------返回-----------------------------//
	delete pDbgImg;
	return;
}

