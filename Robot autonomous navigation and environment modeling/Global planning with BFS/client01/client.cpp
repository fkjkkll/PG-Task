#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <atlstr.h>
#include "list"
#include "vector"
#include "winsock2.h"
#include "StructData.h"
#include "Header.h"
#include"opencv2/opencv.hpp"

#pragma comment(lib,"ws2_32.lib")//���ÿ��ļ�
using namespace std;
using namespace cv;
SOCKET sockClient;
S2CINFO S2Cdata;
C2SINFO C2Sdata;
POSE Initial_rPos;
POSE Cur_rPos;
POSE Cur_dPos;
INT16 obstacle[360];
int Runstatus = 1;


//===================================================================//
//                              ��������                              //
//===================================================================//
Mat img;	// ����ͼƬ
Point pt;	// ���ڻ��㻭Բ
node* start = new node();	// ���
node* object = new node();	// �յ�
uchar **map_initial;	// �����ͼƬת��������
short **map_result;		// BFS��õ��ı������
vector<node*> path;		// �õ���·����һ�Զ������ʾnode(xi,yi)
node* rpos = new node();	// ���ػ�����Ŀǰλ�ã��ҵ�����ϵ��
node* curobject = NULL;		// �յ�λ�ã��ҵ�����ϵ��
double curangle;			// ��ǰ�����˷���Ƕ�
double Zx, Zy;				// �����ۼӣ���ֹ 100��INT16(0.1)�ĺ�Ϊ0
double theta = 0;				// ���ڱ��ֱ����������������λ��ͬ����ϵ����ֻ���Ӹ������������ˣ�
double gamma = 0;				// �����״����ײϵ��
// �����ǿɵ��ڲ���
const double gamma_value = 1;				// ����
const uchar kernel_size = 61;				// ���ͺ˴�С
const uchar path_filter_size = 20;			// ·����ɸѡ���
const uchar perception_range = 30;			// �����˸�֪·����İ뾶
const uchar avoid_impact_threshold = 20;	// �����˲��ü����״����ײ��С������ֵ
uchar line_speed = 60;						// �����˵��ٶ�
const double angle_speed_weight = 5;		// �����˽��ٶȱ仯��Ȩֵ



