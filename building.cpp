#include "globals.h"
#include "object.h"
#include "building.h"

Building::Building(){
	pos.x = 0;
	pos.y = 0;
	model = 0;
	buildtime = 0;
	max_hp;
	cur_hp = 1;
	defense = 20;
}

Building::Building(PointI p){
	pos = p;
	model = 0;
	buildtime = 0;
	max_hp;
	cur_hp = 1;
	defense = 20;

}

Building::~Building(){
}

void Building::Draw(){
}
