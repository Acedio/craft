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

static void ShadowMatrix(float ground[4], float light[4]){
	float  dot;
	float  shadowMat[4][4];

	dot = ground[0] * light[0] +
		ground[1] * light[1] +
		ground[2] * light[2] +
		ground[3] * light[3];

	shadowMat[0][0] = dot - light[0] * ground[0];
	shadowMat[1][0] = 0.0 - light[0] * ground[1];
	shadowMat[2][0] = 0.0 - light[0] * ground[2];
	shadowMat[3][0] = 0.0 - light[0] * ground[3];

	shadowMat[0][1] = 0.0 - light[1] * ground[0];
	shadowMat[1][1] = dot - light[1] * ground[1];
	shadowMat[2][1] = 0.0 - light[1] * ground[2];
	shadowMat[3][1] = 0.0 - light[1] * ground[3];

	shadowMat[0][2] = 0.0 - light[2] * ground[0];
	shadowMat[1][2] = 0.0 - light[2] * ground[1];
	shadowMat[2][2] = dot - light[2] * ground[2];
	shadowMat[3][2] = 0.0 - light[2] * ground[3];

	shadowMat[0][3] = 0.0 - light[3] * ground[0];
	shadowMat[1][3] = 0.0 - light[3] * ground[1];
	shadowMat[2][3] = 0.0 - light[3] * ground[2];
	shadowMat[3][3] = dot - light[3] * ground[3];

	glMultMatrixf((const GLfloat*)shadowMat);
}

