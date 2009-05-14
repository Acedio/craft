#include "resource_gold.h"

Resource_Gold::Resource_Gold(ModelManager* modelManager, TextureManager *textureManager, int x, int y) : Resource(modelManager,textureManager,x,y){
	type = OBJ_RESOURCE_GOLD;
	model = modelManager->LoadModel("data/resources/gold/gold.mdl", textureManager);
}
