#include "globals.h"
#include "object.h"

Object::Object(ModelManager *modelManager, TextureManager *textureManager){
	expired = false;
}

Object::~Object(){
}

ObjectType Object::GetType(){
	return type;
}
