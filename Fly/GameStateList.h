/* 游戏状态声明 */

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
	GS_MENU,
	GS_RETURN,
	GS_Restart,			// Restart
	GS_Quit				// Quit
};

extern int Current, Previous, Next;  // 状态指示器
#endif