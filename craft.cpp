#include <iostream>
#include <cassert>
using namespace std;

#include "game.h"

int main(){
	Game *game = NULL;
	try{
		game = new Game();
		assert(game != NULL);
		game->Run();
		delete game;
	} catch (GameInitException e){
		cout << "Error initializing game: " << e.what() << endl;
	}
	return 0;
}
