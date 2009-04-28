#ifndef _GRIDMAP_H_
#define _GRIDMAP_H_

#include "objectmanager.h"

class GridMap{
public:
	GridMap(int w, int h, int def = 0);
	~GridMap();
private:
	ObjectRef* object_map;
};

#endif
