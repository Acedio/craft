#ifndef _BUILDING_KEEP_H_
#define _BUILDING_KEEP_H_

#include "building.h"

class Building_Keep : public Building {
public:
	Building_Keep(ModelManager* modelManager, TextureManager* textureManager, int x, int y);
	~Building_Keep();
};

#endif
