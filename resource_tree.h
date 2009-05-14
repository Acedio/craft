#ifndef _RESOURCE_TREE_H_
#define _RESOURCE_TREE_H_

#include "resource.h"

class Resource_Tree : public Resource {
public:
	Resource_Tree(ModelManager* modelManager, TextureManager* textureManager, int x, int y);
};

#endif
