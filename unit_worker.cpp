#include "unit_worker.h"
#include <iostream>
using namespace std;

Unit_Worker::Unit_Worker(ModelManager* modelManager, TextureManager *textureManager, int x, int y) : Unit(modelManager,textureManager,x,y){
	type = OBJ_UNIT_WORKER;
	model = modelManager->LoadModel("data/units/worker/worker.mdl", textureManager);
	name += "Worker";
	max_hp = 80;
	cur_hp = max_hp;
	atk = 4;
	def = 0;
	atk_speed = 1200; //milliseconds per attack
	mov_speed = 0.0012;
	cout << name << endl;
}

Unit_Worker::~Unit_Worker(){
	if(animationInstance != NULL){
		delete animationInstance;
	}
}
