#include <iostream>
#include <cmath>
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
#include "camera.h"

Game::Game() throw(GameInitException){
	try{
		display = new Display(800,600,32);
	} catch(DisplayException e){
		cout << "Error initializing display: " << e.what() << endl;
		const char* s = "Error initializing display."; // const char* so g++ doesn't complain about string -> char deprecation
		throw GameInitException(s); // don't run the game!
	}
	textureManager = new TextureManager();
	input = new Input();
	VertexF camPos;
	camPos.x = 0;
	camPos.y = 0;
	camPos.z = -15;
	camera = new Camera(camPos);
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
	if(camera != NULL){
		delete camera;
	}
}

void Game::Run(){
	glEnable(GL_TEXTURE_2D);

	TextureRef test = textureManager->LoadTexture("test.png");

	float theta = 0;

	VertexF camPos;
	camPos.x = 5*cos(theta);
	camPos.y = 2.5*sin(2*theta);
	camPos.z = 15;

	Uint32 ticks = SDL_GetTicks();
	int frames = 0;

	while(!input->WindowClosed()){
		input->ProcessInput();
		theta += .001;
		camPos.x = 5*cos(theta);
		camPos.y = 2.5*sin(2*theta);
		camera->MoveTo(camPos);
		camera->LookThrough();
		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(1,1,1);
		textureManager->BindTexture(test);
		glBegin(GL_QUADS);
			glTexCoord2f(0,0);
			glVertex3f(-1,-1,0);
			glTexCoord2f(1,0);
			glVertex3f(1,-1,0);
			glTexCoord2f(1,1);
			glVertex3f(1,1,0);
			glTexCoord2f(0,1);
			glVertex3f(-1,1,0);
		glEnd();

		SDL_GL_SwapBuffers();

		++frames;
		if(frames > 5000){
			cout << "FPS: " << (1000*frames)/(SDL_GetTicks() - ticks) << endl;
			ticks = SDL_GetTicks();
			frames = 0;
		}
	}
}
