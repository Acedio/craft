#include "globals.h"
#include "object.h"
#include "unit.h"

Unit::Unit(){
	pos.x = 0;
	pos.y = 0;
	mPercent = 0;
	angle = 0;
	target.x = 0;
	target.y = 0;
	model = 0;
	animationName = "stand";
	max_hp = 0;
	cur_hp = 0;
	def = 0;
	atk = 0;
	mov_speed = 0;
	atk_speed = 0;
}

Unit::Unit(PointI p){
	pos = p;
	angle = 0;
	mPercent = 0;
	target.x = 0;
	target.y = 0;
	model = 0;
	animationName = "stand";
	max_hp = 0;
	cur_hp = 0;
	def = 0;
	atk = 0;
	mov_speed = 0;
	atk_speed = 0;
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
