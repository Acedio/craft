#ifndef _GRIDMAP_H_
#define _GRIDMAP_H_

#include <list>

#include "objectmanager.h"
#include "globals.h"

struct AStarPoint{
	PointI point;
	PointI* parent;
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

	stack<PointI> AStar(PointI start, PointI end);
private:
	ObjectRef* object_map;
};

#endif
