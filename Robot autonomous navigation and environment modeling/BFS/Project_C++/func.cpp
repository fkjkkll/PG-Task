#include "Header.h"

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
	// BFS给地图标记
	while (!q.empty()) {
		curnode = q.front();
		q.pop();
		// 找到目标
		if (curnode->x == object->x && curnode->y == object->y) {
			findit = true;
			break;
		}
		// 对下、右、上、左探索未标记、不是墙的区域加入队列
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



void getpath(short** map_result, node* start, node* object, vector<node*> &path) {
	node* curnode;
	path.push_back(new node(object->x, object->y));	//将目标点放入
	bool findroad;
	curnode = object;
	// 若未抵达起点
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
			cout << "终点有误！" << endl; ////////////////////////
			break;
		}
	}
}


