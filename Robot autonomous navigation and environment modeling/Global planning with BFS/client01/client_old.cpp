#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <atlstr.h>
#include "list"
#include "vector"
#include "winsock2.h"
#pragma comment(lib,"ws2_32.lib")//引用库文件
using namespace std;
#define MAX_CAR_NUM 6
#define PI 3.1415926
int Run_status = 1;
int Node_Num[MAX_CAR_NUM];
typedef struct{
	int node_id;					/*坐标点编号*/
	POINT node_coor;				/*坐标点*/
	int node_flag;                  //表示首先被哪个机器人访问？
	bool Be_seen[MAX_CAR_NUM];      //节点对其他机器人的可见性
	int branch_num;
	int around_node_id[4];//相邻节点编号
	double orientation[4];//branch orientation
	int length[4];        //branch orientation
}NODE;
typedef struct{
	int node1_id;
	int node2_id;
	int edge_len;
	POINT node1_coor;
	POINT node2_coor;
	int edge_flag;
	bool Be_seen[MAX_CAR_NUM];
	int Edge_visit_record[MAX_CAR_NUM];
}EDGE;
typedef struct{
	bool Valid;            //update flag;
	int branch_num;
	double orientation[4];    //branch orientation
	CString imagepath[4];
}NODE_PUSH;
typedef struct{
	double r_x;
	double r_y;
	double r_orientation;
}CAR_POSE;
typedef struct{
	int Timestamp;                  //时间戳
	int Run_status;
	int Target_ID;			        //目标ID
	int Source_ID;			        //源ID,非Server时为转发信息，Info_Type后数据有效
	CAR_POSE r_Pos;                 //机器人位姿，仅Orientation有效
	double r_Relative_Dis;			//机器人里程数（距上一节点）
	double r_Relative_Ori;			//机器人姿态（弧度）
	double r_Road_Ori;				//道路方向（弧度）
	NODE_PUSH Node_push;            //节点推送信息
	bool CanComm[MAX_CAR_NUM];      //可通讯的机器人///////////
	int ReqData_Type[MAX_CAR_NUM];  //请求:0:无请求, 1:有请求
	int AnsData_Type[MAX_CAR_NUM];  //答复，1：同意请求；2：拒绝请求
	int Target_Node_ID[MAX_CAR_NUM];//投标节点序号
	int Cost[MAX_CAR_NUM];          //成本
}S2CINFO;
typedef struct{
	int Timestamp;                  //时间戳
	int Target_ID;			        //目标ID
	int Source_ID;			        //源ID
	CAR_POSE r_Pos;                 //机器人推算位姿,(x,y,orientaition均有效)
	int ReqData_Type;               //请求：0:无请求，1：有请求
	int AnsData_Type[MAX_CAR_NUM];  //答复，1：同意请求；2：拒绝请求
	double r_Desired_ori;			//分支方向选择（节点处）
	int Target_Node_ID;		        //投标节点序号
	int Cost;                       //成本
	double r_linear_vel;            //线速度 cm/s
	double r_angular_vel;           //角速度 Rad/s
}C2SINFO;
typedef std::vector<NODE> VECTOR_NODE;
S2CINFO S2Cdata[MAX_CAR_NUM];
C2SINFO C2Sdata[MAX_CAR_NUM];
CAR_POSE Initial_Pos[MAX_CAR_NUM];
CAR_POSE Car_Pos[MAX_CAR_NUM];
NODE Previous_Node[MAX_CAR_NUM];
NODE Cur_Node[MAX_CAR_NUM];
VECTOR_NODE Vector_Node[MAX_CAR_NUM];
void Initialition(){
	memset(S2Cdata, 0, MAX_CAR_NUM*sizeof(S2CINFO));
	memset(C2Sdata, 0, MAX_CAR_NUM*sizeof(C2SINFO));
	memset(Initial_Pos, 0, MAX_CAR_NUM*sizeof(CAR_POSE));
	memset(Car_Pos, 0, MAX_CAR_NUM*sizeof(CAR_POSE));
	memset(Previous_Node, 0, MAX_CAR_NUM*sizeof(NODE));
	memset(Cur_Node, 0, MAX_CAR_NUM*sizeof(NODE));
	for (int i = 0; i < MAX_CAR_NUM; i++){
		Node_Num[i] = 0;
		Vector_Node[i].clear();
	}
}
void UpdatePos(S2CINFO S2Cdata[MAX_CAR_NUM], C2SINFO(&C2Sdata)[MAX_CAR_NUM], CAR_POSE(&Car_Pos)[MAX_CAR_NUM], NODE Cur_Node[MAX_CAR_NUM], int Car_num){
	Car_Pos[Car_num].r_orientation = S2Cdata[Car_num].r_Pos.r_orientation;
	Car_Pos[Car_num].r_x = Cur_Node[Car_num].node_coor.x + S2Cdata[Car_num].r_Relative_Dis*cos(S2Cdata[Car_num].r_Relative_Ori);
	Car_Pos[Car_num].r_y = Cur_Node[Car_num].node_coor.y + S2Cdata[Car_num].r_Relative_Dis*sin(S2Cdata[Car_num].r_Relative_Ori);
	C2Sdata[Car_num].r_Pos.r_x = Car_Pos[Car_num].r_x;
	C2Sdata[Car_num].r_Pos.r_y = Car_Pos[Car_num].r_y;
	C2Sdata[Car_num].r_Pos.r_orientation = Car_Pos[Car_num].r_orientation;
	C2Sdata[Car_num].Source_ID = Car_num;
	C2Sdata[Car_num].Target_ID = 99;
	C2Sdata[Car_num].Target_Node_ID = -1;
	C2Sdata[Car_num].ReqData_Type = 0;
	for (int i = 0; i < MAX_CAR_NUM; i++){
		C2Sdata[Car_num].AnsData_Type[i] = -1;
	}
}
void UpdateNode(S2CINFO S2Cdata[MAX_CAR_NUM], VECTOR_NODE(&Vector_Node)[MAX_CAR_NUM], NODE Cur_Node[MAX_CAR_NUM], NODE Previous_Node[MAX_CAR_NUM], int Car_num){
	if (S2Cdata[Car_num].Node_push.Valid){//到达新的节点，更新
		if (Node_Num[Car_num] == 0){//第一个节点，节点坐标依赖机器人的初始坐标
			NODE temp_node;
			temp_node.node_flag = Car_num;
			temp_node.node_id = Node_Num[Car_num];
			temp_node.node_coor.x = Car_Pos[Car_num].r_x;
			temp_node.node_coor.y = Car_Pos[Car_num].r_y;
			temp_node.branch_num = S2Cdata[Car_num].Node_push.branch_num;
			for (int j = 0; j < temp_node.branch_num; j++){
				temp_node.around_node_id[j] = -1;
				temp_node.orientation[j] = S2Cdata[Car_num].Node_push.orientation[j];
			}
			Vector_Node[Car_num].push_back(temp_node);
			Cur_Node[Car_num] = temp_node;
			Previous_Node[Car_num] = temp_node;
			Node_Num[Car_num]++;
		}
		else{//已经有一个节点
			if (S2Cdata[Car_num].r_Relative_Dis>20){//新节点
				NODE temp_node;
				temp_node.node_flag = Car_num;
				temp_node.node_id = Node_Num[Car_num];
				temp_node.node_coor.x = Car_Pos[Car_num].r_x;
				temp_node.node_coor.y = Car_Pos[Car_num].r_y;
				temp_node.branch_num = S2Cdata[Car_num].Node_push.branch_num;
				for (int j = 0; j < temp_node.branch_num; j++){
					temp_node.around_node_id[j] = -1;
					temp_node.orientation[j] = S2Cdata[Car_num].Node_push.orientation[j];
				}
				Vector_Node[Car_num].push_back(temp_node);
				Previous_Node[Car_num] = Cur_Node[Car_num];
				Cur_Node[Car_num] = temp_node;
				Node_Num[Car_num]++;
			}
		}
	}
}
double get_angle_error(double r_orientation, double desired_ori)//OK!
{
	double angle_error = desired_ori - r_orientation;
	if (angle_error > PI)
		angle_error -= PI * 2;
	else if (angle_error <= -PI)
		angle_error += PI * 2;
	return angle_error;
}
double Local_Greedy(S2CINFO S2Cdata[MAX_CAR_NUM], C2SINFO (&C2Sdata)[MAX_CAR_NUM], int Car_num){
	double desired_angle = 6.4;
	if (S2Cdata[Car_num].Node_push.Valid){ //到达节点
		C2Sdata[Car_num].Timestamp = S2Cdata[Car_num].Timestamp;
		double Real_Ori = S2Cdata[Car_num].r_Pos.r_orientation;
		int branch_num = S2Cdata[Car_num].Node_push.branch_num;
		double minimum_angle = PI * 2;
		int num = 4;
		for (int i = 0; i < branch_num; i++){
			double angle_error = fabs(get_angle_error(Real_Ori, S2Cdata[Car_num].Node_push.orientation[i]));
			if (minimum_angle > angle_error){
				num = i;
				minimum_angle = angle_error;
				desired_angle = S2Cdata[Car_num].Node_push.orientation[i];
			}
		}
	}
	C2Sdata[Car_num].r_Desired_ori = desired_angle;
	return desired_angle;
}
CString GetExePath()
{
	CString strExePath;
	CString strPath;
	GetModuleFileName(NULL, strPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH + 1);
	int nPos = strPath.ReverseFind(_T('\\'));
	strExePath = strPath.Left(nPos + 1);
	return strExePath;
}
int main()
{
	char buffs[] = "";
	CString path = GetExePath() + "plan_MFCSingle";
	ShellExecute(NULL, NULL, path, NULL, NULL, SW_MAXIMIZE);
	printf("请先点击MC_Server的运行-开启远程，再输入任意字符，按回车键继续！");
	//while (getchar() != '\n')
		//continue;
	cin >> buffs;
	//加载套接字
	Initialition();
	WSADATA wsaData;
	char Recvbuff[1400];
	char Sendbuff[1024];
	memset(Recvbuff, 0, sizeof(Recvbuff));
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("初始化Winsock失败");
		return 0;
	}

	SOCKADDR_IN addrSrv;
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8888);//端口号
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//IP地址
	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);//创建套接字
	if (SOCKET_ERROR == sockClient){
		printf("Socket() error:%d", WSAGetLastError());
		return 0;
	}
	//向服务器发出连接请求
	if (connect(sockClient, (struct  sockaddr*)&addrSrv, sizeof(addrSrv)) == INVALID_SOCKET){
		printf("连接失败:%d", WSAGetLastError());
		return 0;
	}
	while (Run_status){
		if (recv(sockClient, Recvbuff, sizeof(Recvbuff), 0)){
			memset(S2Cdata, 0, MAX_CAR_NUM*sizeof(S2CINFO));
			memcpy(S2Cdata, Recvbuff, MAX_CAR_NUM*sizeof(S2CINFO));
			Run_status = S2Cdata[0].Run_status;
			if (Run_status == 0)
				break;
			memset(C2Sdata, 0, MAX_CAR_NUM*sizeof(C2SINFO));
			if (S2Cdata[0].Timestamp == 0){//第一帧
				for (int Car_num = 0; Car_num < MAX_CAR_NUM; Car_num++){
					Initial_Pos[Car_num] = S2Cdata[Car_num].r_Pos;
					Car_Pos[Car_num] = S2Cdata[Car_num].r_Pos;
					Cur_Node[Car_num].node_coor.x = Car_Pos[Car_num].r_x;
					Cur_Node[Car_num].node_coor.y = Car_Pos[Car_num].r_y;
					Previous_Node[Car_num] = Cur_Node[Car_num];
					UpdatePos(S2Cdata, C2Sdata, Car_Pos, Cur_Node, Car_num);
					UpdateNode(S2Cdata, Vector_Node, Cur_Node, Previous_Node, Car_num);
					Local_Greedy(S2Cdata, C2Sdata, Car_num);
				}
				memset(Sendbuff, 0, sizeof(Sendbuff));
				memcpy(Sendbuff, C2Sdata, MAX_CAR_NUM*sizeof(C2SINFO));
			}
			else{
				for (int Car_num = 0; Car_num < MAX_CAR_NUM; Car_num++){
					UpdatePos(S2Cdata, C2Sdata, Car_Pos, Cur_Node, Car_num);
					UpdateNode(S2Cdata, Vector_Node, Cur_Node, Previous_Node, Car_num);
					Local_Greedy(S2Cdata, C2Sdata, Car_num);
				}
				memset(Sendbuff, 0, sizeof(Sendbuff));
				memcpy(Sendbuff, C2Sdata, MAX_CAR_NUM*sizeof(C2SINFO));
			}
			//printf("%d %d %1.3f %1.3f %1.3f %1.3f %1.3f %1.3f\n", S2Cdata[0].Timestamp, S2Cdata[0].Run_status, C2Sdata[0].r_Desired_ori, C2Sdata[1].r_Desired_ori,
				//C2Sdata[2].r_Desired_ori, C2Sdata[3].r_Desired_ori, C2Sdata[4].r_Desired_ori, C2Sdata[5].r_Desired_ori);
				printf("%d %d %d %.2f %d %d %.2f %d %d %.2f %d %d %.2f %d %d %.2f %d %d %.2f\n", S2Cdata[0].Timestamp, (int)Car_Pos[0].r_x, (int)Car_Pos[0].r_y, Car_Pos[0].r_orientation, (int)Car_Pos[1].r_x, (int)Car_Pos[1].r_y, Car_Pos[1].r_orientation, (int)Car_Pos[2].r_x, (int)Car_Pos[2].r_y, Car_Pos[2].r_orientation,
					(int)Car_Pos[3].r_x, (int)Car_Pos[3].r_y, Car_Pos[3].r_orientation, (int)Car_Pos[4].r_x, (int)Car_Pos[4].r_y, Car_Pos[4].r_orientation, (int)Car_Pos[5].r_x, (int)Car_Pos[5].r_y, Car_Pos[5].r_orientation);
			send(sockClient, Sendbuff, sizeof(Sendbuff), 0);
		}
	}
	closesocket(sockClient);
	WSACleanup();//释放初始化Ws2_32.dll所分配的资源。
	return 0;
}