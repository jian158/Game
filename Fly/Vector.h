#pragma once
#ifndef _Vector
#define _Vector
typedef struct Matrix
{
	float m[3][3];
}Matrix;

typedef struct Vec2
{
	float x, y;
}Vec2;

enum TYPE
{
	// ��Ϸ���������б�
	TYPE_SHIP = 0,	//��������Ϊ�ɴ�
	TYPE_BULLET,	//��������Ϊ�ӵ�
	TYPE_ENEMY,		//��������Ϊ�о�
	TYPE_SKill,		//��������Ϊ����
	TYPE_ENYME_BULLET,//��������Ϊ�����ӵ�
	TYPE_BOSS1,		//BOSS1
	TYPE_NUM
};
// ��Ϸ�������/�ṹ
typedef struct
{
	unsigned long		type;		// ��Ϸ��������
	AEGfxVertexList*	pMesh;		// ��״
}GameObjBase;

// ��Ϸ������/�ṹ
typedef struct GameObj
{
	GameObjBase*		pObject;	// ָ����ࣨԭʼ��״�����ͣ�
	unsigned long		flag;		// ���־
	float				scale;		// �ߴ�
	Vec2				posCurr;	// ��ǰλ��
	Vec2				velCurr;	// ��ǰ�ٶ�
	float				dirCurr;	// ��ͷָ��
	Matrix				transform;	// �任����ÿһ֡����ҪΪÿһ���������
	int					live;		//	����
	float				speed;
}GameObj;

#endif



