#ifndef _RMW_ALG_EDGE_DETECTOR_H
#define _RMW_ALG_EDGE_DETECTOR_H

///////////////////////////////////////////////////////////////////////////////////////////
//
//   ������
//
///////////////////////////////////////////////////////////////////////////////////////////
//Gradient����
void RmwGradientGryImg(BYTE *pGryImg, int width, int height, BYTE *pGrdImg);
//Robert����
void RmwRobertsGryImg(BYTE *pGryImg, int width, int height, BYTE *pRbtImg);
//Sobel����
void RmwSobelGryImg(BYTE *pGryImg, int width, int height, BYTE *pSbImg);
//Prewitt����
void RmwPrewittGryImg(BYTE *pGryImg, int width, int height, BYTE *pPRTImg);
//������
void RmwShenJunGryImg(BYTE *pGryImg, BYTE *pTmpImg, int width, int height, double a0, BYTE *pSJImg);
///////////////////////////////////////////////////////////////////////////////////////////
//
//Ӧ��ʾ����������Ե���
//
///////////////////////////////////////////////////////////////////////////////////////////
void RmwExtractRiceEdge( BYTE *pGryImg, //ԭʼ�Ҷ�ͼ�� 
	                     BYTE *pTmpImg, //����ͼ��
	                     int width,
	                     int height,
	                     double a0, 
	                     int grdThre,
	                     BYTE *pEdgeImg
                       );
#endif

