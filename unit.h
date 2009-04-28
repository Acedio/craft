#ifndef _UNIT_H_
#define _UNIT_H_

#include "globals.h"
#include "object.h"

class Unit : public Object {
public:
	Unit();
	~Unit();
	virtual void Draw() = 0;
	virtual void MoveTo(Vertex3 tgt);
	virtual void LookAt(Object* tgt);
	virtual void LookAt(Vertex3 tgt);
private:
	Vertex3 vel;
	Vertex3 target;
	float angle; // TODO: vertical angle as well?
};

#endif
