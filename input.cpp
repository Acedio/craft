#include <SDL/SDL.h>
#include <iostream>
using namespace std;

#include "input.h"

void Input::UpdateKeys(){
	for(int key = 0; key < keyCount; key++){
		if(keys[key] == KS_PRESSED){
			keys[key] = KS_DOWN;
		} else if(keys[key] == KS_RELEASED){
			keys[key] = KS_UP;
		}
	}
}

void Input::UpdateKey(KeyName key, KeyState state){
	if(key > 0 && key < keyCount){
		keys[key] = state;
	}
}

void Input::UpdateMouse(){
	for(int button = 0; button < MOUSE_BUTTONS; button++){
		if(mouse.buttons[button] == BS_PRESSED){
			mouse.buttons[button] = BS_DOWN;
		} else if(mouse.buttons[button] == BS_RELEASED) {
			mouse.buttons[button] = BS_UP;
		}
	}
}

void Input::UpdateMouseButton(ButtonName name, ButtonState state){
	if(name > 0 && name < MOUSE_BUTTONS){
		mouse.buttons[name] = state;
	}
}

void Input::UpdateMousePos(int x, int y){
	mouse.pos.x = x;
	mouse.pos.y = y;
}

KeyState Input::GetKeyState(KeyName name){
	if(name > 0 && name < keyCount){
		return keys[name];
	}
	return KS_UP;
}

ButtonState Input::GetMouseButtonState(ButtonName name){
	if(name > 0 && name < MOUSE_BUTTONS){
		return mouse.buttons[name];
	}
	return BS_UP;
}

PointI Input::GetMousePos(){
	return mouse.pos;
}

Input::Input(){
	Uint8 *keyboardState = SDL_GetKeyState(&keyCount);
	keys = new KeyState[keyCount];
	for(int key = 0; key < keyCount; key++){
		if(keyboardState[key]){
			keys[key] = KS_DOWN;
		} else {
			keys[key] = KS_UP;
		}
	}
	int x, y;
	Uint8 bstate = SDL_GetMouseState(&x, &y);
	mouse.buttons[BUTTON_LEFT] = bstate&SDL_BUTTON(1)?BS_DOWN:BS_UP;
	mouse.buttons[BUTTON_MIDDLE] = bstate&SDL_BUTTON(2)?BS_DOWN:BS_UP;
	mouse.buttons[BUTTON_RIGHT] = bstate&SDL_BUTTON(3)?BS_DOWN:BS_UP;
	mouse.buttons[BUTTON_WHEELUP] = BS_UP;
	mouse.buttons[BUTTON_WHEELDOWN] = BS_UP;
	mouse.pos.x = x;
	mouse.pos.y = y;
	windowClosed = false;
}

Input::~Input(){
	delete keys;
}

void Input::ProcessInput(){
	UpdateKeys(); // Update KS_PRESSED and KS_RELEASED
	UpdateMouse();
	SDL_Event e;
	while(SDL_PollEvent(&e)){ // returns 1 if there are new events
		switch(e.type){
			case SDL_QUIT:
				windowClosed = true;
				break;
			case SDL_KEYDOWN:
				UpdateKey((KeyName)e.key.keysym.sym, KS_PRESSED);
				break;
			case SDL_KEYUP:
				UpdateKey((KeyName)e.key.keysym.sym, KS_RELEASED);
				break;
			case SDL_MOUSEBUTTONDOWN:
				UpdateMouseButton((ButtonName)e.button.button, BS_PRESSED);
				break;
			case SDL_MOUSEBUTTONUP:
				UpdateMouseButton((ButtonName)e.button.button, BS_RELEASED);
				break;
			case SDL_MOUSEMOTION:
				UpdateMousePos(e.motion.x, e.motion.y);
				break;
			default:
				break;
		}
	}
}

bool Input::WindowClosed(){
	return windowClosed;
}
