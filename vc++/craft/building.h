#ifndef _UNIT_H_
#define _BUILDING_H_

#include "globals.h"
#include "object.h"
#include "texturemanager.h"
#include "modelmanager.h"
#include "gridmap.h"
#include <string>
using namespace std;

class Building : public Object {
public:
	Building();
	Building(PointI p);
	~Building();
	virtual void Draw();
protected:
	PointI pos;
	ModelRef model;
	PointI size;
	int max_hp;
	int cur_hp;
	int defense;
};

#endif