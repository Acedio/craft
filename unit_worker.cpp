#include "unit_worker.h"
#include <iostream>
using namespace std;

Unit_Worker::Unit_Worker(ModelManager* modelManager, TextureManager *textureManager, int x, int y) : Unit(modelManager,textureManager,x,y){
	type = OBJ_UNIT_WORKER;
	model = modelManager->LoadModel("data/units/worker/worker.mdl", textureManager);
	name += "Worker";
	mov_speed = 0.0012;
	cout << name << endl;
}

Unit_Worker::~Unit_Worker(){
	if(animationInstance != NULL){
		delete animationInstance;
	}
}
