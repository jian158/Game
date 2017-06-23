///******************************************************************************/
///*!
// \file   SoundSystem.c
// \proj   Dark Rising
// \author Dean Hulse
// \par    Course: GAM150
// \par    All content ?2014 DigiPen (USA) Corporation, all rights reserved.
// \brief  Debugging System defined
// */
///******************************************************************************/
#include <Windows.h>
#include "SoundSystem.h"
void Sound::SoundSystemLoad(void)
{
	Result = FMOD_System_Create(&System);

	Result = FMOD_System_Init(System, 10, FMOD_INIT_NORMAL, NULL);
}

int Sound::SoundAdd(const char* filePath, const int loopStyle, const unsigned int soundList)
{
	Result = FMOD_System_CreateSound(System, filePath, loopStyle, 0, &sound[soundList]);

	if(Result)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void Sound::SoundPlay(const unsigned int soundList)
{
	Result = FMOD_System_PlaySound(System, FMOD_CHANNEL_FREE, sound[soundList], 0, &Channel[soundList]);
	SetVolume(SOUND_BACK, 1);
}

void Sound::SoundPausePlay(const unsigned int soundList)
{
	Result = FMOD_Channel_GetPaused(Channel[soundList], &Paused[soundList]);

	if(Paused[soundList])
	{
		FMOD_Channel_SetPaused(Channel[soundList], 0);
	}
	else
	{
		FMOD_Channel_SetPaused(Channel[soundList], 1);
	}
}

void Sound::SoundStop(const unsigned int soundList)
{
	if(SoundIsPlaying(soundList))
	{
		Result = FMOD_Channel_Stop(Channel[soundList]);
	}
}

int Sound::SoundIsPlaying(const unsigned int soundList)
{
	Result = FMOD_Channel_IsPlaying(Channel[soundList], &Playing[soundList]);
	return Playing[soundList];
}

void Sound::SetVolume(const unsigned int soundList, float volume)
{
	FMOD_Channel_SetVolume(Channel[soundList], volume);
}

float Sound::GetVolume(const unsigned int soundList)
{
	FMOD_Channel_GetVolume(Channel[soundList], &Volume[soundList]);

	return Volume[soundList];
}

void Sound::SoundSystemExit(void)
{
	Result = FMOD_System_Close(System);
}

void Sound::SoundUpdate (void)
{
	Result = FMOD_System_Update(System);
}