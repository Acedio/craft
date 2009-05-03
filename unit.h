#ifndef _UNIT_H_
#define _UNIT_H_

#include "globals.h"
#include "object.h"
#include "texturemanager.h"
#include "modelmanager.h"

class Unit : public Object {
public:
	Unit();
	~Unit();
	virtual void Draw() = 0;
	virtual void MoveTo(VertexF tgt);
	virtual void LookAt(Object* tgt);
	virtual void LookAt(VertexF tgt);
protected:
	PointI lastPos, nextPos;
	float mPercent; // the percent between lastPos and nextPos
	PointI target;
	float angle; // TODO: vertical angle as well?
	TextureRef texture;
	ModelRef model;
};

#endif
