#ifndef _BUILDING_H_
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
	Building(ModelManager* modelManager,TextureManager* textureManager,int x,int y);
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
	int build_time;
	int build_complete;
	string name;
};

#endif