#include "globals.h"
#include "object.h"
#include "unit.h"
#include "modelmanager.h"
#include "texturemanager.h"

#include <cmath>
#include <ctime>
#include <cstdlib>

string RandomName()
{
	return names[rand()%NAME_COUNT];
}

Unit::Unit(ModelManager *modelManager, TextureManager *textureManager, int x, int y) : Object(modelManager, textureManager){
	name = RandomName();
	lastPos.x = x;
	lastPos.y = y;
	pos.x = x;
	pos.y = y;
	lastAngle = 0;
	angle = 0;
	dAngle = 0;
	mPercent = 1;
	model = 0;
	animationName = "stand";
	animationInstance = NULL;
	animationChanged = true;
	max_hp = 0;
	cur_hp = 0;
	def = 0;
	atk = 0;
	mov_speed = 0;
	atk_speed = 0;
	cr = 1;
	cg = 1;
	cb = 1;
	moving = false;
}

Unit::~Unit(){
}

void Unit::Update(int ticks, GridMap* gridMap, ModelManager* modelManager){
	if(animationChanged){
		animationChanged = false;
		if(animationInstance != NULL){
			delete animationInstance;
		}
		animationInstance = modelManager->GetAnimationInstance(model, animationName);
	}
	if(moving){
		mPercent += ticks*mov_speed;
		while(mPercent > 1){
			mPercent -= 1;
			lastPos = pos;
			lastAngle = angle;
			if(!moveList.empty()){
				int occupiedTiles = 0;
				while(!moveList.empty() && gridMap->GetObjectRefAt(moveList.front()) != 0){ // while there isn't an empty space found
					occupiedTiles++;
					moveList.pop_front();
				}
				if(!moveList.empty() && occupiedTiles > 0){ // something is in our way
					if(occupiedTiles < MAX_OCCUPIED_TILES){
						list<PointI> prepended = gridMap->AStar(pos, moveList.front());
						if(!prepended.empty()){
							prepended.pop_back(); // we already have prependeds destination in our moveList
							moveList.insert(moveList.begin(),prepended.begin(),prepended.end());
						} else { // there are no other paths to the destination
							moveList.clear();
						}
					} else { // too many things in our current path so we just recalc
						MoveTo(moveList.back(),gridMap);
					}
				}
				if(!moveList.empty()){
					pos = moveList.front();
					gridMap->MoveObject(lastPos,pos);
					moveList.pop_front();
					angle = atan2(-(float)(pos.y-lastPos.y),(float)(pos.x-lastPos.x))+3.14159/2; // y is flipped on the map and 0 degrees is straight down the y axis
					if(angle < 0){
						angle += 2*3.14159;
					}
					dAngle = angle - lastAngle;
					if(dAngle > 3.14159){
						dAngle -= 2*3.14159;
					} else if(dAngle < -3.14159){
						dAngle += 2*3.14159;
					}
				}
			}
		}
		if(moveList.empty() && lastPos == pos){ // we're done moving
			if(animationName != "stand"){
				animationName = "stand";
				animationChanged = true;
			}
			moving = false;
			mPercent = 1;
		}
	}
	animationInstance->AdvanceFrames(ticks);
}

void Unit::Draw(ModelManager *modelManager, TextureManager *textureManager){
	/*glDisable(GL_TEXTURE_2D);
	GLUquadric* q = gluNewQuadric();
	glPushMatrix();
	glTranslatef(pos.x*TILE_SIZE,2,pos.y*TILE_SIZE);
	glColor3f(0,1,0);
	gluSphere(q,.5,8,4);
	glPopMatrix();
	float x = 0;
	for(list<PointI>::iterator i = moveList.begin(); i != moveList.end(); ++i){
		x += 1;
		float color = x/moveList.size();
		glPushMatrix();
		glTranslatef(i->x*TILE_SIZE,2,i->y*TILE_SIZE);
		glColor3f(color,1-color,0);
		gluSphere(q,.5,8,4);
		glPopMatrix();
	}
	gluDeleteQuadric(q);*/
	glPushMatrix();
	glTranslatef(2.5+TILE_SIZE*((float)lastPos.x+(pos.x-lastPos.x)*mPercent),0,2.5+TILE_SIZE*((float)lastPos.y+(pos.y-lastPos.y)*mPercent));
	if(mPercent < .1){
		glRotatef(180.0*(lastAngle + (dAngle)*mPercent*10)/3.14159,0,1,0);
	} else {
		dAngle = 0;
		glRotatef(180.0*angle/3.14159,0,1,0);
	}
	modelManager->DrawModel(model,textureManager,cr,cg,cb,animationInstance);
	glPopMatrix();
}

void Unit::MoveTo(PointI tgt, GridMap* gridMap){
	moveList = gridMap->AStar(pos,tgt);
	if(moveList.empty() && pos == lastPos){ // if we don't have any moves, we aren't moving
		moving = false;
	} else {
		moving = true;
		if(animationName != "walk"){
			animationName = "walk";
			animationChanged = true;
		}
	}
}
