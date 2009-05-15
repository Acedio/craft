#include "objectmanager.h"
#include "object.h"
#include "unit.h"
#include "units.h"
#include "building.h"
#include "buildings.h"
#include "resource.h"
#include "resources.h"
#include "gridmap.h"
#include "input.h"

#include <set>
#include <map>
#include <iostream>
#include <fstream>
using namespace std;

ObjectManager::ObjectManager(){
	next_unused_ref = 1;
}

void ObjectManager::UpdateAll(int ticks, GridMap *gridMap, ModelManager *modelManager){
	for(map<ObjectRef,Object*>::iterator i = objects.begin(); i != objects.end(); ++i){
		if(i->second != NULL){
			i->second->Update(ticks,gridMap,modelManager);
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

void ObjectManager::DrawShadows(ModelManager *modelManager, set<ObjectRef> refs){
	for(set<ObjectRef>::iterator ref = refs.begin(); ref != refs.end(); ref++){
		if(*ref != 0){ // there is no object 0
			map<ObjectRef,Object*>::iterator obj;
			obj = objects.find(*ref);
			if(obj != objects.end() && obj->second != NULL){
				obj->second->DrawShadow(modelManager);
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
	PointI size;
	size.x = size.y = 1;
	gridMap->AddObject(next_unused_ref,PT_PASSABLE,unit->pos,size);
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

void ObjectManager::LoadObjectMap(string mapFileName, GridMap *gridMap, ModelManager* modelManager, TextureManager *textureManager)
{
	fstream mapFile;
	mapFile.open(mapFileName.c_str(), fstream::in);
	
	if(mapFile.fail()){
		cout << "Error: Could not load object map file \"" << mapFileName << "\"." << endl;
	}

	int width, height;

	mapFile >> width;
	mapFile >> height;

	TileState empty;
	empty.objRef = 0;
	empty.passType = PT_EMPTY;

	vector<vector<TileState> > object_map(height,vector<TileState>(width,empty));

	*gridMap = GridMap(object_map);

	PointI p;
	PointI unitSize;
	unitSize.x = unitSize.y = 1;

	for (p.y = 0; p.y < height; p.y++)
	{
		for (p.x = 0; p.x < width; p.x++)
		{
			string s;
			mapFile >> s;

			switch(s[0])
			{
				case '*': //gold
					gridMap->AddObject(Add(new Resource_Gold(modelManager,textureManager,p.x,p.y)),PT_IMPASSABLE,p,unitSize); 
					break;
				case '2': //player2 start
				case '1': //player1 start
					gridMap->AddObject(Add(new Unit_Worker(modelManager,textureManager,p.x,p.y)),PT_PASSABLE,p,unitSize);
					break;
				case 'b':
					gridMap->AddObject(Add(new Building_Farm(modelManager,textureManager,p.x,p.y)),PT_IMPASSABLE,p,unitSize*2);
					break;
				case '#': //non-walkable (trees for now)
				case '^': //trees
					gridMap->AddObject(Add(new Resource_Tree(modelManager,textureManager,p.x,p.y)),PT_IMPASSABLE,p,unitSize);
					break;
				case '.':
				default:
					break;
			}
		}
	}
	mapFile.close();
}

void ObjectManager::HandleClick(VertexF location, ButtonName buttonName, GridMap *gridMap){
	PointI pos;
	pos.x = (int)(location.x/TILE_SIZE);
	pos.y = (int)(location.z/TILE_SIZE);
	ObjectRef ref = gridMap->GetObjectRefAt(pos);
	/*if(ref != 0){
		map<ObjectRef,Object*>::iterator obj = objects.find(ref);
		if(obj != objects.end()){
			if(obj->second->type&OBJ_UNIT){
				Unit* unit = (Unit*)obj->second;
				cout << unit->name << endl;
			}
		}
	}*/
	/*for(int y = -1; y <= 1; y++){
		for(int x = -1; x <= 1; x++){
			if(x != 0 || y != 0){ // we don't want to check center again
				PointI temp;
				temp.x = (int)location.x + x;
				temp.y = (int)location.y + y;
				if((ref = gridMap->GetObjectRefAt(temp)) != 0){ // there is an object at this point on the map
					map<ObjectRef,Object*>::iterator obj = objects.find(ref);
					if(obj != objects.end()){
						if(obj->second->type&OBJ_UNIT){
							Unit* unit = (Unit*)obj->second;
							if(unit->lastPos == pos){
								cout << unit->name << endl;
							}
						}
					}
				}
			}
		}
	}*/
}
