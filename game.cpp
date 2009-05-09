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
#include "modelmanager.h"
#include "objectmanager.h"
#include "unit_worker.h"
#include "display.h"
#include "input.h"
#include "camera.h"
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
	objectManager = new ObjectManager();
	input = new Input();
	VertexF camPos;
	camPos.x = 0;
	camPos.y = 0;
	camPos.z = 15;
	PointF camAngle;
	camAngle.x = -3.14159/3;
	camAngle.y = 0;
	camera = new Camera(camPos, camAngle);
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
	if(objectManager != NULL){
		delete objectManager;
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

	//ObjectRef worker = objectManager->Add(new Unit_Worker(modelManager, textureManager));

	VertexF camPos;
	camPos.x = 0;
	camPos.y = 40;
	camPos.z = 0;

	PointF camAngle;
	camAngle.x = -3.14159/3;
	camAngle.y = 0;

	int frames = 0;
	
	bool running = true;

	Uint32 lastFPSCheck = SDL_GetTicks();

	Uint32 ticks = SDL_GetTicks();

	while(running){
		ticks = SDL_GetTicks() - ticks;

		Uint32 frameTicks = ticks;

		ticks = SDL_GetTicks();

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
			camPos.z -= .05*frameTicks;
		}
		if(input->GetKeyState(KEY_s) == KS_DOWN){
			camPos.z += .05*frameTicks;
		}
		if(input->GetKeyState(KEY_a) == KS_DOWN){
			camPos.x -= .05*frameTicks;
		}
		if(input->GetKeyState(KEY_d) == KS_DOWN){
			camPos.x += .05*frameTicks;
		}
		if(input->GetKeyState(KEY_DOWN)){
			camAngle.x -= .001*frameTicks;
		}
		if(input->GetKeyState(KEY_UP)){
			camAngle.x += .001*frameTicks;
		}
		if(input->GetKeyState(KEY_LEFT)){
			camAngle.y += .001*frameTicks;
		}
		if(input->GetKeyState(KEY_RIGHT)){
			camAngle.y -= .001*frameTicks;
		}

		//objectManager->UpdateAll(frameTicks);

		/*PointI mousePos = input->GetMousePos();
		GLdouble modelview[16];
		GLdouble projection[16];
		GLint viewport[4];

		glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
		glGetDoublev(GL_PROJECTION_MATRIX,projection);
		glGetIntegerv(GL_VIEWPORT,viewport);

		GLfloat z;
		glReadPixels(mousePos.x,viewport[3]-mousePos.y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&z);

		double ox,oy,oz;

		if(gluUnProject(mousePos.x,viewport[3]-mousePos.y,z,modelview,projection,viewport,&ox,&oy,&oz) == GLU_FALSE){
			cout << "fail" << endl;
		} else {
			if(input->GetMouseButtonState(BUTTON_LEFT) == BS_PRESSED && oy > 0.2){
				cout << "Yes m'lord?" << endl;
			}
		}*/

		//\/\/\/\/\/\/\/\/\/\/\/\/\//
		// END MAIN GAME LOOP CODE //
		/////////////////////////////

		////////////////////////
		// START DISPLAY CODE //
		//\/\/\/\/\/\/\/\/\/\///

		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//camera->MoveTo(camPos);
		//camera->ChangeAngle(camAngle);
		//camera->LookThrough();

		//glPushMatrix(); // we want to save this matrix so we can use it for picking in the next game loop

		//glEnable(GL_TEXTURE_2D);
		//textureManager->BindTexture(bell);

		/*glBegin(GL_QUADS);
			glColor3f(1,1,1);

			glNormal3f(0,1,0);

			glTexCoord2f(0,12);
			glVertex3f(-30,0,-30);

			glTexCoord2f(12,12);
			glVertex3f(30,0,-30);

			glTexCoord2f(12,0);
			glVertex3f(30,0,30);

			glTexCoord2f(0,0);
			glVertex3f(-30,0,30);
		glEnd();*/

		//set<ObjectRef> refs;
		//refs.insert(worker);

		//objectManager->DrawObjects(modelManager,textureManager,refs);

		//glPopMatrix(); // bring back the matrix for picking

		SDL_GL_SwapBuffers();
		
		//\/\/\/\/\/\/\/\/\///
		// END DISPLAY CODE //
		//////////////////////

		frames++;

		if(frames > 500){
			cout << "FPS: " << (1000*frames)/(SDL_GetTicks() - lastFPSCheck) << endl;
			lastFPSCheck = SDL_GetTicks();
			frames = 0;
		}
	}
}
