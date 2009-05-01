#include <queue>
#include <stack>
#include <vector>
#include <set>
#include <iostream>
#include <cmath>
using namespace std;

#include "gridmap.h"
#include "objectmanager.h"
#include "globals.h"

GridMap::GridMap(int w, int h, ObjectRef def){
	object_map = new ObjectRef[w*h];
	for(int i = 0; i < w*h; i++){
		object_map[i] = def;
	}
}

GridMap::~GridMap(){
	if(object_map != NULL){
		delete[] object_map;
	}
}

stack<PointI> GridMap::AStar(PointI a, PointI b){
	priority_queue<AStarPoint*,vector<AStarPoint*>,ASPComp> open;
	set<AStarPoint*> closed;
	AStarPoint* start = new AStarPoint;
	start->point = a;
	start->parent = NULL;
	start->d = 0;
	start->rank = 0;
	open.push(start);
	AStarPoint* cur;
	while(!open.empty() && (cur = open.top()->point) != b){
		for(int d = 0; d < 4; ++d){
			AStarPoint* n = new AStarPoint;
			PointI temp;
			temp.x = cur->point.x + 2*(d&1) - 1;
			temp.y = cur->point.y + (d&2) - 1;
			n->point = temp;
			n->d = cur->d+1;
			n->rank = d + abs(b.x-n->point.x) + abs(b.y-n->point.y);
		}
	}
	return stack<PointI>();
}

int main(){
	GridMap g(10,10,0);
	PointI a, b;
	a.x = a.y = 1;
	b.x = b.y = 2;
	g.AStar(a,b);
	return 0;
}
