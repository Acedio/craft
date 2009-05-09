#include <iostream>
#include <string>
using namespace std;

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

#include "display.h"

Display::Display(int width, int height, int bitsperpixel) throw (DisplayException){
	w = width;
	h = height;
	bpp = bitsperpixel;

	// Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING | SDL_INIT_NOPARACHUTE);
	screen = SDL_SetVideoMode(w, h, bpp, SDL_OPENGL);
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	if(screen == NULL){
		SDL_Quit();
		throw DisplayException(SDL_GetError());
	}
	SDL_WM_SetCaption("Craft", NULL);

	// Initialize OpenGL
	glClearColor(0,0,0,1);

	glEnable(GL_DEPTH_TEST);

	glShadeModel(GL_SMOOTH);

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
