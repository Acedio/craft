#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "globals.h"
#include "modelmanager.h"
#include "texturemanager.h"
#include "gridmap.h"

enum ObjectType {	OBJ_UNIT = 0x10,		OBJ_UNIT_ARCHER = 0x10, 
											OBJ_UNIT_CATAPULT = 0x11, 
											OBJ_UNIT_KNIGHT = 0x12, 
											OBJ_UNIT_LANCER = 0x13, 
											OBJ_UNIT_SWORDSMAN = 0x14, 
											OBJ_UNIT_WORKER = 0x15, 
					OBJ_RESOURCE = 0x20,	OBJ_RESOURCE_GOLD = 0x20, 
											OBJ_RESOURCE_TREE = 0x21, 
					OBJ_BUILDING = 0x30,	OBJ_BUILDING_BARRACKS = 0x30, 
											OBJ_BUILDING_BLACKSMITH = 0x31, 
											OBJ_BUILDING_FARM = 0x32, 
											OBJ_BUILDING_KEEP = 0x33, 
											OBJ_BUILDING_LUMBERMILL = 0x34, 
											OBJ_BUILDING_WALL = 0x35, };

class Object{
public:
	Object(ModelManager *modelManager = NULL, TextureManager *textureManager = NULL);
	virtual ~Object();
	virtual void Draw(ModelManager *modelManager, TextureManager *textureManager) = 0;
	virtual void DrawShadow(ModelManager *modelManager) = 0;
	virtual void Update(int frames, GridMap* gridMap, ModelManager *modelManager) = 0;
	ObjectType GetType();
protected:
	ObjectType type;
	bool expired;
	friend class ObjectManager;
};

#endif
