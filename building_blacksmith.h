#ifndef _BUILDING_BLACKSMITH_H_
#define _BUILDING_BLACKSMITH_H_

#include "building.h"

class Building_Blacksmith : public Building {
public:
	Building_Blacksmith(ModelManager* modelManager, TextureManager* textureManager, int x, int y);
	~Building_Blacksmith();
};

#endif
