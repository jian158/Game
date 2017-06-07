/*ϵͳ������*/
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

// ϵͳ��ʼ��
extern int System_Initialize(HINSTANCE hInstance, int nCmdShow);

// ϵͳ�˳�
extern void System_Exit(void);
extern SystemManage *manage;
extern SystemManage *MenuManage;
#endif