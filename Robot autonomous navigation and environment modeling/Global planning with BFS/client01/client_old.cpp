#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <atlstr.h>
#include "list"
#include "vector"
#include "winsock2.h"
#pragma comment(lib,"ws2_32.lib")//���ÿ��ļ�
using namespace std;
#define MAX_CAR_NUM 6
#define PI 3.1415926
int Run_status = 1;
int Node_Num[MAX_CAR_NUM];
typedef struct{
	int node_id;					/*�������*/
	POINT node_coor;				/*�����*/
	int node_flag;                  //��ʾ���ȱ��ĸ������˷��ʣ�
	bool Be_seen[MAX_CAR_NUM];      //�ڵ�����������˵Ŀɼ���
	int branch_num;
	int around_node_id[4];//���ڽڵ���
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
	int Timestamp;                  //ʱ���
	int Run_status;
	int Target_ID;			        //Ŀ��ID
	int Source_ID;			        //ԴID,��ServerʱΪת����Ϣ��Info_Type��������Ч
	CAR_POSE r_Pos;                 //������λ�ˣ���Orientation��Ч
	double r_Relative_Dis;			//�����������������һ�ڵ㣩
	double r_Relative_Ori;			//��������̬�����ȣ�
	double r_Road_Ori;				//��·���򣨻��ȣ�
	NODE_PUSH Node_push;            //�ڵ�������Ϣ
	bool CanComm[MAX_CAR_NUM];      //��ͨѶ�Ļ�����///////////
	int ReqData_Type[MAX_CAR_NUM];  //����:0:������, 1:������
	int AnsData_Type[MAX_CAR_NUM];  //�𸴣�1��ͬ������2���ܾ�����
	int Target_Node_ID[MAX_CAR_NUM];//Ͷ��ڵ����
	int Cost[MAX_CAR_NUM];          //�ɱ�
}S2CINFO;
typedef struct{
	int Timestamp;                  //ʱ���
	int Target_ID;			        //Ŀ��ID
	int Source_ID;			        //ԴID
	CAR_POSE r_Pos;                 //����������λ��,(x,y,orientaition����Ч)
	int ReqData_Type;               //����0:������1��������
	int AnsData_Type[MAX_CAR_NUM];  //�𸴣�1��ͬ������2���ܾ�����
	double r_Desired_ori;			//��֧����ѡ�񣨽ڵ㴦��
	int Target_Node_ID;		        //Ͷ��ڵ����
	int Cost;                       //�ɱ�
	double r_linear_vel;            //���ٶ� cm/s
	double r_angular_vel;           //���ٶ� Rad/s
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
	if (S2Cdata[Car_num].Node_push.Valid){//�����µĽڵ㣬����
		if (Node_Num[Car_num] == 0){//��һ���ڵ㣬�ڵ��������������˵ĳ�ʼ����
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
		else{//�Ѿ���һ���ڵ�
			if (S2Cdata[Car_num].r_Relative_Dis>20){//�½ڵ�
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
	if (S2Cdata[Car_num].Node_push.Valid){ //����ڵ�
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
	printf("���ȵ��MC_Server������-����Զ�̣������������ַ������س���������");
	//while (getchar() != '\n')
		//continue;
	cin >> buffs;
	//�����׽���
	Initialition();
	WSADATA wsaData;
	char Recvbuff[1400];
	char Sendbuff[1024];
	memset(Recvbuff, 0, sizeof(Recvbuff));
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("��ʼ��Winsockʧ��");
		return 0;
	}

	SOCKADDR_IN addrSrv;
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8888);//�˿ں�
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//IP��ַ
	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);//�����׽���
	if (SOCKET_ERROR == sockClient){
		printf("Socket() error:%d", WSAGetLastError());
		return 0;
	}
	//�������������������
	if (connect(sockClient, (struct  sockaddr*)&addrSrv, sizeof(addrSrv)) == INVALID_SOCKET){
		printf("����ʧ��:%d", WSAGetLastError());
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
			if (S2Cdata[0].Timestamp == 0){//��һ֡
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
	WSACleanup();//�ͷų�ʼ��Ws2_32.dll���������Դ��
	return 0;
}