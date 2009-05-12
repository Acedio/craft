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

void Unit::Update(int ticks, GridMap* gridMap){
	mPercent += ticks*mov_speed;
	while(mPercent > 1){
		mPercent -= 1;
		lastPos = pos;
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
			}
		}
	}
	animationInstance->AdvanceFrames(ticks);
}

void Unit::Update(int ticks){
	cout << "Error: Unit: I'm blind, I'm blind! Where is my GridMap??" << endl;
}

void Unit::Draw(ModelManager *modelManager, TextureManager *textureManager){
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslatef(2.5,0,2.5);
	GLUquadric* q = gluNewQuadric();
	glPushMatrix();
	glTranslatef(pos.x*TILE_SIZE,2,pos.y*TILE_SIZE);
	glColor3f(0,1,0);
	gluSphere(q,.5,16,8);
	glPopMatrix();
	for(list<PointI>::iterator i = moveList.begin(); i != moveList.end(); ++i){
		glPushMatrix();
		glTranslatef(i->x*TILE_SIZE,2,i->y*TILE_SIZE);
		glColor3f(0,1,0);
		gluSphere(q,.5,16,8);
		glPopMatrix();
	}
	gluDeleteQuadric(q);
	glTranslatef(TILE_SIZE*((float)lastPos.x+(pos.x-lastPos.x)*mPercent),0,TILE_SIZE*((float)lastPos.y+(pos.y-lastPos.y)*mPercent));
	glRotatef(180.0*angle/3.14159,0,1,0);
	modelManager->DrawModel(model,textureManager,cr,cg,cb,animationInstance);
	glPopMatrix();
}

void Unit::MoveTo(PointI tgt, GridMap* gridMap){
	moveList = gridMap->AStar(pos,tgt);
	if(pos.x == lastPos.x && pos.y == lastPos.y){ // we're already at our current destination TODO this might be risky, not sure if we could ever have the same location twice in one path
		mPercent = 1;
	}
}
