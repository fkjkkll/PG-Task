#ifndef IMG_PROC_FRAME_LIB_H
#define IMG_PROC_FRAME_LIB_H

//��ʼ��
bool ImgProcFrame_Initialize(bool isRGB, int width, int height);
//ִ��
bool ImgProcFrame_DoNext(BYTE *pImgData, int frameID);
//����ӿ�
BYTE *ImgProcFrame_GetResImg();
int ImgProcFrame_GetResWidth();
int ImgProcFrame_GetResHeight();

#endif

