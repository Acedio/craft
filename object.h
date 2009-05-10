#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "globals.h"
#include "modelmanager.h"
#include "texturemanager.h"

enum ObjectType { OBJ_UNIT = 0x10, OBJ_UNIT_WORKER = 0x10 };

class Object{
public:
	Object(ModelManager *modelManager = NULL, TextureManager *textureManager = NULL);
	virtual ~Object();
	virtual void Draw(ModelManager *modelManager, TextureManager *textureManager) = 0;
	virtual void Update(int frames) = 0;
	ObjectType GetType();
protected:
	ObjectType type;
	bool expired;
	friend class ObjectManager;
};

#endif
