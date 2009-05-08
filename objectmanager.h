#ifndef _OBJECTMANAGER_H_
#define _OBJECTMANAGER_H_

#include <map>
#include <set>
using namespace std;

#include "object.h"

typedef unsigned int ObjectRef;

class ObjectManager{
public:
	ObjectManager();
	~ObjectManager();

	ObjectRef Add(Object* obj);
	void AddRef(ObjectRef ref);
	void RemoveRef(ObjectRef ref);
	void UpdateAll(int tickss);
	void DrawObjects(set<ObjectRef> refs);
private:
	map<ObjectRef,Object*> objects;
	map<ObjectRef,int> refcounts;
	int next_unused_ref;
};

#endif
