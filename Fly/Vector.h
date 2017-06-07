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
	// 游戏对象类型列表
	TYPE_SHIP = 0,	//对象类型为飞船
	TYPE_BULLET,	//对象类型为子弹
	TYPE_ENEMY,		//对象类型为敌军
	TYPE_SKill,		//对象类型为技能
	TYPE_ENYME_BULLET,//对象类型为敌人子弹
	TYPE_BOSS1,		//BOSS1
	TYPE_NUM
};
// 游戏对象基类/结构
typedef struct
{
	unsigned long		type;		// 游戏对象类型
	AEGfxVertexList*	pMesh;		// 形状
}GameObjBase;

// 游戏对象类/结构
typedef struct GameObj
{
	GameObjBase*		pObject;	// 指向基类（原始形状和类型）
	unsigned long		flag;		// 活动标志
	float				scale;		// 尺寸
	Vec2				posCurr;	// 当前位置
	Vec2				velCurr;	// 当前速度
	float				dirCurr;	// 机头指向
	Matrix				transform;	// 变换矩阵：每一帧都需要为每一个对象计算
	int					live;		//	生命
	float				speed;
}GameObj;

#endif



