/*系统管理器*/
#pragma once
#ifndef _System
#define _System
#include <Windows.h>
#include "AEEngine.h"
#include "MyMath.h"
#include "LevelBase.h"
#include "GameStateManager.h"
#include "GameStateList.h"
#include <time.h>
#include "SoundSystem.h"
#include "Vector.h"
#include <math.h>
#pragma comment (lib, "fmod/fmodex_vc.lib")
#pragma comment (lib, "fmod/fmodexL_vc.lib")

// 系统初始化
extern int System_Initialize(HINSTANCE hInstance, int nCmdShow);

// 系统退出
extern void System_Exit(void);
extern SystemManage *manage;
extern SystemManage *MenuManage;
#endif