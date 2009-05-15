#include "unit_archer.h"
#include <iostream>
using namespace std;

Unit_Archer::Unit_Archer(ModelManager* modelManager, TextureManager *textureManager, int x, int y) : Unit(modelManager,textureManager,x,y){
	type = OBJ_UNIT_ARCHER;
	model = modelManager->LoadModel("data/units/archer/archer.mdl", textureManager);
	name += "Archer";
	max_hp = 120;
	cur_hp = max_hp;
	atk = 10;
	def = 10;
	atk_speed = 1000; //milliseconds per attack
	mov_speed = 0.0022;
	cout << name << endl;
}

Unit_Archer::~Unit_Archer(){
	if(animationInstance != NULL){
		delete animationInstance;
	}
}
