#include "gridmap.h"
#include "objectmanager.h"

GridMap::GridMap(int w, int h, int def = 0){
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
