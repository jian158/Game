/* ע��:		����Ŀ��Input��math��vec��matrix����AE�ṩ�ĺ���
	            ���Ϻ���Ӧ���Ŷ��Լ���ɱ���   */

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
	//		AESysFrameStart();		// ֡��ʼ����װ��PeekMessage
	//		AEInputUpdate();		// Alpha Engine��������
	//		pUpdate();
	//		pDraw();
	//		AESysFrameEnd();		// ֡����
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
