#include "globals.h"
#include "object.h"
#include "building.h"

#include <iostream>
using namespace std;

Building::Building(ModelManager* modelManager,TextureManager* textureManager,int x,int y){
	pos.x = x;
	pos.y = y;
	model = 0;
	build_time = 0;
	max_hp = 1;
	cur_hp = 1;
	defense = 20;
	cr = 1;
	cg = 1;
	cb = 1;
}

Building::Building(PointI p){
	pos = p;
	model = 0;
	build_time = 0;
	max_hp = 1;
	cur_hp = 1;
	defense = 20;
	cr = 1;
	cg = 1;
	cb = 1;
}

Building::~Building(){
}

void Building::Update(int ticks, GridMap *gridMap, ModelManager* modelManager){
}

void Building::Draw(ModelManager *modelManager, TextureManager *textureManager){
	glPushMatrix();
	//glTranslatef(2.5+(float)(TILE_SIZE*size.x)/2.0,0,2.5+(float)(TILE_SIZE*size.y)/2.0);
	glTranslatef(TILE_SIZE*pos.x,0,TILE_SIZE*pos.y);
	modelManager->DrawModel(model,textureManager,cr,cg,cb,NULL,true); // textured
	glPopMatrix();
}

void Building::DrawShadow(ModelManager *modelManager){
	glPushMatrix();
	//glTranslatef(2.5+(float)(TILE_SIZE*size.x)/2.0,0,2.5+(float)(TILE_SIZE*size.y)/2.0);
	glTranslatef(TILE_SIZE*pos.x,0,TILE_SIZE*pos.y);
	modelManager->DrawModel(model,NULL,0,0,0,NULL,false); // not textured
	glPopMatrix();
}
