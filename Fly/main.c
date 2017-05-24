/* ע��:		����Ŀ��Input��math��vec��matrix����AE�ṩ�ĺ���
	            ���Ϻ���Ӧ���Ŷ��Լ���ɱ���   */

#include "System.h"
#include <Windows.h>
#include "GameStateList.h"
#include "GameStateManager.h"
#include "AEEngine.h"
#include <mmsystem.h>
#pragma comment (lib, "Winmm.lib")

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// System Initialize
	System_Initialize(hInstance, nCmdShow);

	// GSM Initialize
	GSM_Initialize(GS_MAIN);
	PlaySound(TEXT("back.wav"), NULL, SND_FILENAME | SND_ASYNC|SND_LOOP);
	// Game Flow
	while(Current != GS_Quit)
	{
		if(Current == GS_Restart)
		{
			Current = Previous;
			Next = Previous;
		}
		else
		{
			// GSM Update
			GSM_Update(Current);
			pLoad();
		}

		pIni();

		// Game Loop
		while(Current == Next)
		{
			AESysFrameStart();		// ֡��ʼ����װ��PeekMessage
			AEInputUpdate();		// Alpha Engine��������
			pUpdate();
			pDraw();
			AESysFrameEnd();		// ֡����
		}

		pFree();

		if(Next != GS_Restart)
			pUnload();

		Previous = Current;
		Current = Next;
	}

	// System Exit
	System_Exit();

	return 0;
}
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------