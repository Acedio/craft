#ifndef _UNIT_WORKER_H_
#define _UNIT_WORKER_H_

#include "unit.h"

class Unit_Worker : public Unit {
public:
	Unit_Worker(ModelManager* modelManager, TextureManager* textureManager);
	~Unit_Worker();
};

#endif
