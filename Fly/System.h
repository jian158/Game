/*系统管理器*/
//write by 黄金宝玉
#pragma once
#ifndef _System
#define _System
#include <Windows.h>
#include "AEEngine/AEEngine.h"
#include "MyMath.h"
#include "LevelBase.h"
#include "GameStateManager.h"
#include "GameStateList.h"
#include <time.h>
#include "fmod/SoundSystem.h"
#include "Vector.h"
#include <math.h>
#pragma comment (lib, "fmod/fmodex_vc.lib")
#pragma comment (lib, "fmod/fmodexL_vc.lib")
#pragma comment (lib, "AEEngine/Alpha_Engine.lib")
// 系统初始化
extern int System_Initialize(HINSTANCE hInstance, int nCmdShow);

// 系统退出
extern void System_Exit(void);
extern SystemManage *manage;
extern SystemManage *MenuManage;
#endif