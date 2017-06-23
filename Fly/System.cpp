//write by Τ�罡
#include "System.h"
SystemManage *manage;
SystemManage *MenuManage = new SystemManage(GS_MENU);
AESysInitInfo sysInitInfo;
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//------------------------------------------------------------------------------
// ϵͳ��ʼ��

int System_Initialize(HINSTANCE hInstance, int nCmdShow)
{
	manage = new SystemManage;
	// Alphaϵͳ��ʼ��
	sysInitInfo.mAppInstance		= hInstance;	// WinMain�ĵ�1������
	sysInitInfo.mShow				= nCmdShow;		// WinMain�ĵ�4������
	sysInitInfo.mWinWidth			= 1024; 
	sysInitInfo.mWinHeight			= 624;
	sysInitInfo.mCreateConsole		= 0;			// �Ƿ���Ҫ�򿪿���̨
	sysInitInfo.mCreateWindow		= 1;			// �Ƿ���Ҫ��������
	sysInitInfo.mWindowHandle		= NULL;			// ��Alphaȱʡ����
	sysInitInfo.mMaxFrameRate		= 60;			// ����֡�ʣ����ʹ��Alpha��֡�ʿ��ƹ��ܵĻ���
	sysInitInfo.mpWinCallBack		= WndProc;			// ָ�����ڹ��̺���
	sysInitInfo.mClassStyle			= CS_HREDRAW | CS_VREDRAW;		// �����ඨ����ػ淽ʽ									
	sysInitInfo.mWindowStyle		= WS_OVERLAPPEDWINDOW;			// ���ڷ��ȡֵ��WS_POPUP | WS_VISIBLE | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	
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

// ϵͳ�˳�
void System_Exit(void)
{
	// Alphaϵͳ�˳�
	AESysExit();
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------





