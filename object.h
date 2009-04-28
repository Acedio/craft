#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "globals.h"

class Object{
public:
	Object();
	virtual ~Object();
	virtual void Draw() = 0;
private:
	Vertex3 pos;
};

#endif
