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

#include "texturemanager.h"

TextureManager::TextureManager(){
	next_unused_ref = 0;
	TextureRef ref = next_unused_ref;
	next_unused_ref += 1;

	// make the default magenta checkerboard texture

	GLfloat color[12] = {1,0,1, 0.1,0.1,0.1, 0.1,0.1,0.1, 1,0,1};
	GLuint texture;

	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,2,2,GL_RGB,GL_FLOAT,color);

	textures[ref] = texture;
}

TextureManager::~TextureManager(){
	for(map<TextureRef, GLuint>::iterator i = textures.begin(); i != textures.end(); ++i){
		glDeleteTextures(1,&(i->second));
	}
}

TextureRef TextureManager::LoadTexture(string filename){
	TextureRef ref;
	if(filenames.find(filename) != filenames.end()){ // if we already have a reference to this texture, return the ref
		ref = filenames[filename];
		refcounts[ref] += 1;
	} else { // otherwise we should load it
		SDL_Surface *surface;
		GLuint texture;

		surface = IMG_Load(filename.c_str());
		if(!surface){
			cout << "Unable to load texture \"" + filename + "\"." << endl;
			return 0;
		}
		int bpp = surface->format->BytesPerPixel;
		for(int y = 0; y < surface->h/2; ++y){
			for(int x = 0; x < surface->w*bpp; ++x){
				Uint8 temp = ((Uint8*)surface->pixels)[y*surface->w*bpp+x];
				((Uint8*)surface->pixels)[y*surface->w*bpp+x] = ((Uint8*)surface->pixels)[(surface->h-y-1)*surface->w*bpp+x];
				((Uint8*)surface->pixels)[(surface->h-y-1)*surface->w*bpp+x] = temp;
			}
		}
		GLenum format;
		if(bpp == 4){
			if(surface->format->Rmask == 0x000000ff){
				format = GL_RGBA;
			} else {
				format = GL_BGRA;
			}
		} else if (bpp == 3){
			if(surface->format->Rmask == 0x000000ff){
				format = GL_RGB;
			} else {
				format = GL_BGR;
			}
		} else {
			cout << "Texture \"" + filename + "\" does not have enough channels." << endl;
			return 0;
		}

		glGenTextures(1, &texture);

		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		gluBuild2DMipmaps(GL_TEXTURE_2D,bpp,surface->w,surface->h,format,GL_UNSIGNED_BYTE,surface->pixels);
		
		SDL_FreeSurface(surface);

		ref = next_unused_ref;

		textures[ref] = texture; // store the stats about our new texture
		filenames[filename] = ref;
		refcounts[ref] = 1;

		next_unused_ref += 1;
	}
	return ref;
}

void TextureManager::UnloadTexture(TextureRef ref){
	if(ref != 0 && refcounts.find(ref) != refcounts.end()){ // if it's not the default texture and the reference exists in our store
		refcounts[ref] -= 1;
		if(refcounts[ref] <= 0){ // if the texture is no longer being used by any objects, so we can really remove it
			refcounts.erase(ref);
			textures.erase(ref);
			for(map<string,TextureRef>::iterator i = filenames.begin(); i != filenames.end(); ++i){
				if(i->second == ref){
					filenames.erase(i);
					break;
				}
			}
		}
	}
}

void TextureManager::BindTexture(TextureRef ref){
	map<TextureRef,GLuint>::iterator tex = textures.find(ref);
	if(tex != textures.end()){
		glBindTexture(GL_TEXTURE_2D, tex->second);
	} else {
		glBindTexture(GL_TEXTURE_2D, textures[0]);
	}
}
