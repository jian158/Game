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
#pragma comment (lib, "fmod/fmodex_vc.lib")
#pragma comment (lib, "fmod/fmodexL_vc.lib")
enum TYPE
{
	// 游戏对象类型列表
	TYPE_SHIP = 0,	//对象类型为飞船
	TYPE_BULLET,	//对象类型为子弹
	TYPE_ENEMY,		//对象类型为敌军
	TYPE_SKill,		//对象类型为导弹
	TYPE_ENYME_BULLET,//对象类型为敌人子弹
	TYPE_BOSS1,		//BOSS1
	TYPE_BOSS2,		//BOSS2
	TYPE_BG1,
	TYPE_NUM
};
// 游戏对象基类/结构
typedef struct
{
	unsigned long		type;		// 游戏对象类型
	AEGfxVertexList*	pMesh;		// 形状
}GameObjBase;

// 游戏对象类/结构
typedef struct
{
	GameObjBase*		pObject;	// 指向基类（原始形状和类型）
	unsigned long		flag;		// 活动标志
	float				scale;		// 尺寸
	AEVec2				posCurr;	// 当前位置
	AEVec2				velCurr;	// 当前速度
	float				dirCurr;	// 机头指向
	AEMtx33				transform;	// 变换矩阵：每一帧都需要为每一个对象计算
	int					live;		//	生命
	float				speed;
}GameObj;
// 系统初始化
extern int System_Initialize(HINSTANCE hInstance, int nCmdShow);

// 系统退出
extern void System_Exit(void);
extern SystemManage *manage;
extern SystemManage *MenuManage;
#endif