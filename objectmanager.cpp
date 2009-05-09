#include "objectmanager.h"

#include <set>
#include <map>
#include <iostream>
using namespace std;

ObjectManager::ObjectManager(){
	next_unused_ref = 1;
}

void ObjectManager::UpdateAll(int ticks){
	for(map<ObjectRef,Object*>::iterator i = objects.begin(); i != objects.end(); ++i){
		if(i->second != NULL){
			i->second->Update(ticks);
		}
	}
}

void ObjectManager::DrawObjects(ModelManager *modelManager, TextureManager *textureManager, set<ObjectRef> refs){
	for(set<ObjectRef>::iterator ref = refs.begin(); ref != refs.end(); ref++){
		map<ObjectRef,Object*>::iterator obj;
		obj = objects.find(*ref);
		if(obj != objects.end() && obj->second != NULL){
			obj->second->Draw(modelManager,textureManager);
		} else {
			cout << "Can't find object " << *ref << "." << endl;
		}
	}
}

ObjectManager::~ObjectManager(){
	for(map<ObjectRef,Object*>::iterator i = objects.begin(); i != objects.end(); ++i){
		if(i->second != NULL){
			delete i->second;
		}
	}
}

ObjectRef ObjectManager::Add(Object* obj){
	objects[next_unused_ref] = obj;
	++next_unused_ref;
	return next_unused_ref - 1;
}

void ObjectManager::AddRef(ObjectRef ref){
	// rather than incrementing using the [] notation, we find the refcount then inc, saving us some linear time searching
	map<ObjectRef,int>::iterator refcount;
	if((refcount = refcounts.find(ref)) != refcounts.end()){
		refcount->second += 1;
	}
}

void ObjectManager::RemoveRef(ObjectRef ref){
	map<ObjectRef,int>::iterator refcount;
	if(ref != 0 && (refcount = refcounts.find(ref)) != refcounts.end()){
		refcount->second -= 1;
		map<ObjectRef, Object*>::iterator obj = objects.find(ref);
		if(refcount->second <= 0 && obj->second->expired == true){
			cout << "Object " << ref << " removed." << endl;
			refcounts.erase(refcount);
			objects.erase(ref);
		}
	}
}
