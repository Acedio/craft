#ifndef _UNIT_ARCHER_H_
#define _UNIT_ARCHER_H_

#include "unit.h"

class Unit_Archer : public Unit {
public:
	Unit_Archer(ModelManager* modelManager, TextureManager* textureManager, int x, int y);
	~Unit_Archer();
};

#endif