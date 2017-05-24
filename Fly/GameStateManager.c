/* Project:		GSMWin
   File Name:	GameStateManager.c
   Author:		刘芳
   Date:		
   Purpose:		GSM  */

#include "GameStateManager.h"
#include "GameStateList.h"
#include "Level1.h"
#include "Level2.h"
#include "MainMenu.h"
#include "GameOver.h"
#include "win.h"


// GSM初始化
void GSM_Initialize(int level)
{
	// 初始化状态指示器
	Current = Previous = Next = level;
}

// GSM更新
void GSM_Update(int level)
{
	// Function Pointer Point to functions
	switch(level)
	{
		case GS_Over:
			pLoad = LoadOver;
			pIni = InitOver;
			pUpdate = UpdateOver;
			pDraw = DrawOver;
			pFree = FreeOver;
			pUnload = UnloadOver;
			break;
		case GS_WIN:
			pLoad = LoadWin;
			pIni = InitWin;
			pUpdate = UpdateWin;
			pDraw = DrawWin;
			pFree = FreeWin;
			pUnload = UnloadWin;
			break;
		case GS_MAIN:
			pLoad = LoadMain;
			pIni = InitMenu;
			pUpdate = UpdateMenu;
			pDraw = DrawMenu;
			pFree = FreeMenu;
			pUnload = UnloadMenu;
			break;
		case GS_L1:
			{
				pLoad = Load1;
				pIni = Ini1;
				pUpdate = Update1;
				pDraw = Draw1;
				pFree = Free1;
				pUnload = Unload1;
				break;
			}
			case GS_L2:
			{
				pLoad = Load2;
				pIni = Ini2;
				pUpdate = Update2;
				pDraw = Draw2;
				pFree = Free2;
				pUnload = Unload2;
				break;
			}
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

