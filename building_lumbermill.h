#ifndef _BUILDING_LUMBERMILL_H_
#define _BUILDING_LUMBERMILL_H_

#include "building.h"

class Building_Lumbermill : public Building {
public:
	Building_Lumbermill(ModelManager* modelManager, TextureManager* textureManager, int x, int y);
	~Building_Lumbermill();
};

#endif
