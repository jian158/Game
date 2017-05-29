/*ϵͳ������*/
#pragma once
#ifndef _System
#define _System
#include <Windows.h>
#include "AEEngine.h"
#include "MyMath.h"
#include "LevelBase.h"
#include "GameStateManager.h"
#include "GameStateList.h"
#include <time.h>
#include "SoundSystem.h"
#pragma comment (lib, "fmod/fmodex_vc.lib")
#pragma comment (lib, "fmod/fmodexL_vc.lib")
enum TYPE
{
	// ��Ϸ���������б�
	TYPE_SHIP = 0,	//��������Ϊ�ɴ�
	TYPE_BULLET,	//��������Ϊ�ӵ�
	TYPE_ENEMY,		//��������Ϊ�о�
	TYPE_SKill,		//��������Ϊ����
	TYPE_ENYME_BULLET,//��������Ϊ�����ӵ�
	TYPE_BOSS1,		//BOSS1
	TYPE_BOSS2,		//BOSS2
	TYPE_BG1,
	TYPE_NUM
};
// ��Ϸ�������/�ṹ
typedef struct
{
	unsigned long		type;		// ��Ϸ��������
	AEGfxVertexList*	pMesh;		// ��״
}GameObjBase;

// ��Ϸ������/�ṹ
typedef struct
{
	GameObjBase*		pObject;	// ָ����ࣨԭʼ��״�����ͣ�
	unsigned long		flag;		// ���־
	float				scale;		// �ߴ�
	AEVec2				posCurr;	// ��ǰλ��
	AEVec2				velCurr;	// ��ǰ�ٶ�
	float				dirCurr;	// ��ͷָ��
	AEMtx33				transform;	// �任����ÿһ֡����ҪΪÿһ���������
	int					live;		//	����
	float				speed;
}GameObj;
// ϵͳ��ʼ��
extern int System_Initialize(HINSTANCE hInstance, int nCmdShow);

// ϵͳ�˳�
extern void System_Exit(void);
extern SystemManage *manage;
extern SystemManage *MenuManage;
#endif