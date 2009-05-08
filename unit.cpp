#include "globals.h"
#include "object.h"
#include "unit.h"
#include <ctime>
#include <cstdlib>

string RandomName()
{
	srand(time(NULL));

	return names[rand()%nameCount];
}

Unit::Unit(int x, int y){
	name = RandomName();
	pos.x = x;
	pos.y = y;
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

void Unit::Update(int ticks){
}

void Unit::Draw(){
}

void Unit::MoveTo(VertexF tgt){
}

void Unit::LookAt(Object* tgt){
}

void Unit::LookAt(VertexF tgt){
}
