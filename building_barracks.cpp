#include "building_barracks.h"
#include <iostream>
using namespace std;

Building_Barracks::Building_Barracks(ModelManager* modelManager, TextureManager *textureManager, int x, int y) : Building(modelManager,textureManager,x,y){
	type = OBJ_BUILDING_BARRACKS;
	model = modelManager->LoadModel("data/buildings/barracks/barracks.mdl", textureManager);

	size.x = 3;
	size.y = 3;
	build_complete = 60000; //milliseconds to complete
	max_hp = 1000;
	name = "Barracks";
}

Building_Barracks::~Building_Barracks(){
	
}
