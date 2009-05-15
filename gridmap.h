#ifndef _GRIDMAP_H_
#define _GRIDMAP_H_

#include <list>
#include <stack>
#include <vector>
#include <set>
#include <iostream>
using namespace std;

typedef unsigned int ObjectRef;

#include "globals.h"
#include "camera.h"

class AStarPoint{
public:
	//bool operator==(AStarPoint* b){return (this->point.x == b->point.x) && (this->point.y == b->point.y);}
	//bool operator<(AStarPoint* b){if(this->point.x == b->point.x) return (this->point.y < b->point.y); else return this->point.x < b->point.x;}
	PointI point;
	AStarPoint* parent;
	int d,rank;
};

class ASPComp{
public:
	bool operator() (AStarPoint* a, AStarPoint* b){if(a->rank == b->rank){return a < b;} else {return a->rank < b->rank;}}
};

enum PassType { PT_EMPTY = 0, PT_PASSABLE = 1, PT_IMPASSABLE = 2 };

struct TileState{
	ObjectRef objRef;
	PassType passType;
};

const float TILE_SIZE = 5;

class GridMap{
public:
	GridMap();
	GridMap(vector<vector<TileState> > omap);
	GridMap(int w, int h, ObjectRef def = 0);

	void WriteOut();

	list<PointI> AStar(PointI start, PointI end, int stopMask = PT_IMPASSABLE);

	bool MoveObject(PointI a, PointI b);

	bool AddObject(ObjectRef ref, PassType passType, PointI pos);

	bool PointIsValid(PointI a);

	set<ObjectRef> GetDrawSet(PointF groundCorners[4]);

	ObjectRef GetObjectRefAt(PointI pos);
private:
	vector<vector<TileState> > object_map;
};


#endif
