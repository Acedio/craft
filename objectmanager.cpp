#include "objectmanager.h"

ObjectManager::ObjectManager(){
	next_unused_ref = 0;
}

ObjectManager::~ObjectManager(){
}

ObjectRef ObjectManager::Add(Object* obj){
	objects[next_unused_ref] = obj;
	++next_unused_ref;
	return next_unused_ref - 1;
}
