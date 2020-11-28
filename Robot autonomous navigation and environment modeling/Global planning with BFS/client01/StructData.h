
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
	INT16 Timestamp;                  //时间戳
	BYTE task_finish;               //任务完成标志
	BYTE detect_object;             //发现目标标志
	BYTE collision;                 //碰撞标志
	INT16 obstacle[360];              //激光信息，正前方为0度，顺时针到359度，间隔1度
	POSE initial_rpose;             //机器人的初始位姿,0无效
	TPoint initial_dpose;             //目标初始位姿,0无效
	double right_angle;             //目标在机器人坐标系里位置，顺时针[0,PI],逆时针[0,-PI],该信息可不用
}S2CINFO;					// server to client
typedef struct{
	INT16 Timestamp;             //时间戳
	double tra_vel;            //线速度 cm/s
	double rot_vel;            //角速度 Rad/s
	POSE cur_rpose;            //当前的机器人位姿,0无效
	TPoint Traj[100];           //全局轨迹,由基于地图的全局规划获得,0无效
}C2SINFO;					// client to server