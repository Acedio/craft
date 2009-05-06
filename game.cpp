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

	float theta = 0;

	VertexF camPos;
	camPos.x = 0;// 1*cos(theta);
	camPos.y = 0;//.5*sin(2*theta);
	camPos.z = 45;

	Uint32 ticks = SDL_GetTicks();

	int gameFrames = 0;
	int displayFrames = 0;
	
	int mspf = 1000/FPS;

	int extraTicks = 0;

	while(!input->WindowClosed()){
		Uint32 frameTicks = SDL_GetTicks();
		
		input->ProcessInput();
		theta += .05;

		archerAttack.NextFrame();
		archerWalk.NextFrame();
		knightAttack.NextFrame();
		knightWalk.NextFrame();
		swordsmanAttack.NextFrame();
		swordsmanWalk.NextFrame();

		++gameFrames;

		frameTicks = SDL_GetTicks() - frameTicks;

		extraTicks += mspf - frameTicks;

		if(extraTicks > 0){ // if we aren't running behind, draw the scene
			frameTicks = SDL_GetTicks();
			camera->MoveTo(camPos);
			camera->LookThrough();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glColor3f(1,1,1);
			
			glRotatef(90*sin(.01*theta),1,0,0);
			glRotatef(90*cos(.01*theta),0,1,0);
			glRotatef(theta,0,1,0);
			glTranslatef(-20,-5,-20);
			glEnable(GL_TEXTURE_2D);
			textureManager->BindTexture(bell);
			glBegin(GL_QUADS);
			glColor3f(1,1,1);
			glNormal3f(0,1,0);
			glTexCoord2f(0,0);
			glVertex3f(-10,0,-10);
			glTexCoord2f(1,0);
			glVertex3f(50,0,-10);
			glTexCoord2f(1,1);
			glVertex3f(50,0,50);
			glTexCoord2f(0,1);
			glVertex3f(-10,0,50);
			glEnd();
			for(int y = -5; y < 5; y++){
				for(int x = -5; x < 5; x++){
					glPushMatrix();
					glRotatef((float)(10*y+x)+10*theta,0,1,0);
					switch((y+x)&3){
						case 0:
							//modelManager->DrawModel(archer,textureManager, &archerWalk);
							modelManager->DrawModel(swordsman,textureManager, &swordsmanWalk);
							break;
						case 1:
							modelManager->DrawModel(knight,textureManager, &knightAttack);
							break;
						case 2:
							modelManager->DrawModel(archer,textureManager, &archerAttack);
							break;
						case 3: 
							modelManager->DrawModel(knight,textureManager, &knightWalk);
							break;
					}
					glPopMatrix();
					glTranslatef(4,0,0);
				}
				glTranslatef(-40,0,4);
			}
			SDL_GL_SwapBuffers();
			frameTicks = SDL_GetTicks() - frameTicks;
			extraTicks -= frameTicks;
			displayFrames++;
			if(extraTicks > 0){ // if we STILL have extra time
				SDL_Delay(extraTicks);
				extraTicks = 0;
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
