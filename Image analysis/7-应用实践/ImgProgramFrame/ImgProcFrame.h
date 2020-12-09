// ImgProcFrame.h : header file
//
#ifndef IMG_PROC_FRAME_H
#define IMG_PROC_FRAME_H

#include <Windows.h>

class ImgProcFrame
{
public:
	//����/����
	ImgProcFrame();
	~ImgProcFrame();
    //��ʼ��
    bool Initialize(bool isRGB,int width,int height);
	//ִ��
	bool DoNext(BYTE *pImgData, int frameID);
    //����ӿ�
    BYTE *GetResImg();
	int GetResWidth();
    int GetResHeight();
private:
	//�ڴ��ͷ�
	void Dump();
	//����
	void Debug();
private:
	//��ʼ���ɹ���־
	bool m_isInitedOK;
	//ͼ������
	bool m_isRGB;
	int m_width;
	int m_height;
	//�ڴ�
	BYTE *m_pRGBImg;
	BYTE *m_pGryImg;
	BYTE *m_pGrdImg;
	BYTE *m_pTmpImg;
	BYTE *m_pResImg;
	int m_memSize;
	//ͼ��֡��
	int m_frameID;
};
#endif
