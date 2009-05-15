#include "unit_swordsman.h"
#include <iostream>
using namespace std;

Unit_Swordsman::Unit_Swordsman(ModelManager* modelManager, TextureManager *textureManager, int x, int y) : Unit(modelManager,textureManager,x,y){
	type = OBJ_UNIT_SWORDSMAN;
	model = modelManager->LoadModel("data/units/swordsman/swordsman.mdl", textureManager);
	name += "Swordsman";
	max_hp = 180;
	cur_hp = max_hp;
	atk = 18;
	def = 15;
	atk_speed = 800; //milliseconds per attack
	mov_speed = 0.0016;
	cout << name << endl;
}

Unit_Swordsman::~Unit_Swordsman(){
	if(animationInstance != NULL){
		delete animationInstance;
	}
}
