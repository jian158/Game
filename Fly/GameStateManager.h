/*��Ϸ״̬������*/
#pragma once
#ifndef _GameStateManager
#define _GameStateManager


// ��Ϸ״̬�������ĳ�ʼ��
extern void GSM_Initialize(int);

// ��Ϸ״̬�������ĸ���
extern void GSM_Update(int);
// 6 Function Pointers
extern void (*pLoad)(void);
extern void (*pIni)(void);
extern void (*pUpdate)(void);
extern void (*pDraw)(void);
extern void (*pFree)(void);
extern void (*pUnload)(void);

#endif