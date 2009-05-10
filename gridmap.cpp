#include <queue>
#include <list>
#include <vector>
#include <set>
#include <iostream>
#include <cmath>
#include <string>
using namespace std;

#include "gridmap.h"
#include "objectmanager.h"
#include "camera.h"
#include "globals.h"

GridMap::GridMap(){
}

GridMap::GridMap(vector<vector<ObjectRef> > omap){
	object_map = omap;
}

GridMap::GridMap(int w, int h, ObjectRef def){
	for(int y = 0; y < h; y++){
		vector<ObjectRef> temp;
		for(int x = 0; x < w; x++){
			temp.push_back(def);
		}
		object_map.push_back(temp);
	}
}

bool GridMap::PointIsValid(PointI a){
	return (a.x >= 0 && a.y >= 0 && a.y < (int)object_map.size() && a.x < (int)object_map[a.y].size());
}

bool GridMap::MoveObject(PointI a, PointI b){
	// Attempts to move object at point A to point B
	// If an object exists at point B, return false, else return true
	if(PointIsValid(a) && PointIsValid(b) && object_map[b.y][b.x] == 0){
		// Points are valid and no object is at B
		object_map[b.y][b.x] = object_map[a.y][a.x];
		object_map[a.y][a.x] = 0;
		return true;
	}
	return false;
}

bool GridMap::AddObject(ObjectRef ref, PointI pos){
	if(PointIsValid(pos) && object_map[pos.y][pos.x] == 0){
		object_map[pos.y][pos.x] = ref;
		return true;
	}
	return false;
}

set<ObjectRef> GridMap::GetDrawSet(Camera camera){
	set<ObjectRef> drawSet;
	for(vector<vector<ObjectRef> >::iterator row = object_map.begin(); row != object_map.end(); ++row){
		for(vector<ObjectRef>::iterator tile = row->begin(); tile != row->end(); ++tile){
			drawSet.insert(*tile);
		}
	}
	return drawSet;
}

ObjectRef GridMap::GetObjectRefAt(PointI pos){
	if(PointIsValid(pos)){
		return object_map[pos.y][pos.x];
	}
	return 0;
}

list<PointI> GridMap::AStar(PointI a, PointI b){
	if(GetObjectRefAt(b) != 0){ // we're trying to move into a filled space
		return list<PointI>();
	}
	priority_queue<AStarPoint*,vector<AStarPoint*>,ASPComp> open;
	set<PointI> closed;
	vector<AStarPoint*> toDelete;
	AStarPoint* start = new AStarPoint;
	toDelete.push_back(start);
	start->point = a;
	start->parent = NULL;
	start->d = 0;
	start->rank = 0;
	open.push(start);
	AStarPoint* cur = open.top();
	while(!open.empty() && (cur->point.x != b.x || cur->point.y != b.y)){
		open.pop();
		closed.insert(cur->point);
		for(int d = 0; d < 8; ++d){
			PointI temp;
			if(d&4){ // diags
				temp.x = cur->point.x + 2*(d&1)-1;
				temp.y = cur->point.y + (d&2)-1;
			} else { // verts and horis
				temp.x = cur->point.x + ((d&1)?((d&2)-1):0);
				temp.y = cur->point.y + ((d&1)?0:((d&2)-1));
			}
			if(PointIsValid(temp) && object_map[temp.y][temp.x] == 0 && closed.find(temp) == closed.end()){
				AStarPoint* n = new AStarPoint;
				toDelete.push_back(n);
				n->point = temp;
				n->d = cur->d+1;
				int x = b.x-n->point.x;
				int y = b.y-n->point.y;
				x = x<0?-x:x;
				y = y<0?-y:y;
				n->rank = n->d + ((x>y)?x:y); // total distance traveled + manhattan distance w/ unit diagonals (nice heuristic)
				n->parent = cur;
				//cout << temp.x << " " << temp.y << " " << n->rank << endl;
				open.push(n);
			}
		}
		if(!open.empty()){
			cur = open.top();
		}
	}
	list<PointI> pathList;
	if(cur->point.x == b.x && cur->point.y == b.y){
		while(cur->parent != NULL){
			pathList.push_front(cur->point);
			cur = cur->parent;
		}
	}
	for(vector<AStarPoint*>::iterator i = toDelete.begin(); i != toDelete.end(); ++i){
		delete *i;
	}
	return pathList;
}
