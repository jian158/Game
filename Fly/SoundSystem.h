///******************************************************************************/
///*!
// \file   SoundSystem.c
// \proj   Dark Rising
// \author Jesus Herrera
// \par    Course: GAM150
// \par    All content ?2014 DigiPen (USA) Corporation, all rights reserved.
// \brief  Debugging System defined
// */
///******************************************************************************/

#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H

#include "fmod/fmod.h"
enum SOUND_LIST
{
	SOUND_BACK,
	SOUND_SHOOT,
	SOUND_BOOM,
	SOUND_END
};
class Sound
{
private:
FMOD_RESULT  Result;
FMOD_SYSTEM  *System;
FMOD_SOUND   *sound[SOUND_END];
FMOD_CHANNEL *Channel[SOUND_END];
FMOD_BOOL    Playing[SOUND_END];
FMOD_BOOL    Paused[SOUND_END];
float        Volume[SOUND_END];
public:
void SoundSystemLoad(void);
int SoundAdd(const char* filePath, const int loopStyle, const unsigned int soundList);
void SoundPlay(const unsigned int soundList);
void SoundPausePlay(const unsigned int soundList);
void SoundStop(const unsigned int soundList);
int SoundIsPlaying(const unsigned int soundList);
void SetVolume(const unsigned int soundList, float volume);
float GetVolume(const unsigned int soundList);
void SoundSystemExit(void);
void SoundUpdate (void);
void Sound_test(void);
};




#endif