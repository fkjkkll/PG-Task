#include "Header.h"
#include <fstream>


Mat img;
int main() {
	img = imread("map.jpg");
	resize(img, img, Size(600, 400));
	cvtColor(img, img, CV_RGB2GRAY);
	threshold(img, img, 200, 255, THRESH_BINARY);
	medianBlur(img, img, 3);

	node* start = new node(50, 30);
	node* object = new node(150, 80);

	uchar **map_initial = decodeimg(img);
	short **map_result;

	// 将起点和终点在图片标出
	Point pt;
	pt.x = start->y;
	pt.y = start->x;
	circle(img, pt, 10, Scalar(0));
	pt.x = object->y;
	pt.y = object->x;
	circle(img, pt, 10, Scalar(0));

	// 根据标记地图得到路径表
	vector<node*> path;// 得到的路径以一对对坐标表示node(xi,yi)
	if (BFS(map_initial, img.rows, img.cols, start, object, map_result)) {
		// 根据BFS获取路径(xi, yi)
		getpath(map_result, start, object, path);
	}
	
	// 画图
	for (int i = (int)path.size() - 1; i >= 0; i--) {
		pt.x = path[i]->y;
		pt.y = path[i]->x;
		circle(img, pt, 2, Scalar(100), -1);
		imshow("result", img);
		waitKey(1);
	}
	waitKey(0);

	cout << "END!" << endl;
	return 0;
}