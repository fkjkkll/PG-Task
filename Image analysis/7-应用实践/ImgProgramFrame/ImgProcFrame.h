// ImgProcFrame.h : header file
//
#ifndef IMG_PROC_FRAME_H
#define IMG_PROC_FRAME_H

#include <Windows.h>

class ImgProcFrame
{
public:
	//构造/析构
	ImgProcFrame();
	~ImgProcFrame();
    //初始化
    bool Initialize(bool isRGB,int width,int height);
	//执行
	bool DoNext(BYTE *pImgData, int frameID);
    //对外接口
    BYTE *GetResImg();
	int GetResWidth();
    int GetResHeight();
private:
	//内存释放
	void Dump();
	//调试
	void Debug();
private:
	//初始化成功标志
	bool m_isInitedOK;
	//图像属性
	bool m_isRGB;
	int m_width;
	int m_height;
	//内存
	BYTE *m_pRGBImg;
	BYTE *m_pGryImg;
	BYTE *m_pGrdImg;
	BYTE *m_pTmpImg;
	BYTE *m_pResImg;
	int m_memSize;
	//图像帧号
	int m_frameID;
};
#endif
