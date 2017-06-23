/* 注意:		本项目的Input、math、vec、matrix采用AE提供的函数
	            以上函数应由团队自己完成编码   */

#include "System.h"
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	System_Initialize(hInstance, nCmdShow);

	manage->OpenOrCloseSound(manage->IsPlaying);
	manage->Run();
	
	return 0;
}
