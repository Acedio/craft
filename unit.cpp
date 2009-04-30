#include "globals.h"
#include "object.h"
#include "unit.h"

Unit::Unit(){
	angle = 0;
	lastPos.x = 0;
	lastPos.y = 0;
	nextPos.x = 0;
	nextPos.y = 0;
	mPercent = 0;
	target.x = 0;
	target.y = 0;
	texture = 0;
}

Unit::~Unit(){
}

void Unit::Draw(){
}

void Unit::MoveTo(VertexF tgt){
}

void Unit::LookAt(Object* tgt){
}

void Unit::LookAt(VertexF tgt){
}
