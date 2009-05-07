#ifndef _UNIT_H_
#define _UNIT_H_

#include "globals.h"
#include "object.h"
#include "texturemanager.h"
#include "modelmanager.h"
#include "gridmap.h"
#include <string>
using namespace std;

class Unit : public Object {
public:
	Unit(int x = 0, int y = 0);
	~Unit();
	virtual void Draw();
	virtual void MoveTo(VertexF tgt);
	virtual void LookAt(Object* tgt);
	virtual void LookAt(VertexF tgt);
protected:
	PointI pos;
	float mPercent; // the percent between lastPos and nextPos
	PointI target;
	float angle; // TODO: vertical angle as well?
	ModelRef model;
	int max_hp;
	int cur_hp;
	int def;
	int atk;
	int mov_speed;
	int atk_speed;
	AnimationInstance animationInstance;
	string animationName;
};

#endif
