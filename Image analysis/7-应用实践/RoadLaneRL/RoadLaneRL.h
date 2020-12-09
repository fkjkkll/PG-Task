/////////////////////////////////////////////////////////////////////
//
// RmwRoadLaneRL.h : header file
//
/////////////////////////////////////////////////////////////////////
#ifndef RMW_ROAD_LANE_RL_H
#define RMW_ROAD_LANE_RL_H

#define RMW_MAX_RL_NUM       32            //���32����ѡ�߶�

class RmwRoadLaneRL
{
public:
	//����/����
	RmwRoadLaneRL();
	~RmwRoadLaneRL();
    //��ʼ��
    bool Initialize( int width, //����ͼ��Ŀ��
		             int height, //����ͼ��ĸ߶�
		             int estLaneW //���Ƶ��е��߿��
	               );
	//ִ��
	bool DoNext(BYTE *pGryImg,int frameID);
	//���-�õ�����������յ�
	int GetRL(int *pX1, int *pX2, int N);
	//���-���ش�ֱͶӰ����
	int *GetPrjVer(); 
private:
	//�ڴ��ͷ�
	void Dump();
	//����
	void Debug(BYTE *pGryImg);
	//ͶӰ�ָ�
	void PrjAndSegment(BYTE *pGryImg);
private:
	//��ʼ���ɹ��ı�־
	bool m_isInitedOK;
	//ͼ������
	int m_width;
	int m_height; 
	//���Ƶ��е��߿��
	int m_estLaneW;
	//�ڴ�
	int *m_prjVerOrg;
	int *m_prjVerAvr;
	int m_memImgSize;
	//���
	int m_threshold;
	int m_pX1[RMW_MAX_RL_NUM]; //RL�����
	int m_pX2[RMW_MAX_RL_NUM]; //RL���յ�
	int m_nRL;
	//ͼ��֡��
	int m_frameID;
};
#endif
