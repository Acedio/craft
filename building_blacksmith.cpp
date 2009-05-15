#include "building_blacksmith.h"
#include <iostream>
using namespace std;

Building_Blacksmith::Building_Blacksmith(ModelManager* modelManager, TextureManager *textureManager, int x, int y) : Building(modelManager,textureManager,x,y){
	type = OBJ_BUILDING_BLACKSMITH;
	model = modelManager->LoadModel("data/buildings/blacksmith/blacksmith.mdl", textureManager);

	size.x = 2;
	size.y = 2;
	build_complete = 50000; //milliseconds to complete
	max_hp = 900;
	name = "Blacksmith";
}

Building_Blacksmith::~Building_Blacksmith(){
	
}
