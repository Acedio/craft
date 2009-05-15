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
#include "globals.h"

GridMap::GridMap(){
}

GridMap::GridMap(vector<vector<TileState> > omap){
	object_map = omap;
}

GridMap::GridMap(int w, int h, ObjectRef def){
	for(int y = 0; y < h; y++){
		vector<TileState> temp;
		for(int x = 0; x < w; x++){
			TileState tile;
			tile.objRef = def;
			tile.passType = PT_PASSABLE;
			temp.push_back(tile);
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
	if(PointIsValid(a) && PointIsValid(b) && object_map[b.y][b.x].objRef == 0){
		// Points are valid and no object is at B
		TileState empty;
		empty.objRef = 0;
		empty.passType = PT_EMPTY;
		object_map[b.y][b.x] = object_map[a.y][a.x];
		object_map[a.y][a.x] = empty;
		return true;
	}
	return false;
}

bool GridMap::AddObject(ObjectRef ref, PassType passType, PointI pos, PointI size){
	PointI temp;
	for(temp.y = 0; temp.y < size.y; temp.y++){
		for(temp.x = 0; temp.x < size.x; temp.x++){
			if(!PointIsValid(pos + temp) && object_map[pos.x+temp.x][pos.y+temp.y].objRef == 0){
				return false;
			}
		}
	}
	for(temp.y = 0; temp.y < size.y; temp.y++){
		for(temp.x = 0; temp.x < size.x; temp.x++){
			object_map[pos.y+temp.x][pos.x+temp.y].objRef = ref;
			object_map[pos.y+temp.x][pos.x+temp.y].passType = passType;
		}
	}
	return true;
}

set<ObjectRef> GridMap::GetDrawSet(PointF corners[4]){
	set<ObjectRef> drawSet;
	int height = object_map.size();
	if(height <= 0){
		cout << "Map has zero height!" << endl;
		return drawSet;
	}
	int width = object_map[0].size();
	for(int i = 0; i < 4; i++){
		corners[i].x /= TILE_SIZE;
		corners[i].y /= TILE_SIZE;
		corners[i].x += ((i&1)*2-1)*3;
		corners[i].y += ((i&2)-1)*3;
		if(corners[i].x < 0) corners[i].x = 0;
		if(corners[i].y < 0) corners[i].y = 0;
		if(corners[i].x > width) corners[i].x = width;
		if(corners[i].y > height) corners[i].y = height;
	}
	float leftSquared = pow(corners[2].x-corners[0].x,2) + pow(corners[2].y-corners[0].y,2);
	float rightSquared = pow(corners[3].x-corners[1].x,2) + pow(corners[3].y-corners[1].y,2);
	int rows;
	if(leftSquared > rightSquared){
		rows = 2*sqrt(leftSquared);
	} else {
		rows = 2*sqrt(rightSquared);
	}
	float topSquared = pow(corners[1].x-corners[0].x,2) + pow(corners[1].y-corners[0].y,2);
	float bottomSquared = pow(corners[3].x-corners[2].x,2) + pow(corners[3].y-corners[2].y,2);
	int cols;
	if(topSquared > bottomSquared){
		cols = 2*sqrt(topSquared);
	} else {
		cols = 2*sqrt(bottomSquared);
	}
	PointF lInc,rInc; // amount to increment on the left and right corners per row
	lInc.x = (corners[2].x-corners[0].x)/rows;
	lInc.y = (corners[2].y-corners[0].y)/rows;
	rInc.x = (corners[3].x-corners[1].x)/rows;
	rInc.y = (corners[3].y-corners[1].y)/rows;
	PointF left = corners[0], right = corners[1];
	for(int row = 0; row <= rows; row++){
		PointF p = left;
		PointF inc;
		inc.x = (right.x-left.x)/cols;
		inc.y = (right.y-left.y)/cols;
		for(int col = 0; col <= cols; col++){
			if((int)p.x >= 0 && (int)p.y >= 0 && (int)p.x < width && (int)p.y < height){
				drawSet.insert(object_map[(int)p.y][(int)p.x].objRef);
			} else {
			}
			p = p + inc;
		}
		left = left + lInc;
		right = right + rInc;
	}
	return drawSet;
}

ObjectRef GridMap::GetObjectRefAt(PointI pos){
	if(PointIsValid(pos)){
		return object_map[pos.y][pos.x].objRef;
	}
	return 0;
}

list<PointI> GridMap::AStar(PointI a, PointI b, int stopMask){ // stopMask : what to not walk through
	if(!PointIsValid(a) || !PointIsValid(b) || GetObjectRefAt(b) != 0){ // we're trying to move into a filled space
		return list<PointI>();
	}
	set<AStarPoint*,ASPComp> open;
	set<PointI> closed;
	set<AStarPoint*> toDelete;
	AStarPoint* start = new AStarPoint;
	toDelete.insert(start);
	start->point = a;
	start->parent = NULL;
	start->d = 0;
	start->rank = 0;
	open.insert(start);
	AStarPoint* cur = *(open.begin());
	while(!open.empty() && !(cur->point == b)){
		open.erase(open.begin());
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
			if(PointIsValid(temp) && (object_map[temp.y][temp.x].passType&stopMask) == 0 && closed.find(temp) == closed.end()){
				AStarPoint* n = new AStarPoint;
				toDelete.insert(n);
				n->point = temp;
				n->d = cur->d+1;
				int x = b.x-n->point.x;
				int y = b.y-n->point.y;
				x = x<0?-x:x;
				y = y<0?-y:y;
				// TODO some sort of cross product?
				n->rank = n->d + 2*(x>y?x:y) + ((d&4)>>2); // total distance traveled + manhattan distance w/ unit diagonals
				n->parent = cur;
				set<AStarPoint*,ASPComp>::iterator node;
				for(node = open.begin(); node != open.end(); ++node){
					if((*node)->point == n->point){ // they're the same point, we only want the better one
						if(n->rank < (*node)->rank){ // the new one is better than old
							open.erase(node);
							open.insert(n);
						}
						break;
					}
				}
				if(node == open.end()){
					open.insert(n);
				}
			}
		}
		if(!open.empty()){
			cur = *(open.begin());
		}
	}
	list<PointI> pathList;
	if(cur->point.x == b.x && cur->point.y == b.y){
		while(cur->parent != NULL){
			pathList.push_front(cur->point);
			cur = cur->parent;
		}
	}
	for(set<AStarPoint*>::iterator i = toDelete.begin(); i != toDelete.end(); ++i){
		delete *i;
	}
	return pathList;
}
