/*ÓÎÏ·×´Ì¬¹ÜÀíÆ÷*/
#pragma once
#ifndef _GameStateManager
#define _GameStateManager
#include "LevelBase.h"
#include "SoundSystem.h"
class SystemManage
{
	LevelBase *level = nullptr;
	Sound sound;
public:
	int IsPlaying = 1;
	SystemManage();
	SystemManage(int level);
	void setLevel(int level);
	int Current = 0, Previous = 0, Next = 0;
	void Run();
	void Update(int level);
	void FreeLevel();
	void OpenOrCloseSound(int mode);
	void PlayShoot();
	void PlayBoom();
	~SystemManage()
	{
		delete level;
	}
};
#endif