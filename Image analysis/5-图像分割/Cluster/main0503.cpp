#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "bmpFile.h"
#include "RmwALG_Cluster.h"
 
void testCluster()
{
	unsigned char *pGryImg;
	int width, height;
	int histogram[256];
	double pCenter[10]; //���10��
	int k,LUT[256],step;
	int i,mid,pre;
	bool ret;

	// step.1-------------��ͼ���ļ�------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0503\\Fig0503Gry.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	// step.2-------------����ͼ��--------------------------//
	//ֱ��ͼ
	RmwGetHistogram(pGryImg, width, height, histogram);
	//����
	k = 4; //�����
	ret=RmwClusterHistogram(histogram, k, 1.0, pCenter);
	//���ұ�
	if (ret)
	{
		step = 255/(k-1);
		mid = (int)(0.5+(pCenter[0]+pCenter[1])/2);
		for (i = 0; i<mid; i++) LUT[i] = 0;
		for (pre=mid,i = 1; i<k-1; i++)
		{
			mid = (int)(0.5+(pCenter[i]+pCenter[i+1])/2);
			for (i = pre; i<mid; i++) LUT[i] = step;
			step += step;
			pre = mid;
		}
		for (i = mid; i<256; i++) LUT[i] = 255;
		//�任
		for (i = 0; i<width*height; i++) pGryImg[i] = LUT[pGryImg[i]];
	}
	// step.3-------------����ͼ��--------------------------//
	Rmw_Write8BitImg2BmpFile(pGryImg, width, height, "..\\RmwCPP0503\\Fig0503Gry_cluster.bmp");
	// step.4-------------����------------------------------//
	delete pGryImg; //�ͷ��Լ�������ڴ�
	printf("Press any key to exit!\n");
	getchar();
	return;
}

void main()
{
	testCluster();
}

 