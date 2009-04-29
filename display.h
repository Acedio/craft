#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <exception>
#include <string>
using namespace std;

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

class DisplayException : public exception{
public:
	DisplayException(char* err){error = err;}
	virtual char* what() throw(){
		return error;
	}
private:
	char* error;
};

class Display{
public:
	Display(int width, int height, int bitsperpixel) throw(DisplayException);
	~Display();
private:
	SDL_Surface *screen;
	int w, h, bpp;
};

#endif
