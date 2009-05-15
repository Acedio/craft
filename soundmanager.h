#ifndef _SOUNDMANAGER_H_
#define _SOUNDMANAGER_H_

#include <vector>
#include <string>
#include <map>
#include <list>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
using namespace std;

#include "globals.h"

typedef unsigned int SoundRef;

class SoundManager{
private:
	map<SoundRef, Mix_Chunk*> sounds;
	map<string, SoundRef> filenames;
	SoundRef nextUnusedRef;

	Mix_Music *music;

public:
	SoundManager();
	~SoundManager();

	SoundRef LoadSound(string filename);
	void UnloadSound(SoundRef ref);
	void PlaySound(SoundRef ref);
	void PlayMusic(string filename);
};

#endif
