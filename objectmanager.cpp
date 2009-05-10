#include "objectmanager.h"
#include "object.h"
#include "gridmap.h"

#include <set>
#include <map>
#include <iostream>
#include <fstream>
using namespace std;

ObjectManager::ObjectManager(){
	next_unused_ref = 1;
}

void ObjectManager::UpdateAll(int ticks, GridMap *gridMap){
	for(map<ObjectRef,Object*>::iterator i = objects.begin(); i != objects.end(); ++i){
		if(i->second != NULL){
			if(i->second->type&OBJ_UNIT){
				((Unit*)i->second)->Update(ticks,gridMap);
			} else {
				i->second->Update(ticks);
			}
		}
	}
}

void ObjectManager::DrawObjects(ModelManager *modelManager, TextureManager *textureManager, set<ObjectRef> refs){
	for(set<ObjectRef>::iterator ref = refs.begin(); ref != refs.end(); ref++){
		if(*ref != 0){ // there is no object 0
			map<ObjectRef,Object*>::iterator obj;
			obj = objects.find(*ref);
			if(obj != objects.end() && obj->second != NULL){
				obj->second->Draw(modelManager,textureManager);
			} else {
				cout << "Can't find object " << *ref << "." << endl;
			}
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

ObjectRef ObjectManager::Add(Unit *unit, GridMap *gridMap){
	objects[next_unused_ref] = unit;
	gridMap->AddObject(next_unused_ref,unit->pos);
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

Object* ObjectManager::GetObject(ObjectRef ref){
	map<ObjectRef,Object*>::iterator obj = objects.find(ref);
	if(obj != objects.end()){
		return obj->second;
	}
	return NULL;
}

vector<vector<ObjectRef> > ObjectManager::LoadObjectMap(string mapFileName)
{
	fstream mapFile;
	mapFile.open(mapFileName.c_str(), fstream::in);

	int width, height;

	mapFile >> width;
	mapFile >> height;

	vector<vector<ObjectRef> > object_map;
	
	for (int y = 0; y < height; y++)
	{
		vector<ObjectRef> temp;
		for (int x = 0; x < width; x++)
		{
			string s;
			mapFile >> s;
			
			switch(s[0])
			{
				/*case '.': //walkable
					temp.push_back(0);
					break;
				case '#': //non-walkable (trees for now)
				case '^': //trees
					break;
				case '*': //gold
					break;
				case '1': //player1 start
					break;
				case '2': //player2 start
					break;*/
				default:
					temp.push_back(0);
					break;
			}
		}
		object_map.push_back(temp);
	}
	mapFile.close();

	return object_map;
}