double cur_tra_vel = 0;
double cur_rot_vel = 0;
void Initialition(){
	memset(&S2Cdata, 0, sizeof(S2CINFO));
	memset(&C2Sdata, 0, sizeof(C2SINFO));
	memset(&Initial_rPos, 0, sizeof(POSE));
	memset(&Cur_rPos, 0, sizeof(POSE));
	memset(&Cur_dPos, 0, sizeof(POSE));
	memset(obstacle, 0, 360 * sizeof(INT16));
}
double abs_ori(double ori, double relative_ori){
	double absori = ori + relative_ori;
	if (absori < 0)
		absori += 2 * PI;
	else if (absori >= 2 * PI)
		absori -= 2 * PI;
	return absori;
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
DWORD WINAPI Recv_Thre(LPVOID lpParameter)
{
	char recvBuf[800];//Receive Data
	char Sendbuff[600];//Send Data
	while (true){
		memset(recvBuf, 0, sizeof(recvBuf));//��������
		if (recv(sockClient, recvBuf, sizeof(recvBuf), 0)>0){
			memset(&S2Cdata, 0, sizeof(S2CINFO));
			memset(&C2Sdata, 0, sizeof(C2SINFO));
			memcpy(&S2Cdata, recvBuf, sizeof(S2CINFO));
			//�����ǽ��ܵ���S2Cdata
			{
				if (S2Cdata.Timestamp == 999999){
					Runstatus = 0;
					return 0;
				}
				//��һ֡ ���ȫ��·��
				if(S2Cdata.Timestamp == 0){
					memcpy(&Initial_rPos, &S2Cdata.initial_rpose, sizeof(POSE));
					memcpy(&Cur_rPos, &S2Cdata.initial_rpose, sizeof(POSE));
					memcpy(&Cur_dPos, &S2Cdata.initial_dpose, sizeof(POSE));


					//===================================================================//
					//                             ����ͼƬ����                            //
					//===================================================================//
					img = imread("map.jpg");
					cvtColor(img, img, CV_RGB2GRAY);
					threshold(img, img, 200, 255, THRESH_BINARY);
					medianBlur(img, img, 3);
					Mat structure_element = getStructuringElement(MORPH_ELLIPSE, Size(kernel_size, kernel_size)); //����B����״����С
					erode(img, img, structure_element); //����
					start->x = S2Cdata.initial_rpose.coor_y;
					start->y = S2Cdata.initial_rpose.coor_x;
					rpos->x = S2Cdata.initial_rpose.coor_y;
					rpos->y = S2Cdata.initial_rpose.coor_x;
					object->x = S2Cdata.initial_dpose.coor_y;
					object->y = S2Cdata.initial_dpose.coor_x;
					Point pt;
					pt.x = start->y;
					pt.y = start->x;
					circle(img, pt, kernel_size/2, Scalar(200), -1);
					pt.x = object->y;
					pt.y = object->x;
					circle(img, pt, kernel_size/2, Scalar(200), -1);
					map_initial = decodeimg(img);
					//===================================================================//
					//                               ִ��BFS                             //
					//                          ���ݱ�ǵ�ͼ�õ�·����                      //
					//                  �õ���·����һ�Զ������ʾnode(xi,yi)                //
					//===================================================================//
					if (BFS(map_initial, img.rows, img.cols, start, object, map_result)) {
						// ����BFS��ȡ·��(xi, yi)
						getpath(map_result, start, object, path);
					}
					vector<node*> temp(path);
					path.clear();
					path.push_back(temp[0]);
					for (int i = 1; i < (int)temp.size()-1; i++) {
						if (i % path_filter_size == 0) {
							path.push_back(temp[i]);
						}
					}
					path.push_back(temp[temp.size()-1]);
					cout<<"Path Get"<<endl;//////////////
					//--------------��·���㼯��ͼ--------------//
					for (int i = (int)path.size() - 1; i >= 0; i--) {
						pt.x = path[i]->y;
						pt.y = path[i]->x;
						circle(img, pt, 2, Scalar(100), 3);
					}
				}


				//--------------����--------------//
				/*if (S2Cdata.task_finish) {
					break;
				}*/
				C2Sdata.Timestamp = S2Cdata.Timestamp;
				memcpy(obstacle, S2Cdata.obstacle, 360 * sizeof(INT16)); //��ȡ�����״���Ϣ����������Χ������ʮ���ϰ���Ϣ
				/////////////////////////////////////////////ȫ�ֹ켣
				//C2Sdata.Traj[0].coor_x = rpos->y; // ����
				//C2Sdata.Traj[0].coor_y = rpos->x;
				//C2Sdata.Traj[1].coor_x = Cur_dPos.coor_x;
				//C2Sdata.Traj[1].coor_y = Cur_dPos.coor_y;
				////////////////////////////////////////////////////////////////////����λ��
				Zx = Cur_rPos.coor_x + cur_tra_vel * 0.2*cos(Cur_rPos.coor_ori);
				Zy = Cur_rPos.coor_y + cur_tra_vel * 0.2*sin(Cur_rPos.coor_ori);
				Cur_rPos.coor_ori = Cur_rPos.coor_ori + cur_rot_vel * 0.2;
				Cur_rPos.coor_x = INT16(Zx + 0.5);
				Cur_rPos.coor_y = INT16(Zy + 0.5);
				rpos->y = Cur_rPos.coor_x;
				rpos->x = Cur_rPos.coor_y;
				C2Sdata.cur_rpose.coor_x = Cur_rPos.coor_x;
				C2Sdata.cur_rpose.coor_y = Cur_rPos.coor_y;
				C2Sdata.cur_rpose.coor_ori = Cur_rPos.coor_ori;
				

				//===================================================================//
				//                        ��BFS�滮��·��������·��                      //
				//===================================================================//
				if (!path.empty()) {
					curobject = path.back();
					while (distance(curobject, rpos) < perception_range) {
						if (curobject->x == S2Cdata.initial_dpose.coor_y && curobject->y == S2Cdata.initial_dpose.coor_x) {
							break;
						}
						path.pop_back();
						curobject = path.back();
					}
					curangle = getangle(rpos, curobject, Cur_rPos.coor_ori);
				}


				//===================================================================//
				//                    �����غ�Server�����˲�ͬ������                   //
				//===================================================================//
				double a_server = S2Cdata.right_angle;
				double a_client = getangle(rpos, new node(S2Cdata.initial_dpose.coor_y, S2Cdata.initial_dpose.coor_x), C2Sdata.cur_rpose.coor_ori);
				theta = (a_server - a_client);
				if (fabs(theta) > 0.2) {
					theta = 0;
				}
	

				//===================================================================//
				//                            �����״����ײ                           //
				//===================================================================//
				double distance = 10000;
				int index = -1;
				for (int i = 0; i < 360; i++) {
					if (i <= 90 || i>=270) {
						if (obstacle[i] < distance) {
							distance = obstacle[i];
							index = i;
						}
					}
				}
				if (S2Cdata.Timestamp && distance < avoid_impact_threshold) {
					if (index < 180) {
						gamma = -gamma_value * (1.0 / distance) * avoid_impact_threshold;
					}
					else {
						gamma = gamma_value * (1.0/ distance) * avoid_impact_threshold;
					}
				}
				else {
					gamma = 0;
				}


				//===================================================================//
				//                               ��ʾͼ��                              //
				//===================================================================//
				pt.x = rpos->y;
				pt.y = rpos->x;
				circle(img, pt, 15, Scalar(100), 1);
				imshow("result", img);
				waitKey(1);


				//===================================================================//
				//                             ·���滮���                            //
				//===================================================================//
				cur_tra_vel = line_speed; // ����
				C2Sdata.tra_vel = line_speed;
				cur_rot_vel  = (curangle + gamma) * angle_speed_weight;
				C2Sdata.rot_vel = (curangle + theta + gamma) * angle_speed_weight;


				//--------------�������ݸ�server--------------//

				memset(Sendbuff, 0, sizeof(Sendbuff));
				memcpy(Sendbuff, &C2Sdata, sizeof(C2SINFO));
				send(sockClient, Sendbuff, sizeof(Sendbuff), 0);
				Sleep(30);
			}
		}
		else
			Sleep(1000);
	}
	closesocket(sockClient);
	return 0;
}
int main()
{
	CString path = GetExePath() + "Server.exe";
	ShellExecute(NULL, NULL, path, NULL, NULL, SW_SHOW);
	Sleep(1000);
	Initialition();
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("��ʼ��Winsockʧ��");
		return 0;
	}
	SOCKADDR_IN addrSrv;
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8888);//�˿ں�
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//IP��ַ
	sockClient = socket(AF_INET, SOCK_STREAM, 0);//�����׽���
	if (SOCKET_ERROR == sockClient){
		printf("Socket() error:%d", WSAGetLastError());
		return 0;
	}
	//�������������������
	if (connect(sockClient, (struct  sockaddr*)&addrSrv, sizeof(addrSrv)) == INVALID_SOCKET){
		printf("����ʧ��:%d", WSAGetLastError());
		return 0;
	}
	HANDLE hThreadRecv = CreateThread(NULL, 0, Recv_Thre, 0, 0, NULL);
	if (NULL == hThreadRecv)
		CloseHandle(hThreadRecv);
	while (Runstatus)
	{
		Sleep(50);
	}
	WSACleanup();//�ͷų�ʼ��Ws2_32.dll���������Դ��
	return 0;
}
