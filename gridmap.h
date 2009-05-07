#ifndef _GRIDMAP_H_
#define _GRIDMAP_H_

#include <list>
#include <stack>
using namespace std;

#include "objectmanager.h"
#include "globals.h"

class AStarPoint{
public:
	bool operator==(AStarPoint* b){return (this->point.x == b->point.x) && (this->point.y == b->point.y);}
	//bool operator<(AStarPoint* b){if(this->point.x == b->point.x) return (this->point.y < b->point.y); else return this->point.x < b->point.x;}
	PointI point;
	AStarPoint* parent;
	int d,rank;
};

class ASPComp{
public:
	bool operator() (AStarPoint* a, AStarPoint* b){return a->rank > b->rank;}
};

class GridMap{
public:
	GridMap(int w, int h, ObjectRef def = 0);
	~GridMap();

	void LoadMap(string mapname);

	void WriteOut();

	stack<PointI> AStar(PointI start, PointI end);
private:
	ObjectRef** object_map;
	int width,height;
};


#endif