Game::Game() throw(GameInitException){
	try{
		display = new Display(1025,768,32); // display must be initialized first because it initializes SDL
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

	TextureRef grass = textureManager->LoadTexture("grass.png");

	objectManager.LoadObjectMap("map.map", &gridMap, &modelManager, textureManager);

	VertexF camPos;
	camPos.x = 30;
	camPos.y = 60;
	camPos.z = 30;

	PointF camAngle;
	camAngle.x = -3.14159/3;
	camAngle.y = 0;

	int frames = 0;
	
	bool running = true;

	float zoom = 0;

	Uint32 lastFPSCheck = SDL_GetTicks();

	Uint32 ticks = SDL_GetTicks();

	ObjectRef selected = 0;

	SoundManager soundmanager;
	SoundRef go_here = soundmanager.LoadSound("data/sounds/go.wav");
	SoundRef select_unit = soundmanager.LoadSound("data/sounds/click.wav");

	soundmanager.PlayMusic("data/sounds/music1.mp3");

	while(running){
		ticks = SDL_GetTicks() - ticks;

		Uint32 frameTicks = ticks;

		ticks = SDL_GetTicks();

		///////////////////////////////
		// START MAIN GAME LOOP CODE //
		//\/\/\/\/\/\/\/\/\/\/\/\/\/\//

		input->ProcessInput();

		objectManager.UpdateAll(frameTicks,&gridMap,&modelManager);
		
		VertexF worldPos;
		VertexF screenCorners[4];
		//Uint32 b = SDL_GetTicks();
		display->ScreenToWorld(input->GetMousePos(), &worldPos, screenCorners);
		//cout << "t: " << SDL_GetTicks() - b << endl << endl;

		PointF groundCorners[4];

		for(int i = 0; i < 4; i++){
			VertexF v = screenCorners[i] - camPos;
			groundCorners[i].x = camPos.x - v.x*(camPos.y/v.y);
			groundCorners[i].y = camPos.z - v.z*(camPos.y/v.y);
		}

		/*// UPPER LEFT CORNER POSITION
		VertexF ulv = upperLeft - camPos;
		PointF start;

		start.x = camPos.x - ulv.x*(camPos.y/ulv.y);
		start.y = camPos.z - ulv.z*(camPos.y/ulv.y);

		// DIMENSIONS OF DRAWING AREA

		VertexF lrv = lowerRight - camPos;
		PointF end;

		end.x = camPos.x - lrv.x*(camPos.y/lrv.y);
		end.y = camPos.z - lrv.z*(camPos.y/lrv.y);*/

		if(input->GetMouseButtonState(BUTTON_LEFT) == BS_RELEASED){
			if(worldPos.y >= -1){ // If we're below -1 then we've definitely missed the platform
				objectManager.HandleClick(worldPos,BUTTON_LEFT,&gridMap);
				PointI pos;
				pos.x = worldPos.x/TILE_SIZE;
				pos.y = worldPos.z/TILE_SIZE;
				ObjectRef ref = gridMap.GetObjectRefAt(pos);
				if(ref != 0){
					selected = ref;
					if (objectManager.GetObject(ref) != NULL && (objectManager.GetObject(ref)->GetType() & OBJ_UNIT))
					{
						soundmanager.PlaySound(select_unit);
					}
				}
			}
		}
		if(input->GetMouseButtonState(BUTTON_RIGHT) == BS_RELEASED){
			if(worldPos.y >= -1){// If we're below -1 then we've definitely missed the platform
				PointI pos;
				pos.x = worldPos.x/TILE_SIZE;
				pos.y = worldPos.z/TILE_SIZE;
				Object *obj = objectManager.GetObject(selected);
				if(obj != NULL){
					if(obj->GetType()&OBJ_UNIT){
						((Unit*)obj)->MoveTo(pos,&gridMap);
						soundmanager.PlaySound(go_here);
					}
				}
			}
		}

		if(input->GetKeyState(KEY_a) == KS_RELEASED){
			Object* obj = objectManager.GetObject(selected);
			if (obj != NULL && (obj->GetType() & OBJ_UNIT))
			{
				Unit* unit = (Unit*)obj;
			}
		}

		if(input->WindowClosed()){
			running = false;
		}
		if(input->GetKeyState(KEY_ESCAPE) == KS_RELEASED){
			running = false;
		}
		if(input->GetKeyState(KEY_UP) == KS_DOWN){
			camPos.z -= .1*frameTicks;
		}
		if(input->GetKeyState(KEY_DOWN) == KS_DOWN){
			camPos.z += .1*frameTicks;
		}
		if(input->GetKeyState(KEY_LEFT) == KS_DOWN){
			camPos.x -= .1*frameTicks;
		}
		if(input->GetKeyState(KEY_RIGHT) == KS_DOWN){
			camPos.x += .1*frameTicks;
		}
		if(input->GetMouseButtonState(BUTTON_WHEELUP) == BS_RELEASED){
			zoom += .1;
			if(zoom > 1){
				zoom = 1;
			}
		}
		if(input->GetMouseButtonState(BUTTON_WHEELDOWN) == BS_RELEASED){
			zoom -= .1;
			if(zoom < 0){
				zoom = 0;
			}
		}

		camAngle.x = -3.14159/3+(3.14159/6)*zoom;
		camPos.y = 80-60*zoom;

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

		GLfloat light[4] = {1000,1000,1000,1};
		glLightfv(GL_LIGHT0, GL_POSITION, light);

		glPushMatrix(); // we want to save this matrix so we can use it for picking in the next game loop

		glEnable(GL_TEXTURE_2D);
		textureManager->BindTexture(grass);

		int s = 64;
		
		glBegin(GL_QUADS);
			glColor3f(1,1,1);

			glNormal3f(0,1,0);

			glTexCoord2f(0,s/2);
			glVertex3f(0,0,0);

			glTexCoord2f(s/2,s/2);
			glVertex3f(5*s,0,0);

			glTexCoord2f(s/2,0);
			glVertex3f(5*s,0,5*s);

			glTexCoord2f(0,0);
			glVertex3f(0,0,5*s);
		glEnd();
		
		set<ObjectRef> drawSet = gridMap.GetDrawSet(groundCorners);

		objectManager.DrawObjects(&modelManager,textureManager,drawSet);

		glPopMatrix();

		// SHADOWS

		glPushMatrix();
		
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(-1,-1);

		float ground[4] = {0,1,0,0};

		ShadowMatrix(ground,light);
		
		objectManager.DrawShadows(&modelManager,drawSet);

		glDisable(GL_POLYGON_OFFSET_FILL);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);

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
