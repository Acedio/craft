#ifndef _UNIT_H_
#define _UNIT_H_

#include "globals.h"
#include "object.h"
#include "texturemanager.h"
#include "modelmanager.h"
#include "gridmap.h"
#include <string>
using namespace std;

string const names[] =   {"John the ",
								"Marcus the ",
								"Thomas the ",
								"Bob the ",
								"Maxwell the ",
								"Jerry the ",
								"Joshua the ",
								"Jason the ",
								"Kent Jones the ",
								"Carl the ",
								"Benny the ",
								"Gerald the ",
								"Hairy the ",
								"Manny the ",
								"Decoy the ",
								"Dummyhead the ",
								"Clarence the ",
								"Victor the ",
								"Rodger the ",
								"Donnie the ",
								"Alabaster the ",
								"Vincent the ",
								"Montgomery the ",
								"Maximus the ",
								"Chrono the ",
								"Ness the ",
								"Cloud the ",
								"Jeff the ",
								"Poo the ",
								"Terrence the ",
								"Kevin the ",
								"Scott the ",
								"Kyle the ",
								"Ian the ",
								"James the ",
								"Keegan the ",
								"Cameron the ",
								"Lucas the ",
								"Ninten the ",
								"Claus the ",
								"Flint the ",
								"Duster the ",
								"SKULLCRUSHER the ",
								"t3h 1337 ",
								"Hello! My name is ",
								"Mario the ",
								"Acedio the ",
								"Victavis the ",
								"NULL the ",
								"EOF the ",
								"Dead Beef the ",
								"L u L z the ",
								"B.H. Pirkle the ",
								"Tortimer the ",
								"Mr. Resetti the ",
								"Ace the ",
								"Dizzy the ",
								"Frank the ",
								"Kurt the ",
								"Emery the ",
								"Elliot the ",
								"Matthew the ",
								"David the ",
								"Daniel the ",
								"Michael the ",
								"Giygas the ",
								"Alex the "};

const int NAME_COUNT = 67;

const int MAX_OCCUPIED_TILES = 10; // the maximum number of occupied tiles before just scrapping the path and re-pathing

string RandomName();

class Unit : public Object {
public:
	Unit(ModelManager* modelManager, TextureManager* textureManager, int x, int y);
	~Unit();
	virtual void Draw(ModelManager *modelManager, TextureManager *textureManager);
	virtual void MoveTo(PointI tgt, GridMap *gridMap);
	virtual void Update(int ticks);
	virtual void Update(int ticks, GridMap *gridMap);
protected:
	PointI lastPos;
	PointI pos;
	float mPercent; // the percent between pos and lastPos
	list<PointI> moveList;
	float angle;
	ModelRef model;
	float cr, cg, cb;
	int max_hp;
	int cur_hp;
	int def;
	int atk;
	float mov_speed;
	int atk_speed;
	AnimationInstance *animationInstance;
	string animationName;
	string name;
	friend class ObjectManager;
};

#endif
