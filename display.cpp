#include <SDL/SDL.h>

#include "display.h"

Display::Display(int width, int height, int bitsperpixel){
	w = width;
	h = height;
	bpp = bitsperpixel;

	// Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	screen = SDL_SetVideoMode(w, h, bpp, SDL_OPENGL);
	SDL_WM_SetCaption("Craft", NULL);

	// Initialize OpenGL
	glClearColor(0,0,0,1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45.0f,1.0f*w/h,1.0f,2000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

Display::~Display(){
	SDL_FreeSurface(screen);

	SDL_Quit();
}
