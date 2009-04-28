#include <SDL/SDL.h>

#include "input.h"

Mouse::Mouse(){
}

Mouse::~Mouse(){
}

Keyboard::Keyboard(){
}

Keyboard::~Keyboard(){
}

Input::Input(){
	windowClosed = false;
}

Input::~Input(){
}

void Input::ProcessInput(){
	SDL_Event e;
	while(SDL_PollEvent(&e)){ // returns 1 if there are new events
		switch(e.type){
			case SDL_QUIT:
				windowClosed = true;
				break;
			default:
				break;
		}
	}
}

bool Input::WindowClosed(){
	return windowClosed;
}
