#ifndef _BUILDING_WALL_H_
#define _BUILDING_WALL_H_

#include "building.h"

class Building_Wall : public Building {
public:
	Building_Wall(ModelManager* modelManager, TextureManager* textureManager, int x, int y);
	~Building_Wall();
};

#endif
