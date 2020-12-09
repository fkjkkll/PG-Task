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
// 构造/析构函数
//
/////////////////////////////////////////////////////////////////////
ImgProcFrame::ImgProcFrame()
{  	//在构造函数中把所有的成员变量赋零值
	//是否初始化成功
	m_isInitedOK=false;
	//图像属性
    m_isRGB=false;
	m_width=0;
	m_height=0;
    //内存空间
	m_pRGBImg=NULL;
	m_pGryImg=NULL;
	m_pGrdImg=NULL;
	m_pTmpImg=NULL;
	m_pResImg=NULL;
	m_memSize=0; //已申请的内存大小(参考标准)
	//图像帧号
	m_frameID=0;
}
	
ImgProcFrame::~ImgProcFrame()
{	//在析构函数中释放类中动态申请的内存
	Dump();
}

void ImgProcFrame::Dump()
{   //写一个专门的用作释放内存的函数,因为内存可能会有多次的申请与释放
	if (m_pRGBImg) { delete m_pRGBImg; m_pRGBImg=NULL; }
	if (m_pGryImg) { delete m_pGryImg; m_pGryImg=NULL; }
	if (m_pGrdImg) { delete m_pGrdImg; m_pGrdImg=NULL; }
	if (m_pTmpImg) { delete m_pTmpImg; m_pTmpImg=NULL; }
	if (m_pResImg) { delete m_pResImg; m_pResImg=NULL; }
	m_memSize=0;
}
/////////////////////////////////////////////////////////////////////
//
// 初始化
//
/////////////////////////////////////////////////////////////////////
bool ImgProcFrame::Initialize(bool isRGB,int width,int height)
{   //在初始化函数中对所有的成员变量赋初值
	//动态申请内存仅发生在初始化函数中
	//若已经申请的内存大于本次需要的内存，则不再申请
	//初始化函数可以被多次调用，可以作为复位函数使用
	
	//step.1------图像属性------------------------//
	m_isRGB=isRGB;
	m_width=width;
	m_height=height;
	//step.2------内存申请------------------------//
	if (m_width*m_height>m_memSize)
	{
		//先释放
		Dump();
		//后申请
		m_pRGBImg=new BYTE [m_width*m_height*3]; //彩色图像
		m_pGryImg=new BYTE [m_width*m_height];  //灰度图像
		m_pGrdImg=new BYTE [m_width*m_height]; //梯度图像
		m_pTmpImg=new BYTE [m_width*m_height]; //临时图像
		m_pResImg=new BYTE [m_width*m_height]; //结果图像
        //记录已申请的内存大小(参考标准)
		m_memSize=m_width*m_height;
	}
	//step.3------初始化成功标志------------------//
	m_isInitedOK = m_pRGBImg && 
		         m_pGryImg && 
		         m_pGrdImg && 
		         m_pTmpImg && 
		         m_pResImg;
	return m_isInitedOK;
}
/////////////////////////////////////////////////////////////////////
//
// 执行
//
/////////////////////////////////////////////////////////////////////
bool ImgProcFrame::DoNext(BYTE *pImgData,int frameID)
{ 	
	//step.1------安全验证------------------------//
	if (!m_isInitedOK) return false;
	m_frameID= frameID; //记录图像帧号
	//step.2------算法处理------------------------//
	//...
	//...
	//step.3------调试与返回----------------------//
	//某个特定帧的调试,比如10
	if (m_frameID==10)
	{
		Debug();
	}
	//返回
	return true;
}
/////////////////////////////////////////////////////////////////////
//
// 对外接口
//
/////////////////////////////////////////////////////////////////////
BYTE *ImgProcFrame::GetResImg()
{   //返回结果图像
	return m_pResImg;
}
int ImgProcFrame::GetResWidth()
{   //返回结果图像的宽度
	return m_width;
}
int ImgProcFrame::GetResHeight()
{   //返回结果图像的高度
	return m_height;
}
/////////////////////////////////////////////////////////////////////
//
// 调试
//
/////////////////////////////////////////////////////////////////////
void ImgProcFrame::Debug()
{   //本类的内部调试,屏幕输出、文件输出等
	return;
}
