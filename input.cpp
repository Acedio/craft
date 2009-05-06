#include <SDL/SDL.h>

#include "input.h"

Mouse::Mouse(){
}

Mouse::~Mouse(){
}

Keyboard::Keyboard(){
	Uint8 *keyboardState = SDL_GetKeyState(&keyCount);
	keys = new KeyState[keyCount];
	for(int key = 0; key < keyCount; key++){
		if(keyboardState[key]){
			keys[key] = KS_DOWN;
		} else {
			keys[key] = KS_UP;
		}
	}
}

Keyboard::~Keyboard(){
	delete keys;
}

void Keyboard::UpdateKeys(){
	for(int key = 0; key < keyCount; key++){
		if(keys[key] == KS_PRESSED){
			keys[key] = KS_DOWN;
		} else if(keys[key] == KS_RELEASED){
			keys[key] = KS_UP;
		}
	}
}

void Keyboard::UpdateKey(KeyName key, KeyState state){
	if(key < keyCount){
		keys[key] = state;
	}
}

Input::Input(){
	windowClosed = false;
}

Input::~Input(){
}

void Input::ProcessInput(){
	keyboard.UpdateKeys(); // Update KS_PRESSED and KS_RELEASED
	SDL_Event e;
	while(SDL_PollEvent(&e)){ // returns 1 if there are new events
		switch(e.type){
			case SDL_QUIT:
				windowClosed = true;
				break;
			case SDL_KEYDOWN:
				//keyboard.UpdateKey(e.key.keysym.sym, KS_PRESSED);
				break;
			case SDL_KEYUP:
				//keyboard.UpdateKey(e.key.keysym.sym, KS_RELEASED);
				break;
			default:
				break;
		}
	}
}

bool Input::WindowClosed(){
	return windowClosed;
}
