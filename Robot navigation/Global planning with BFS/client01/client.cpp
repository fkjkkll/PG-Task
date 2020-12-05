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

#pragma comment(lib,"ws2_32.lib")//引用库文件
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
//                              新增变量                              //
//===================================================================//
Mat img;	// 读入图片
Point pt;	// 用于画点画圆
node* start = new node();	// 起点
node* object = new node();	// 终点
uchar **map_initial;	// 读入的图片转化的数组
short **map_result;		// BFS后得到的标记数组
vector<node*> path;		// 得到的路径以一对对坐标表示node(xi,yi)
node* rpos = new node();	// 本地机器人目前位置（我的坐标系）
node* curobject = NULL;		// 终点位置（我的坐标系）
double curangle;			// 当前机器人方向角度
double Zx, Zy;				// 浮点累加，防止 100次INT16(0.1)的和为0
double theta = 0;				// 用于保持本机与服务器机器人位置同步的系数（只附加给服务器机器人）
double gamma = 0;				// 激光雷达防碰撞系数
// 以下是可调节参数
const double gamma_value = 1;				// 参数
const uchar kernel_size = 61;				// 膨胀核大小
const uchar path_filter_size = 20;			// 路径点筛选跨度
const uchar perception_range = 30;			// 机器人感知路径点的半径
const uchar avoid_impact_threshold = 20;	// 机器人采用激光雷达防碰撞最小距离阈值
uchar line_speed = 60;						// 机器人的速度
const double angle_speed_weight = 5;		// 机器人角速度变化量权值



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
		memset(recvBuf, 0, sizeof(recvBuf));//接收数据
		if (recv(sockClient, recvBuf, sizeof(recvBuf), 0)>0){
			memset(&S2Cdata, 0, sizeof(S2CINFO));
			memset(&C2Sdata, 0, sizeof(C2SINFO));
			memcpy(&S2Cdata, recvBuf, sizeof(S2CINFO));
			//下面是接受到的S2Cdata
			{
				if (S2Cdata.Timestamp == 999999){
					Runstatus = 0;
					return 0;
				}
				//第一帧 获得全局路径
				if(S2Cdata.Timestamp == 0){
					memcpy(&Initial_rPos, &S2Cdata.initial_rpose, sizeof(POSE));
					memcpy(&Cur_rPos, &S2Cdata.initial_rpose, sizeof(POSE));
					memcpy(&Cur_dPos, &S2Cdata.initial_dpose, sizeof(POSE));


					//===================================================================//
					//                             读入图片处理                            //
					//===================================================================//
					img = imread("map.jpg");
					cvtColor(img, img, CV_RGB2GRAY);
					threshold(img, img, 200, 255, THRESH_BINARY);
					medianBlur(img, img, 3);
					Mat structure_element = getStructuringElement(MORPH_ELLIPSE, Size(kernel_size, kernel_size)); //定义B的形状及大小
					erode(img, img, structure_element); //膨胀
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
					//                               执行BFS                             //
					//                          根据标记地图得到路径表                      //
					//                  得到的路径以一对对坐标表示node(xi,yi)                //
					//===================================================================//
					if (BFS(map_initial, img.rows, img.cols, start, object, map_result)) {
						// 根据BFS获取路径(xi, yi)
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
					//--------------将路径点集画图--------------//
					for (int i = (int)path.size() - 1; i >= 0; i--) {
						pt.x = path[i]->y;
						pt.y = path[i]->x;
						circle(img, pt, 2, Scalar(100), 3);
					}
				}


				//--------------其他--------------//
				/*if (S2Cdata.task_finish) {
					break;
				}*/
				C2Sdata.Timestamp = S2Cdata.Timestamp;
				memcpy(obstacle, S2Cdata.obstacle, 360 * sizeof(INT16)); //获取激光雷达信息，机器人周围三百六十度障碍信息
				/////////////////////////////////////////////全局轨迹
				//C2Sdata.Traj[0].coor_x = rpos->y; // 绿线
				//C2Sdata.Traj[0].coor_y = rpos->x;
				//C2Sdata.Traj[1].coor_x = Cur_dPos.coor_x;
				//C2Sdata.Traj[1].coor_y = Cur_dPos.coor_y;
				////////////////////////////////////////////////////////////////////绿线位置
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
				//                        将BFS规划的路径拿来当路标                      //
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
				//                    处理本地和Server机器人不同步问题                   //
				//===================================================================//
				double a_server = S2Cdata.right_angle;
				double a_client = getangle(rpos, new node(S2Cdata.initial_dpose.coor_y, S2Cdata.initial_dpose.coor_x), C2Sdata.cur_rpose.coor_ori);
				theta = (a_server - a_client);
				if (fabs(theta) > 0.2) {
					theta = 0;
				}
	

				//===================================================================//
				//                            激光雷达防碰撞                           //
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
				//                               显示图像                              //
				//===================================================================//
				pt.x = rpos->y;
				pt.y = rpos->x;
				circle(img, pt, 15, Scalar(100), 1);
				imshow("result", img);
				waitKey(1);


				//===================================================================//
				//                             路径规划结果                            //
				//===================================================================//
				cur_tra_vel = line_speed; // 绿线
				C2Sdata.tra_vel = line_speed;
				cur_rot_vel  = (curangle + gamma) * angle_speed_weight;
				C2Sdata.rot_vel = (curangle + theta + gamma) * angle_speed_weight;


				//--------------发送数据给server--------------//

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
		printf("初始化Winsock失败");
		return 0;
	}
	SOCKADDR_IN addrSrv;
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8888);//端口号
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//IP地址
	sockClient = socket(AF_INET, SOCK_STREAM, 0);//创建套接字
	if (SOCKET_ERROR == sockClient){
		printf("Socket() error:%d", WSAGetLastError());
		return 0;
	}
	//向服务器发出连接请求
	if (connect(sockClient, (struct  sockaddr*)&addrSrv, sizeof(addrSrv)) == INVALID_SOCKET){
		printf("连接失败:%d", WSAGetLastError());
		return 0;
	}
	HANDLE hThreadRecv = CreateThread(NULL, 0, Recv_Thre, 0, 0, NULL);
	if (NULL == hThreadRecv)
		CloseHandle(hThreadRecv);
	while (Runstatus)
	{
		Sleep(50);
	}
	WSACleanup();//释放初始化Ws2_32.dll所分配的资源。
	return 0;
}
