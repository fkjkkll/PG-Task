
#define PI 3.1415926
typedef struct{
	INT16 coor_x;
	INT16 coor_y;
	double coor_ori;
}POSE;
typedef struct{
	INT16 coor_x;
	INT16 coor_y;
}TPoint;
typedef struct{
	INT16 Timestamp;                  //ʱ���
	BYTE task_finish;               //������ɱ�־
	BYTE detect_object;             //����Ŀ���־
	BYTE collision;                 //��ײ��־
	INT16 obstacle[360];              //������Ϣ����ǰ��Ϊ0�ȣ�˳ʱ�뵽359�ȣ����1��
	POSE initial_rpose;             //�����˵ĳ�ʼλ��,0��Ч
	TPoint initial_dpose;             //Ŀ���ʼλ��,0��Ч
	double right_angle;             //Ŀ���ڻ���������ϵ��λ�ã�˳ʱ��[0,PI],��ʱ��[0,-PI],����Ϣ�ɲ���
}S2CINFO;					// server to client
typedef struct{
	INT16 Timestamp;             //ʱ���
	double tra_vel;            //���ٶ� cm/s
	double rot_vel;            //���ٶ� Rad/s
	POSE cur_rpose;            //��ǰ�Ļ�����λ��,0��Ч
	TPoint Traj[100];           //ȫ�ֹ켣,�ɻ��ڵ�ͼ��ȫ�ֹ滮���,0��Ч
}C2SINFO;					// client to server