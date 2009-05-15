#ifndef _UNIT_SWORDSMAN_H_
#define _UNIT_SWORDSMAN_H_

#include "unit.h"

class Unit_Swordsman : public Unit {
public:
	Unit_Swordsman(ModelManager* modelManager, TextureManager* textureManager, int x, int y);
	~Unit_Swordsman();
};

#endif