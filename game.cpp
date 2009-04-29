#include <iostream>
using namespace std;

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

#include "globals.h"
#include "game.h"
#include "texturemanager.h"
#include "display.h"
#include "input.h"

Game::Game(){
	try{
		display = new Display(800,600,32);
	} catch(DisplayException e){
		cout << "Error initializing display: " << e.what() << endl;
	}
	textureManager = new TextureManager();
	input = new Input();
}

Game::~Game(){
	if(display != NULL){
		delete display;
	}
	if(textureManager != NULL){
		delete textureManager;
	}
	if(input != NULL){
		delete input;
	}
}

void Game::Run(){
	glEnable(GL_TEXTURE_2D);

	TextureRef test = textureManager->LoadTexture("test.png");

	while(!input->WindowClosed()){
		input->ProcessInput();
		SDL_Delay(100);

		glClear(GL_COLOR_BUFFER_BIT);
		glLoadIdentity();
		glColor3f(1,1,1);
		glTranslatef(0,0,-25);
		textureManager->BindTexture(test);
		glBegin(GL_QUADS);
			glTexCoord2f(0,0);
			glVertex3f(-5,-5,0);
			glTexCoord2f(1,0);
			glVertex3f(5,-5,0);
			glTexCoord2f(1,1);
			glVertex3f(5,5,0);
			glTexCoord2f(0,1);
			glVertex3f(-5,5,0);
		glEnd();

		SDL_GL_SwapBuffers();
	}
}
