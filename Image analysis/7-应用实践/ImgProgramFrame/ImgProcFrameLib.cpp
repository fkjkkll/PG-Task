#include "ImgProcFrame.h"

//����һ��ȫ�ֶ���
static class ImgProcFrame  _gImgProc; 
//��ʼ��
bool ImgProcFrame_Initialize(bool isRGB, int width, int height)
{
	return _gImgProc.Initialize(isRGB, width, height);
}
//ִ��
bool ImgProcFrame_DoNext(BYTE *pImgData, int frameID)
{
	return _gImgProc.DoNext(pImgData, frameID);
}
//����ӿ�
BYTE *ImgProcFrame_GetResImg()
{
	return _gImgProc.GetResImg();
}
int ImgProcFrame_GetResWidth()
{
	return _gImgProc.GetResWidth();
}
int ImgProcFrame_GetResHeight()
{
	return _gImgProc.GetResHeight();
}
