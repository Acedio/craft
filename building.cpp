#include "globals.h"
#include "object.h"
#include "building.h"

Building::Building(ModelManager* modelManager,TextureManager* textureManager,int x,int y){
	pos.x = 0;
	pos.y = 0;
	model = 0;
	build_time = 0;
	max_hp;
	cur_hp = 1;
	defense = 20;
}

Building::Building(PointI p){
	pos = p;
	model = 0;
	build_time = 0;
	max_hp;
	cur_hp = 1;
	defense = 20;

}

Building::~Building(){
}

void Building::Draw(){
}
