#ifndef IMG_PROC_FRAME_LIB_H
#define IMG_PROC_FRAME_LIB_H

//初始化
bool ImgProcFrame_Initialize(bool isRGB, int width, int height);
//执行
bool ImgProcFrame_DoNext(BYTE *pImgData, int frameID);
//对外接口
BYTE *ImgProcFrame_GetResImg();
int ImgProcFrame_GetResWidth();
int ImgProcFrame_GetResHeight();

#endif

