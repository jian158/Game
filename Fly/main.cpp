/* ע��:		����Ŀ��Input��math��vec��matrix����AE�ṩ�ĺ���
	            ���Ϻ���Ӧ���Ŷ��Լ���ɱ���   */

#include "System.h"
#pragma comment (lib, "Winmm.lib")
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	System_Initialize(hInstance, nCmdShow);

//	PlaySound(TEXT("back.wav"), NULL, SND_FILENAME | SND_ASYNC|SND_LOOP);
	manage->OpenOrCloseSound(manage->IsPlaying);
	manage->Run();
	
	return 0;
}
