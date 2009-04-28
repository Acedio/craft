#ifndef _GAME_H_
#define _GAME_H_

#include "texturemanager.h"
#include "display.h"
#include "input.h"

class Game{
public:
	Game();
	~Game();

	void Run();
private:
	Display *display;
	Input *input;
	TextureManager *textureManager;
};

#endif
