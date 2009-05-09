#include "globals.h"
#include "object.h"
#include "unit.h"
#include "modelmanager.h"
#include "texturemanager.h"

#include <ctime>
#include <cstdlib>

string RandomName()
{
	srand(time(NULL));

	return names[rand()%nameCount];
}

Unit::Unit(ModelManager *modelManager, TextureManager *textureManager, int x, int y) : Object(modelManager, textureManager){
	name = RandomName();
	lastPos.x = x;
	lastPos.y = y;
	pos.x = x;
	pos.y = y;
	angle = 0;
	mPercent = 0;
	model = 0;
	animationName = "";
	animationInstance = NULL;
	max_hp = 0;
	cur_hp = 0;
	def = 0;
	atk = 0;
	mov_speed = 0;
	atk_speed = 0;
	cr = 1;
	cg = 1;
	cb = 1;
}

Unit::~Unit(){
}

void Unit::Update(int ticks){
	animationInstance->AdvanceFrames(ticks);
}

void Unit::Draw(ModelManager *modelManager, TextureManager *textureManager){
	glPushMatrix();
	glTranslatef(TILE_SIZE*((float)pos.x-(pos.x-lastPos.x)*mPercent)+2.5,0,TILE_SIZE*((float)pos.y-(pos.y-lastPos.y)*mPercent)+2.5);
	modelManager->DrawModel(model,textureManager,cr,cg,cb,animationInstance);
	//modelManager->DrawModel(model,textureManager,cr,cg,cb,NULL);
	glPopMatrix();
}

void Unit::MoveTo(VertexF tgt){
}

void Unit::LookAt(Object* tgt){
}

void Unit::LookAt(VertexF tgt){
}
