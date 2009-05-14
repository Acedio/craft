#include "resource_tree.h"

Resource_Tree::Resource_Tree(ModelManager* modelManager, TextureManager *textureManager, int x, int y) : Resource(modelManager,textureManager,x,y){
	type = OBJ_RESOURCE_TREE;
	model = modelManager->LoadModel("data/resources/trees/trees.mdl", textureManager);
}
