#ifndef _RMW_ALG_BINIMG_FILTER_H
#define _RMW_ALG_BINIMG_FILTER_H

///////////////////////////////////////////////////////////////////////////////////////
//
//��ֵͼ���˲�
//
///////////////////////////////////////////////////////////////////////////////////////
void RmwBinImgFilter( BYTE *pBinImg,  //ԭʼ��ֵͼ��
	                  int width, int height,//ͼ��Ŀ�Ⱥ͸߶�
	                  int M, int N, //�˲�����M��N��
	                  int threshold, //�Ҷ���ֵ,���ڵ��ڸ�ֵʱ�����255
	                  BYTE *pResImg //���ͼ��
                    );

#endif

