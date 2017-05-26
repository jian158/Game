/*游戏状态管理器*/
#pragma once
#ifndef _GameStateManager
#define _GameStateManager


// 游戏状态管理器的初始化
extern void GSM_Initialize(int);

// 游戏状态管理器的更新
extern void GSM_Update(int);
// 6 Function Pointers
extern void (*pLoad)(void);
extern void (*pIni)(void);
extern void (*pUpdate)(void);
extern void (*pDraw)(void);
extern void (*pFree)(void);
extern void (*pUnload)(void);

#endif