#include "unit_worker.h"
#include <iostream>
using namespace std;

Unit_Worker::Unit_Worker(ModelManager* modelManager, TextureManager *textureManager) : Unit(modelManager,textureManager){
	model = modelManager->LoadModel("data/units/knight/knight.mdl", textureManager);
	animationName = "walk";
	animationInstance = modelManager->GetAnimationInstance(model,animationName);
	name += "Worker";
}
