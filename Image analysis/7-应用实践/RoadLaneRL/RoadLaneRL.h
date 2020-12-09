/////////////////////////////////////////////////////////////////////
//
// RmwRoadLaneRL.h : header file
//
/////////////////////////////////////////////////////////////////////
#ifndef RMW_ROAD_LANE_RL_H
#define RMW_ROAD_LANE_RL_H

#define RMW_MAX_RL_NUM       32            //最多32条候选线段

class RmwRoadLaneRL
{
public:
	//构造/析构
	RmwRoadLaneRL();
	~RmwRoadLaneRL();
    //初始化
    bool Initialize( int width, //输入图像的宽度
		             int height, //输入图像的高度
		             int estLaneW //估计的行道线宽度
	               );
	//执行
	bool DoNext(BYTE *pGryImg,int frameID);
	//结果-得到波峰的起点和终点
	int GetRL(int *pX1, int *pX2, int N);
	//结果-返回垂直投影数据
	int *GetPrjVer(); 
private:
	//内存释放
	void Dump();
	//调试
	void Debug(BYTE *pGryImg);
	//投影分割
	void PrjAndSegment(BYTE *pGryImg);
private:
	//初始化成功的标志
	bool m_isInitedOK;
	//图像属性
	int m_width;
	int m_height; 
	//估计的行道线宽度
	int m_estLaneW;
	//内存
	int *m_prjVerOrg;
	int *m_prjVerAvr;
	int m_memImgSize;
	//结果
	int m_threshold;
	int m_pX1[RMW_MAX_RL_NUM]; //RL的起点
	int m_pX2[RMW_MAX_RL_NUM]; //RL的终点
	int m_nRL;
	//图像帧号
	int m_frameID;
};
#endif
