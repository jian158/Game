/* GSM  */

#include "GameStateManager.h"
#include "System.h"
#include "MainUi.h"
#include "Level1.h"
#include "Level2.h"
#include "win.h"
#include "GameOver.h"
#include "GameMenu.h"

SystemManage::SystemManage()
{
	Current = Previous = Next = GS_MAIN;
}
SystemManage::SystemManage(int level)
{
	Current = Previous = Next = level;
}
void SystemManage::setLevel(int level)
{
	Current = Previous = Next = level;
}
void SystemManage::Run()
{
	while (Current != GS_Quit&&Current!=GS_RETURN)
	{
		if (Current == GS_Restart)
		{
			Current = Previous;
			Next = Previous;
		}
		else
		{
			// GSM Update
			this->Update(Current);
			level->Load();
		}

		level->Init();

		// Game Loop
		while (Current == Next)
		{
			AESysFrameStart();		// 帧开始：封装了PeekMessage
			AEInputUpdate();		// Alpha Engine处理输入
			level->Updata();
			level->Draw();
			AESysFrameEnd();		// 帧结束
		}

		level->Free();

		if (Next != GS_Restart)
			level->UnLoad();

		Previous = Current;
		Current = Next;
	}
	if (Current==GS_Quit)
	{
		System_Exit();
	}
}

void SystemManage::Update(int level)
{
	FreeLevel();
	switch (level)
	{
	case GS_Over:
		this->level = new GameOver;
		break;
	case GS_WIN:
		this->level = new Winner;
		break;
	case GS_MAIN:
		this->level = new MainUi;
		break;
	case GS_L1:
		this->level = new Level1;
		break;
	case GS_L2:
		this->level = new Level2;
		break;
	case GS_MENU:
		this->level = new GameMenu;
	}
}

void SystemManage::FreeLevel()
{
	delete level;
	level = nullptr;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

