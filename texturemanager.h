#ifndef _TEXTUREMANAGER_H_
#define _TEXTUREMANAGER_H_

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <iostream>
#include <string>
#include <map>
using namespace std;

typedef unsigned int TextureRef;

class TextureManager{
public:
	TextureManager();
	~TextureManager();

	TextureRef LoadTexture(string filename);
	void UnloadTexture(TextureRef ref);
private:
	map<string, TextureRef> filenames;
	map<TextureRef, GLuint> textures;
	map<TextureRef, int> refcounts;
	TextureRef next_unused_ref;
};

#endif
