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
#include "gridmap.h"

Game::Game() throw(GameInitException){
	try{
		display = new Display(800,600,32); // display must be initialized first because it initializes SDL
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
	gridMap = new GridMap(30,30,0);
}

Game::~Game(){
	if(gridMap != NULL){
		delete gridMap;
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
	if(display != NULL){ // display must be deleted last because it will uninitialize SDL
		delete display;
	}
}

void Game::Run(){
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	GLfloat p[4] = {0,0,10,1};
	glLightfv(GL_LIGHT0, GL_POSITION, p);

	TextureRef bell = textureManager->LoadTexture("bell.png");

	ModelRef archer;
	archer = modelManager->LoadModel("data/units/archer/archer.mdl",textureManager);
	AnimationInstance archerWalk = modelManager->GetAnimationInstance(archer, "walk");
	AnimationInstance archerAttack = modelManager->GetAnimationInstance(archer, "attack");

	ModelRef knight;
	knight = modelManager->LoadModel("data/units/knight/knight.mdl",textureManager);
	AnimationInstance knightWalk = modelManager->GetAnimationInstance(knight, "walk");
	AnimationInstance knightAttack = modelManager->GetAnimationInstance(knight, "attack");

	ModelRef swordsman;
	swordsman = modelManager->LoadModel("data/units/swordsman/swordsman.mdl",textureManager);
	AnimationInstance swordsmanWalk = modelManager->GetAnimationInstance(swordsman, "walk");
	AnimationInstance swordsmanAttack = modelManager->GetAnimationInstance(swordsman, "attack");

	ModelRef worker;
	worker = modelManager->LoadModel("data/units/worker/worker.mdl",textureManager);
	AnimationInstance workerWalk = modelManager->GetAnimationInstance(worker, "walk");
	AnimationInstance workerAttack = modelManager->GetAnimationInstance(worker, "attack");

	ModelRef catapult;
	catapult = modelManager->LoadModel("data/units/catapult/catapult.mdl",textureManager);
	AnimationInstance catapultWalk = modelManager->GetAnimationInstance(catapult, "walk");
	AnimationInstance catapultAttack = modelManager->GetAnimationInstance(catapult, "attack");

	VertexF camPos;
	camPos.x = 0;
	camPos.y = 30;
	camPos.z = 55;

	VertexF camDir;
	camDir.x = 0;
	camDir.y = 0;
	camDir.z = 1;

	Uint32 ticks = SDL_GetTicks();

	int gameFrames = 0;
	int displayFrames = 0;
	
	int mspf = 1000/60;

	int extraTicks = 0;

	bool running = true;

	while(running){
		Uint32 frameTicks = SDL_GetTicks();

		///////////////////////////////
		// START MAIN GAME LOOP CODE //
		//\/\/\/\/\/\/\/\/\/\/\/\/\/\//

		input->ProcessInput();
		if(input->WindowClosed()){
			running = false;
		}
		if(input->GetKeyState(KEY_ESCAPE) == KS_DOWN){
			running = false;
		}
		
		if(input->GetKeyState(KEY_w) == KS_DOWN){
			camPos.z -= .5;
		}
		if(input->GetKeyState(KEY_s) == KS_DOWN){
			camPos.z += .5;
		}
		if(input->GetKeyState(KEY_a) == KS_DOWN){
			camPos.x -= .5;
		}
		if(input->GetKeyState(KEY_d) == KS_DOWN){
			camPos.x += .5;
		}
		if(input->GetKeyState(KEY_DOWN)){
			camDir.y -= .01;
		}
		if(input->GetKeyState(KEY_UP)){
			camDir.y += .01;
		}

		//\/\/\/\/\/\/\/\/\/\/\/\/\//
		// END MAIN GAME LOOP CODE //
		/////////////////////////////

		++gameFrames;

		frameTicks = SDL_GetTicks() - frameTicks;

		extraTicks += mspf - frameTicks;

		if(extraTicks > 0){ // if we aren't running behind, draw the scene
			frameTicks = SDL_GetTicks();

			////////////////////////
			// START DISPLAY CODE //
			//\/\/\/\/\/\/\/\/\/\///

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			camera->MoveTo(camPos);
			camera->LookTo(camDir);
			camera->LookThrough();

			glEnable(GL_TEXTURE_2D);
			textureManager->BindTexture(bell);

			glBegin(GL_QUADS);
				glColor3f(1,1,1);

				glNormal3f(0,1,0);

				glTexCoord2f(0,1);
				glVertex3f(-30,0,-30);

				glTexCoord2f(1,1);
				glVertex3f(30,0,-30);

				glTexCoord2f(1,0);
				glVertex3f(30,0,30);

				glTexCoord2f(0,0);
				glVertex3f(-30,0,30);
			glEnd();

			SDL_GL_SwapBuffers();
			
			//\/\/\/\/\/\/\/\/\///
			// END DISPLAY CODE //
			//////////////////////

			frameTicks = SDL_GetTicks() - frameTicks;
			extraTicks -= frameTicks;
			displayFrames++;
			if(extraTicks > 0){ // TODO: Rather than simply limiting frame rate I really should do some sort of delta calculations and update animations accordingly
				SDL_Delay(extraTicks);
			}
		}

		if(gameFrames > 500){
			cout << "Game FPS: " << (1000*gameFrames)/(SDL_GetTicks() - ticks) << " Display FPS: " << (1000*displayFrames)/(SDL_GetTicks() - ticks) << endl;
			ticks = SDL_GetTicks();
			gameFrames = 0;
			displayFrames = 0;
		}
	}
}
