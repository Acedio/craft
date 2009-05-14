#include <fstream>
#include <vector>
#include <stack>
#include <queue>
#include <sstream>
#include <string>
#include <cmath>
#include <iostream>
#include <cassert>
#include <map>
using namespace std;

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/gl.h>

#include "globals.h"
#include "soundmanager.h"

SoundManager::SoundManager()
{
	nextUnusedRef = 1;

	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 512 ) == -1 )
    {
		cout << "Encountered an error while initializing the sound." << endl;
    }
}

SoundManager::~SoundManager()
{
	map<SoundRef, Mix_Chunk*>::iterator sound;
	for (sound = sounds.begin(); sound != sounds.end(); sound++)
	{
		Mix_FreeChunk(sound->second);
	}
	sounds.clear();
	Mix_CloseAudio();
}

SoundRef SoundManager::LoadSound(string filename)
{
	SoundRef ref;
	map<string, SoundRef>::iterator fileref = filenames.find(filename);
	if (fileref != filenames.end())
	{
		ref = fileref->second;
	}
	else
	{
		Mix_Chunk* Chunk = Mix_LoadWAV( filename.c_str() );
		if (Chunk == NULL)
		{
			cout << "Unable to load sound \"" + filename + "\"." << endl;
			return 0;
		}
		ref = nextUnusedRef;
		nextUnusedRef ++;
		sounds[ref] = Chunk;
	}
	return ref;
}

void SoundManager::PlaySound(SoundRef ref)
{
	map<SoundRef, Mix_Chunk*>::iterator sound = sounds.find(ref);
	if (sound != sounds.end())
	{
		Mix_PlayChannel( -1, sound->second, 0 );
	}
	else
	{
		cout << "Could not play sound " << ref << "." << endl;
	}
}