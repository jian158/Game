/*ÓÎÏ·×´Ì¬¹ÜÀíÆ÷*/
#pragma once
#ifndef _GameStateManager
#define _GameStateManager
#include "LevelBase.h"
extern class SystemManage
{
	LevelBase *level = nullptr;
public:
	SystemManage();
	SystemManage(int level);
	void setLevel(int level);
	int Current = 0, Previous = 0, Next = 0;
	void Run();
	void Update(int level);
	void FreeLevel();
	~SystemManage()
	{
		delete level;
	}
};
#endif