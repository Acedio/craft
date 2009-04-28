#ifndef _INPUT_H_
#define _INPUT_H_

#include <SDL/SDL.h>

class Keyboard{
public:
	Keyboard();
	~Keyboard();
private:
};

class Mouse{
public:
	Mouse();
	~Mouse();
private:
};

class Input{
public:
	Input();
	~Input();

	void ProcessInput();

	bool WindowClosed();

	Keyboard keyboard;
	Mouse mouse;
private:
	bool windowClosed;
};

#endif
