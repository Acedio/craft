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
#include "globals.h"

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

void Display::ScreenToWorld(PointI cursorScreenPos, VertexF *cursorPos, PointF *upperLeft, PointF *dimensions){
	GLdouble modelview[16];
	GLdouble projection[16];
	GLint viewport[4];

	glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
	glGetDoublev(GL_PROJECTION_MATRIX,projection);
	glGetIntegerv(GL_VIEWPORT,viewport);

	GLfloat z;
	// TODO this is, sadly, HOLY CRAP slow. It works REALLY well but it's ridiculously slow.
	glReadPixels(cursorScreenPos.x,viewport[3]-cursorScreenPos.y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&z);
	z = 0;

	double ox,oy,oz;

	if(gluUnProject(cursorScreenPos.x,viewport[3]-cursorScreenPos.y,z,modelview,projection,viewport,&ox,&oy,&oz) == GLU_FALSE){
		cursorPos->x = 0;
		cursorPos->y = 0;
		cursorPos->z = 0;
	} else {
		cursorPos->x = ox;
		cursorPos->y = oy;
		cursorPos->z = oz;
	}
	if(gluUnProject(0,0,0,modelview,projection,viewport,&ox,&oy,&oz) == GLU_FALSE){
		upperLeft->x = 0;
		upperLeft->y = 0;
		dimensions->x = 0;
		dimensions->y = 0;
	} else {
		upperLeft->x = ox;
		upperLeft->y = oz;
		if(gluUnProject(viewport[2],viewport[3],0,modelview,projection,viewport,&ox,&oy,&oz) == GLU_FALSE){
			dimensions->x = 0;
			dimensions->y = 0;
		} else {
			dimensions->x = ox - upperLeft->x;
			dimensions->y = oz - upperLeft->y;
		}
	}
	cout << upperLeft->x << " " << upperLeft->y << "     " << dimensions->x << " " << dimensions->y << endl;
}
