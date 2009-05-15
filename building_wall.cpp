#include "building_wall.h"
#include <iostream>
using namespace std;

Building_Wall::Building_Wall(ModelManager* modelManager, TextureManager *textureManager, int x, int y) : Building(modelManager,textureManager,x,y){
	type = OBJ_BUILDING_WALL;
	model = modelManager->LoadModel("data/buildings/wall/wall.mdl", textureManager);

	size.x = 1;
	size.y = 1;
	build_complete = 20000; //milliseconds to complete
	max_hp = 600;
	name = "Wall";
}

Building_Wall::~Building_Wall(){
	
}
