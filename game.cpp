#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
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
#include "soundmanager.h"

Game::Game() throw(GameInitException){
	try{
		display = new Display(800,600,32); // display must be initialized first because it initializes SDL
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
	camPos.z = 15;
	PointF camAngle;
	camAngle.x = -3.14159/3;
	camAngle.y = 0;
	camera = Camera(camPos, camAngle);
}

Game::~Game(){
	if(textureManager != NULL){
		delete textureManager;
	}
	if(input != NULL){
		delete input;
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

	gridMap = GridMap(objectManager.LoadObjectMap("test.map", &modelManager, textureManager));
	
	/*for(int x = 0; x < 10; x++){
		for(int y = 0; y < 10; y++){
			if(x&1){
			objectManager.Add(new Unit_Worker(&modelManager, textureManager, x, y),&gridMap);
			}
		}
	}*/

	VertexF camPos;
	camPos.x = 30;
	camPos.y = 40;
	camPos.z = 30;

	PointF camAngle;
	camAngle.x = -3.14159/3;
	camAngle.y = 0;

	int frames = 0;
	
	bool running = true;

	Uint32 lastFPSCheck = SDL_GetTicks();

	Uint32 ticks = SDL_GetTicks();

	ObjectRef selected = 0;

	SoundManager soundmanager;
	SoundRef sound = soundmanager.LoadSound("data/sounds/low.wav");
	

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
	/////////sound test
		if(input->GetKeyState(KEY_p) == KS_PRESSED){
			soundmanager.PlaySound(sound);
		}

		objectManager.UpdateAll(frameTicks,&gridMap,&modelManager);

		VertexF worldPos = display->ScreenToWorld(input->GetMousePos());

		if(input->GetMouseButtonState(BUTTON_LEFT) == BS_PRESSED){
			if(worldPos.y >= -1){ // If we're below -1 then we've definitely missed the platform
				objectManager.HandleClick(worldPos,BUTTON_LEFT,&gridMap);
				PointI pos;
				pos.x = worldPos.x/TILE_SIZE;
				pos.y = worldPos.z/TILE_SIZE;
				ObjectRef ref = gridMap.GetObjectRefAt(pos);
				if(ref != 0){
					selected = ref;
				}
			}
		}
		if(input->GetMouseButtonState(BUTTON_RIGHT) == BS_PRESSED){
			if(worldPos.y >= -1){// If we're below -1 then we've definitely missed the platform
				PointI pos;
				pos.x = worldPos.x/TILE_SIZE;
				pos.y = worldPos.z/TILE_SIZE;
				Object *obj = objectManager.GetObject(selected);
				if(obj != NULL){
					if(obj->GetType()&OBJ_UNIT){
						((Unit*)obj)->MoveTo(pos,&gridMap);
					}
				}
			}
		}

		//\/\/\/\/\/\/\/\/\/\/\/\/\//
		// END MAIN GAME LOOP CODE //
		/////////////////////////////

		////////////////////////
		// START DISPLAY CODE //
		//\/\/\/\/\/\/\/\/\/\///

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.MoveTo(camPos);
		camera.ChangeAngle(camAngle);
		camera.LookThrough();

		glPushMatrix(); // we want to save this matrix so we can use it for picking in the next game loop

		glEnable(GL_TEXTURE_2D);
		textureManager->BindTexture(bell);

		glBegin(GL_QUADS);
			glColor3f(1,1,1);

			glNormal3f(0,1,0);

			glTexCoord2f(0,1);
			glVertex3f(0,0,0);

			glTexCoord2f(1,1);
			glVertex3f(50,0,0);

			glTexCoord2f(1,0);
			glVertex3f(50,0,50);

			glTexCoord2f(0,0);
			glVertex3f(0,0,50);
		glEnd();

		objectManager.DrawObjects(&modelManager,textureManager,gridMap.GetDrawSet(camera));

		glPopMatrix(); // bring back the matrix for picking

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
