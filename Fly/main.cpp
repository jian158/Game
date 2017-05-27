/* 注意:		本项目的Input、math、vec、matrix采用AE提供的函数
	            以上函数应由团队自己完成编码   */

#include "System.h"
#pragma comment (lib, "Winmm.lib")
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// System Initialize
	System_Initialize(hInstance, nCmdShow);

	// GSM Initialize
//	PlaySound(TEXT("back.wav"), NULL, SND_FILENAME | SND_ASYNC|SND_LOOP);
	// Game Flow
	//while(Current != GS_Quit)
	//{
	//	if(Current == GS_Restart)
	//	{
	//		Current = Previous;
	//		Next = Previous;
	//	}
	//	else
	//	{
	//		// GSM Update
	//		GSM_Update(Current);
	//		pLoad();
	//	}

	//	pIni();

	//	// Game Loop
	//	while(Current == Next)
	//	{
	//		AESysFrameStart();		// 帧开始：封装了PeekMessage
	//		AEInputUpdate();		// Alpha Engine处理输入
	//		pUpdate();
	//		pDraw();
	//		AESysFrameEnd();		// 帧结束
	//	}

	//	pFree();

	//	if(Next != GS_Restart)
	//		pUnload();

	//	Previous = Current;
	//	Current = Next;
	//}

	// System Exit
	manage->Run();

	return 0;
}
