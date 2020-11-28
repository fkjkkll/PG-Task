#pragma once
#include<iostream>
#include<queue>
#include<stack>
#include<vector>
#include <cmath>

#include <opencv2\imgproc\types_c.h>
#include"opencv2/core.hpp"
#include"opencv2/highgui.hpp"
#include"opencv2/imgproc.hpp"
#include"opencv2\opencv.hpp"

#define N 10
#define  PI 3.1415926
#define Domain 8
using namespace std;
using namespace cv;

typedef struct node {
	node(short a, short b) {
		this->x = a;
		this->y = b;
	}
	node() {
		this->x = 0;
		this->y = 0;
	}
	short x;
	short y;
}node;

extern short orient[Domain][2];



uchar** decodeimg(Mat &img);
bool BFS(uchar **&map_initial, short h, short w, node* start, node* object, short **&map_result);
void getpath(short** map_result, node* start, node* object, vector<node*> &path);
double distance(node* a, node* b);
double getangle(node* a, node* b, double l_angle);