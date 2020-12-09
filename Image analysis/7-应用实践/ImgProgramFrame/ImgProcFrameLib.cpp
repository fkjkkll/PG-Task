#include "ImgProcFrame.h"

//定义一个全局对象
static class ImgProcFrame  _gImgProc; 
//初始化
bool ImgProcFrame_Initialize(bool isRGB, int width, int height)
{
	return _gImgProc.Initialize(isRGB, width, height);
}
//执行
bool ImgProcFrame_DoNext(BYTE *pImgData, int frameID)
{
	return _gImgProc.DoNext(pImgData, frameID);
}
//对外接口
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
