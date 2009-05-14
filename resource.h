#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#include "globals.h"
#include "object.h"
#include "texturemanager.h"
#include "modelmanager.h"
#include "gridmap.h"
using namespace std;

class Resource : public Object {
public:
	Resource(ModelManager* modelManager, TextureManager* textureManager, int x, int y);
	virtual void Draw(ModelManager *modelManager, TextureManager *textureManager);
	virtual void Update(int ticks, GridMap *gridMap, ModelManager* modelManager);
protected:
	PointI pos;
	PointF offset;
	float angle;
	int amount;
	ModelRef model;
	friend class ObjectManager;
};

#endif
