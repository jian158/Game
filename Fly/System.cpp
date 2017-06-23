//write by 韦崇健
#include "System.h"
SystemManage *manage;
SystemManage *MenuManage = new SystemManage(GS_MENU);
AESysInitInfo sysInitInfo;
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//------------------------------------------------------------------------------
// 系统初始化

int System_Initialize(HINSTANCE hInstance, int nCmdShow)
{
	manage = new SystemManage;
	// Alpha系统初始化
	sysInitInfo.mAppInstance		= hInstance;	// WinMain的第1个参数
	sysInitInfo.mShow				= nCmdShow;		// WinMain的第4个参数
	sysInitInfo.mWinWidth			= 1024; 
	sysInitInfo.mWinHeight			= 624;
	sysInitInfo.mCreateConsole		= 0;			// 是否需要打开控制台
	sysInitInfo.mCreateWindow		= 1;			// 是否需要创建窗口
	sysInitInfo.mWindowHandle		= NULL;			// 让Alpha缺省处理
	sysInitInfo.mMaxFrameRate		= 60;			// 设置帧率（如果使用Alpha的帧率控制功能的话）
	sysInitInfo.mpWinCallBack		= WndProc;			// 指定窗口过程函数
	sysInitInfo.mClassStyle			= CS_HREDRAW | CS_VREDRAW;		// 窗口类定义的重绘方式									
	sysInitInfo.mWindowStyle		= WS_OVERLAPPEDWINDOW;			// 窗口风格，取值：WS_POPUP | WS_VISIBLE | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	
	if(0 == AESysInit (&sysInitInfo))
		return -1;
	// allocating console for debug
	if( sysInitInfo.mCreateConsole )
	{
		FILE *stream;
		AllocConsole();
		freopen_s( &stream,"CONOUT$", "w", stdout);
		printf("Console is ready for debug");
	}		
	AESysReset();
	return 0;

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		manage->Next = GS_Quit;
		break;
	case WM_DESTROY:
		
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 系统退出
void System_Exit(void)
{
	// Alpha系统退出
	AESysExit();
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------





