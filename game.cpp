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
#include "modelmanager.h"

Game::Game() throw(GameInitException){
	try{
		display = new Display(800,600,32);
	} catch(DisplayException e){
		cout << "Error initializing display: " << e.what() << endl;
		const char* s = "Error initializing display."; // const char* so g++ doesn't complain about string -> char deprecation
		throw GameInitException(s); // don't run the game!
	}
	textureManager = new TextureManager();
	modelManager = new ModelManager();
	input = new Input();
	VertexF camPos;
	camPos.x = 0;
	camPos.y = 0;
	camPos.z = 15;
	camera = new Camera(camPos);
}

Game::~Game(){
	if(display != NULL){
		delete display;
	}
	if(textureManager != NULL){
		delete textureManager;
	}
	if(modelManager != NULL){
		delete modelManager;
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
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat p[4] = {0,0,10,1};
	glLightfv(GL_LIGHT0, GL_POSITION, p);

	ModelRef joint;
	joint = modelManager->LoadModel("joint.mdl",textureManager);

	float theta = 0;

	VertexF camPos;
	//camPos.x = 1*cos(theta);
	//camPos.y = .5*sin(2*theta);
	camPos.z = 15;

	Uint32 ticks = SDL_GetTicks();
	int frames = 0;

	while(!input->WindowClosed()){
		input->ProcessInput();
		theta += .015;
		//camPos.x = 1*cos(theta);
		//camPos.y = .5*sin(2*theta);
		camera->MoveTo(camPos);
		camera->LookThrough();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor3f(1,1,1);
		
		//glRotatef(90*sin(theta),1,0,0);
		//glRotatef(90*cos(theta),0,1,0);
		glRotatef(theta,0,1,0);

		modelManager->DrawModel(joint,textureManager);

		SDL_GL_SwapBuffers();

		++frames;
		if(frames > 5000){
			cout << "FPS: " << (1000*frames)/(SDL_GetTicks() - ticks) << endl;
			ticks = SDL_GetTicks();
			frames = 0;
		}
	}
}
