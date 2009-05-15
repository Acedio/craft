#ifndef _UNIT_KNIGHT_H_
#define _UNIT_KNIGHT_H_

#include "unit.h"

class Unit_Knight : public Unit {
public:
	Unit_Knight(ModelManager* modelManager, TextureManager* textureManager, int x, int y);
	~Unit_Knight();
};

#endif