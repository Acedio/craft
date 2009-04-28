#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

class Display{
public:
	Display(int width, int height, int bitsperpixel);
	~Display();
private:
	SDL_Surface *screen;
	int w, h, bpp;
};

#endif
