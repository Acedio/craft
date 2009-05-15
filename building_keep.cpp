#include "building_keep.h"
#include <iostream>
using namespace std;

Building_Keep::Building_Keep(ModelManager* modelManager, TextureManager *textureManager, int x, int y) : Building(modelManager,textureManager,x,y){
	type = OBJ_BUILDING_KEEP;
	model = modelManager->LoadModel("data/buildings/keep/keep.mdl", textureManager);

	size.x = 4;
	size.y = 4;
	build_complete = 120000; //milliseconds to complete
	max_hp = 2000;
	name = "Keep";
}

Building_Keep::~Building_Keep(){
	
}
