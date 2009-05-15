#include "unit_lancer.h"
#include <iostream>
using namespace std;

Unit_Lancer::Unit_Lancer(ModelManager* modelManager, TextureManager *textureManager, int x, int y) : Unit(modelManager,textureManager,x,y){
	type = OBJ_UNIT_LANCER;
	model = modelManager->LoadModel("data/units/lancer/lancer.mdl", textureManager);
	name += "Lancer";
	max_hp = 220;
	cur_hp = max_hp;
	atk = 20;
	def = 20;
	atk_speed = 1500; //milliseconds per attack
	mov_speed = 0.0014;
	cout << name << endl;
}

Unit_Lancer::~Unit_Lancer(){
	if(animationInstance != NULL){
		delete animationInstance;
	}
}
