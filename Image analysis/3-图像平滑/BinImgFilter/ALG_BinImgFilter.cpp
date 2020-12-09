///////////////////////////////////////////////////////////////////////////////////////////
//
//   第三章
//
///////////////////////////////////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <nmmintrin.h>

///////////////////////////////////////////////////////////////////////////////////////
//
//二值图像滤波
//
///////////////////////////////////////////////////////////////////////////////////////
void RmwBinImgFilter( BYTE *pBinImg,  //原始二值图像
	                  int width, int height,//图像的宽度和高度
	                  int M, int N, //滤波邻域：M列N行
	                  int threshold, //灰度阈值,大于等于该值时结果赋255
	                  BYTE *pResImg //结果图像
                    )
{   //没有对边界上邻域不完整的像素进行处理，可以采用变窗口的策略
	BYTE *pAdd, *pDel, *pRes;
	int halfx, halfy;
	int x, y, sum;
	int sumCol[4096]; //约定图像宽度不大于4096

	// step.1------------初始化--------------------------//
	M = M/2*2+1; //奇数化
	N= N/2*2+1; //奇数化
	halfx = M/2; //滤波器的x半径
	halfy = N/2; //滤波器的y半径
	threshold = threshold *M*N; //转换成邻域内灰度值之和的阈值
	memset(sumCol, 0, sizeof(int)*width);
	for (y = 0, pAdd = pBinImg; y<N; y++)
	{
		for (x = 0; x<width; x++) sumCol[x] += *(pAdd++);
	}
	// step.2------------滤波----------------------------//
	for (y = halfy, pRes = pResImg+y*width,pDel= pBinImg; y<height-halfy; y++)
	{
		//初值
		for (sum=0,x = 0; x<M; x++) sum += sumCol[x];
		//滤波
		pRes += halfx; //跳过左侧
		for (x = halfx; x<width-halfx; x++)
		{
			//求灰度均值
			*(pRes++) = (sum>=threshold)*255; //请理解这个表达式的含义
			//换列,更新灰度和
			sum -= sumCol[x-halfx]; //减左边列
			sum += sumCol[x+halfx+1]; //加右边列
		}
		pRes += halfx;//跳过右侧
		//换行,更新sumCol
		for (x = 0; x<width; x++)
		{
			sumCol[x] -= *(pDel++); //减上一行
			sumCol[x] += *(pAdd++); //加下一行
		}
	}
	// step.3------------返回----------------------------//
	return;
}