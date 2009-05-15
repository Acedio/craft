#ifndef _BUILDING_BARRACKS_H_
#define _BUILDING_BARRACKS_H_

#include "building.h"

class Building_Barracks : public Building {
public:
	Building_Barracks(ModelManager* modelManager, TextureManager* textureManager, int x, int y);
	~Building_Barracks();
};

#endif
