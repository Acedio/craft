#ifndef _GAME_H_
#define _GAME_H_

#include "texturemanager.h"
#include "display.h"

class Game{
public:
	Game();
	~Game();

	void Run();
private:
	Display *display;
	TextureManager *textureManager;
};

#endif
