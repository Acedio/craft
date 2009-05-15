#ifndef _BUILDING_FARM_H_
#define _BUILDING_FARM_H_

#include "building.h"

class Building_Farm : public Building {
public:
	Building_Farm(ModelManager* modelManager, TextureManager* textureManager, int x, int y);
	~Building_Farm();
};

#endif
