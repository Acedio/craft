#include "objectmanager.h"
#include "object.h"
#include "unit.h"
#include "unit_lancer.h"
#include "resource.h"
#include "resource_tree.h"
#include "resource_gold.h"
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
	gridMap->AddObject(next_unused_ref,PT_PASSABLE,unit->pos);
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

vector<vector<TileState> > ObjectManager::LoadObjectMap(string mapFileName, ModelManager* modelManager, TextureManager *textureManager)
{
	fstream mapFile;
	mapFile.open(mapFileName.c_str(), fstream::in);
	
	if(mapFile.fail()){
		cout << "Error: Could not load object map file \"" << mapFileName << "\"." << endl;
		return vector<vector<TileState> >();
	}

	int width, height;

	mapFile >> width;
	mapFile >> height;

	vector<vector<TileState> > object_map;

	for (int y = 0; y < height; y++)
	{
		vector<TileState> temp;
		for (int x = 0; x < width; x++)
		{
			string s;
			mapFile >> s;

			TileState tile;
			
			switch(s[0])
			{
				case '*': //gold
					tile.objRef = Add(new Resource_Gold(modelManager,textureManager,x,y)); 
					tile.passType = PT_IMPASSABLE;
					break;
				case '2': //player2 start
				case '1': //player1 start
					tile.objRef = Add(new Unit_Lancer(modelManager,textureManager,x,y));
					tile.passType = PT_PASSABLE;
					break;
				case '#': //non-walkable (trees for now)
				case '^': //trees
					tile.objRef = Add(new Resource_Tree(modelManager,textureManager,x,y));
					tile.passType = PT_IMPASSABLE;
					break;
				case '.':
				default:
					tile.objRef = 0;
					tile.passType = PT_EMPTY;
					break;
			}
			temp.push_back(tile);
		}
		object_map.push_back(temp);
	}
	mapFile.close();

	return object_map;
}

void ObjectManager::HandleClick(VertexF location, ButtonName buttonName, GridMap *gridMap){
	PointI pos;
	pos.x = (int)(location.x/TILE_SIZE);
	pos.y = (int)(location.z/TILE_SIZE);
	ObjectRef ref = gridMap->GetObjectRefAt(pos);
	if(ref != 0){
		map<ObjectRef,Object*>::iterator obj = objects.find(ref);
		if(obj != objects.end()){
			if(obj->second->type&OBJ_UNIT){
				Unit* unit = (Unit*)obj->second;
				cout << unit->name << endl;
			}
		}
	}
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
