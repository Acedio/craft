#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "globals.h"
#include "modelmanager.h"
#include "texturemanager.h"
#include "gridmap.h"

enum ObjectType { OBJ_UNIT = 0x10, OBJ_UNIT_WORKER = 0x10, OBJ_RESOURCE = 0x20, OBJ_RESOURCE_TREE = 0x20, OBJ_RESOURCE_GOLD = 0x21 };

class Object{
public:
	Object(ModelManager *modelManager = NULL, TextureManager *textureManager = NULL);
	virtual ~Object();
	virtual void Draw(ModelManager *modelManager, TextureManager *textureManager) = 0;
	virtual void DrawShadow(ModelManager *modelManager) = 0;
	virtual void Update(int frames, GridMap* gridMap, ModelManager *modelManager) = 0;
	ObjectType GetType();
protected:
	ObjectType type;
	bool expired;
	friend class ObjectManager;
};

#endif
