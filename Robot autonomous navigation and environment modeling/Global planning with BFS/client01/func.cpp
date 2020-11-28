#include "Header.h"

//===================================================================//
//                               8-����                              //
//===================================================================//
short orient[Domain][2] = {
	{1, 0},
	{0, 1},
	{-1, 0},
	{0, -1},

	{1, 1},
	{-1, 1},
	{-1, -1},
	{1, -1},
};


//===================================================================//
//����opencv�ж�ȡ����ͼ����Ϊuchar�Ķ�ά������ʽ���������������һȦ0��ǽ�ڣ�//
//===================================================================//
uchar** decodeimg(Mat &img) {
	short w = img.cols;
	short h = img.rows;
	uchar **map_initial = new uchar* [h+2];
	for (int i = 0; i < h + 2; i++) {
		map_initial[i] = new uchar[w + 2];
		for (int j = 0; j < w + 2; j++) {
			map_initial[i][j] = 0;
		}
	}
	for (int i = 0; i < h; i++) {
		uchar *rowdata = img.ptr(i);
		for (int j = 0; j < w; j++) {
			map_initial[i + 1][j + 1] = rowdata[j];
		}
	}
	return map_initial;
}


//===================================================================//
//                                BFS                                //
//===================================================================//
bool BFS(uchar **&map_initial, short h, short w, node* start, node* object, short **&map_result) {
	bool findit = false;
	queue<node*> q;
	node* curnode;
	uchar **map_arrived = new uchar*[h + 2];
	map_result = new short*[h + 2];
	for (int i = 0; i < h + 2; i++) {
		map_arrived[i] = new uchar[w + 2];
		map_result[i] = new short[w + 2];
		for (int j = 0; j < w + 2; j++) {
			map_arrived[i][j] = 0;
			map_result[i][j] = 0;
		}
	}
	q.push(start);
	map_arrived[start->x][start->y] = 1;
	// BFS����ͼ���
	while (!q.empty()) {
		curnode = q.front();
		q.pop();
		// �ҵ�Ŀ��
		if (curnode->x == object->x && curnode->y == object->y) {
			findit = true;
			break;
		}
		// ���¡��ҡ��ϡ���̽��δ��ǡ�����ǽ������������
		for (int i = 0; i < Domain; i++) {
			if (map_initial[curnode->x + orient[i][0]][curnode->y + orient[i][1]] && !map_arrived[curnode->x + orient[i][0]][curnode->y + orient[i][1]]) {
				q.push(new node(curnode->x + orient[i][0], curnode->y + orient[i][1]));
				map_arrived[curnode->x + orient[i][0]][curnode->y + orient[i][1]] = 1;
				map_result[curnode->x + orient[i][0]][curnode->y + orient[i][1]] = map_result[curnode->x][curnode->y] + 1;
			}
		}
	}
	cout << "BFS Done" << endl;
	return true;
}


//===================================================================//
//                  ����BFS�õ��ı��ͼ�������·��                       //
//===================================================================//
void getpath(short** map_result, node* start, node* object, vector<node*> &path) {
	node* curnode;
	path.push_back(new node(object->x, object->y));	//��Ŀ������
	bool findroad;
	curnode = object;
	// ��δ�ִ����
	while (curnode->x != start->x || curnode->y != start->y) {
		findroad = false;
		for (int i = 0; i < Domain; i++) {
			if (map_result[curnode->x + orient[i][0]][curnode->y + orient[i][1]] == map_result[curnode->x][curnode->y] - 1) {
				findroad = true;
				path.push_back(new node(curnode->x + orient[i][0], curnode->y + orient[i][1]));
				curnode->x = curnode->x + orient[i][0];
				curnode->y = curnode->y + orient[i][1];
				break;
			}
		}
		if (!findroad) {
			cout << "�յ�����" << endl; ////////////////////////
			break;
		}
	}
}



double distance(node* a, node* b) {
	return sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2));
}


//===================================================================//
//      ���ݵ�ǰλ�á�Ŀ��λ�úͻ����˵�ǰ�Ƕ�ȷ����һʱ�̽��ٶȱ仯��         //
//===================================================================//
double getangle(node* a, node* b, double l_angle) {
	short x1 = a->y;
	short y1 = a->x;
	short x2 = b->y;
	short y2 = b->x;
	double numerator = y2 - y1;
	double denominator = x2 - x1 + 0.00000001;
	double rst = 0;
	double rst2 = 0;
	if (denominator >= 0) {
		rst = atan(numerator / denominator);
	}
	else {
		if (numerator >= 0) {
			rst = atan(numerator / denominator) + PI;
		}
		else {
			rst = atan(numerator / denominator) - PI;
		}
	}
	rst2 = rst - l_angle;
	if (fabs(rst2) > PI) {
		if (rst2 > 0) {
			rst2 -= 2 * PI;
		}
		else {
			rst2 += 2 * PI;
		}
	}
	return rst2;
}

