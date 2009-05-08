#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "globals.h"

enum ObjectType { OBJ_UNIT_WORKER };

class Object{
public:
	Object();
	virtual ~Object();
	virtual void Draw() = 0;
	virtual void Update(int frames) = 0;
protected:
	ObjectType type;
	bool expired;
	friend class ObjectManager;
};

#endif
