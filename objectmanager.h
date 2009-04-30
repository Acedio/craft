#ifndef _OBJECTMANAGER_H_
#define _OBJECTMANAGER_H_

#include <map>
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
private:
	map<ObjectRef,Object*> objects;
	map<ObjectRef,int> refcounts;
	int next_unused_ref;
};

#endif
