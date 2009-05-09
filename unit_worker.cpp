#include "unit_worker.h"
#include <iostream>
using namespace std;

Unit_Worker::Unit_Worker(ModelManager* modelManager, TextureManager *textureManager, int x, int y) : Unit(modelManager,textureManager,x,y){
	model = modelManager->LoadModel("data/units/worker/worker.mdl", textureManager);
	animationName = "walk";
	animationInstance = modelManager->GetAnimationInstance(model,animationName);
	name += "Worker";
	cout << name << endl;
}

Unit_Worker::~Unit_Worker(){
	if(animationInstance != NULL){
		delete animationInstance;
	}
}
