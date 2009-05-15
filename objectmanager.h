#ifndef _OBJECTMANAGER_H_
#define _OBJECTMANAGER_H_

#include <map>
#include <set>
using namespace std;

#include "object.h"
#include "unit.h"
#include "gridmap.h"
#include "input.h"

typedef unsigned int ObjectRef;

class ObjectManager{
public:
	ObjectManager();
	~ObjectManager();

	ObjectRef Add(Object* obj);
	ObjectRef Add(Unit *unit, GridMap *gridMap);
	void AddRef(ObjectRef ref);
	void RemoveRef(ObjectRef ref);
	void UpdateAll(int ticks,GridMap *gridMap, ModelManager *modelManager);
	void DrawObjects(ModelManager *modelManager, TextureManager *textureManager, set<ObjectRef> refs);
	void DrawShadows(ModelManager *modelManager, set<ObjectRef> refs);
	void LoadObjectMap(string mapFileName, GridMap* gridMap, ModelManager *modelManager, TextureManager *textureManager);
	Object* GetObject(ObjectRef ref);
	void HandleClick(VertexF location, ButtonName buttonName, GridMap *gridMap);
private:
	map<ObjectRef,Object*> objects;
	map<ObjectRef,int> refcounts;
	int next_unused_ref;
};

#endif
