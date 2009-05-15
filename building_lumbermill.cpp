#include "building_lumbermill.h"
#include <iostream>
using namespace std;

Building_Lumbermill::Building_Lumbermill(ModelManager* modelManager, TextureManager *textureManager, int x, int y) : Building(modelManager,textureManager,x,y){
	type = OBJ_BUILDING_LUMBERMILL;
	model = modelManager->LoadModel("data/buildings/lumbermill/lumbermill.mdl", textureManager);

	size.x = 3;
	size.y = 2;
	build_complete = 45000; //milliseconds to complete
	max_hp = 800;
	name = "Lumber Mill";
}

Building_Lumbermill::~Building_Lumbermill(){
	
}
