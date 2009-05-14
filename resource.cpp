#include <cstdlib>

#include "globals.h"
#include "object.h"
#include "resource.h"
#include "modelmanager.h"
#include "texturemanager.h"

Resource::Resource(ModelManager *modelManager, TextureManager *textureManager, int x, int y) : Object(modelManager, textureManager){
	pos.x = x;
	pos.y = y;
	offset.x = 2.0*(float)rand()/RAND_MAX - 1;
	offset.y = 2.0*(float)rand()/RAND_MAX - 1;
	angle = 360.0*((float)rand()/RAND_MAX);
	model = 0;
	amount = 0;
}

void Resource::Draw(ModelManager *modelManager, TextureManager *textureManager){
	glPushMatrix();
	glTranslatef(2.5+TILE_SIZE*(float)pos.x+offset.x,0,2.5+TILE_SIZE*(float)pos.y+offset.y);
	glRotatef(angle,0,1,0);
	modelManager->DrawModel(model,textureManager,1,1,1,NULL,true);
	glPopMatrix();
}

void Resource::DrawShadow(ModelManager *modelManager){
	glPushMatrix();
	glTranslatef(2.5+TILE_SIZE*(float)pos.x+offset.x,0,2.5+TILE_SIZE*(float)pos.y+offset.y);
	glRotatef(angle,0,1,0);
	modelManager->DrawModel(model,NULL,0,0,0,NULL,false);
	glPopMatrix();
}

void Resource::Update(int ticks, GridMap *gridMap, ModelManager* modelManager){
}
