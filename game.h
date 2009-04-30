#ifndef _GAME_H_
#define _GAME_H_

#include "texturemanager.h"
#include "display.h"
#include "input.h"
#include "camera.h"

class GameInitException : public exception{
public:
	GameInitException(const char* err){error = err;}
	virtual const char* what() throw(){
		return error;
	}
private:
	const char* error;
};

class Game{
public:
	Game() throw(GameInitException);
	~Game();

	void Run();
private:
	Display *display;
	Input *input;
	TextureManager *textureManager;
	Camera *camera;
};

#endif
