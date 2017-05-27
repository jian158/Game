#include "System.h"

#pragma comment (lib, "Alpha_Engine.lib")
SystemManage *manage;
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
	sysInitInfo.mpWinCallBack		= NULL;			// ָ�����ڹ��̺���
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

// ϵͳ�˳�
void System_Exit(void)
{
	PlaySound(0, 0, SND_FILENAME);
	// Alphaϵͳ�˳�
	AESysExit();
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------





