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
#include "object.h"

Game::Game(){
	display = new Display(800,600,32);
	textureManager = new TextureManager();
}

Game::~Game(){
	if(display != NULL){
		delete display;
	}
	if(textureManager != NULL){
		delete textureManager;
	}
}

void Game::Run(){
}
