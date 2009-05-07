#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "globals.h"

enum ObjectType { OBJ_UNIT_TEST };

class Object{
public:
	Object();
	virtual ~Object();
	virtual void Draw() = 0;
protected:
	VertexF pos;
	ObjectType type;
	bool expired;
	friend class ObjectManager;
};

#endif
