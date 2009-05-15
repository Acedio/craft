#ifndef _UNIT_CATAPULT_H_
#define _UNIT_CATAPULT_H_

#include "unit.h"

class Unit_Catapult : public Unit {
public:
	Unit_Catapult(ModelManager* modelManager, TextureManager* textureManager, int x, int y);
	~Unit_Catapult();
};

#endif