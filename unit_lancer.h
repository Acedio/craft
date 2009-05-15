#ifndef _UNIT_LANCER_H_
#define _UNIT_LANCER_H_

#include "unit.h"

class Unit_Lancer : public Unit {
public:
	Unit_Lancer(ModelManager* modelManager, TextureManager* textureManager, int x, int y);
	~Unit_Lancer();
};

#endif