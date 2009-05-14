#ifndef _RESOURCE_GOLD_H_
#define _RESOURCE_GOLD_H_

#include "resource.h"

class Resource_Gold : public Resource {
public:
	Resource_Gold(ModelManager* modelManager, TextureManager* textureManager, int x, int y);
};

#endif
