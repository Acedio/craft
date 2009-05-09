#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "globals.h"
#include "modelmanager.h"
#include "texturemanager.h"

enum ObjectType { OBJ_UNIT_WORKER };

class Object{
public:
	Object(ModelManager *modelManager, TextureManager *textureManager);
	virtual ~Object();
	virtual void Draw(ModelManager *modelManager, TextureManager *textureManager) = 0;
	virtual void Update(int frames) = 0;
protected:
	ObjectType type;
	bool expired;
	friend class ObjectManager;
};

#endif
