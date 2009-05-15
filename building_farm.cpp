#include "building_farm.h"
#include <iostream>
using namespace std;

Building_Farm::Building_Farm(ModelManager* modelManager, TextureManager *textureManager, int x, int y) : Building(modelManager,textureManager,x,y){
	type = OBJ_BUILDING_FARM;
	model = modelManager->LoadModel("data/buildings/farm/farm.mdl", textureManager);

	size.x = 2;
	size.y = 2;
	build_complete = 30000; //milliseconds to complete
	max_hp = 500;
	name = "Farm";
}

Building_Farm::~Building_Farm(){
	
}
