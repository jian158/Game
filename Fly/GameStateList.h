/* Project:		GSMWin
   File Name:	GameStateList.h
   Author:		刘芳
   Date:		
   Purpose:		游戏状态声明，主要为Game Flow的状态切换所用 */

#ifndef _GameStateList
#define _GameStateList

enum
{
	GS_MAIN,
	GS_L1,              // Level1
	GS_L2,              // Level2
	GS_NUM,             // Total Number
	GS_Over,
	GS_WIN,
	GS_Restart,			// Restart
	GS_Quit				// Quit
};

int Current, Previous, Next;  // 状态指示器
//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

#endif