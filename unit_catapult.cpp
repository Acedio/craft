#include "unit_catapult.h"
#include <iostream>
using namespace std;

Unit_Catapult::Unit_Catapult(ModelManager* modelManager, TextureManager *textureManager, int x, int y) : Unit(modelManager,textureManager,x,y){
	type = OBJ_UNIT_CATAPULT;
	model = modelManager->LoadModel("data/units/catapult/catapult.mdl", textureManager);
	name += "Catapult";
	max_hp = 200;
	cur_hp = max_hp;
	atk = 30;
	def = 20;
	atk_speed = 5000; //milliseconds per attack
	mov_speed = 0.0006;
	cout << name << endl;
}

Unit_Catapult::~Unit_Catapult(){
	if(animationInstance != NULL){
		delete animationInstance;
	}
}
