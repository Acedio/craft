#include "unit_knight.h"
#include <iostream>
using namespace std;

Unit_Knight::Unit_Knight(ModelManager* modelManager, TextureManager *textureManager, int x, int y) : Unit(modelManager,textureManager,x,y){
	type = OBJ_UNIT_KNIGHT;
	model = modelManager->LoadModel("data/units/knight/knight.mdl", textureManager);
	name += "Knight";
	max_hp = 250;
	cur_hp = max_hp;
	atk = 25;
	def = 30;
	atk_speed = 2000; //milliseconds per attack
	mov_speed = 0.0010;
	cout << name << endl;
}

Unit_Knight::~Unit_Knight(){
	if(animationInstance != NULL){
		delete animationInstance;
	}
}
