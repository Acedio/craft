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
private:
	map<ObjectRef,Object*> objects;
	int next_unused_ref;
};

#endif
