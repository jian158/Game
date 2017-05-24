/* 关卡1  */

#include <stdio.h>
#include "GameStateList.h"
#include "System.h"
#include "Level2.h"
#include "AEEngine.h"
#include "AEInput.h"
#include "MyMath.h"
#include <time.h>

#define GAME_OBJ_BASE_NUM_MAX	32			// 对象类型（对象基类）数目上限
#define GAME_OBJ_NUM_MAX		512		// 对象数目上限

#define SHIP_INITIAL_NUM			3		// 飞船的lives数目
#define ASTEROID_NUM				3		// 小行星数目
#define SHIP_SIZE					40.0f	// 飞船尺寸
#define SHIP_ACCEL_FORWARD			50.0f	// 飞船前向加速度(m/s^2)
#define SHIP_ACCEL_BACKWARD			-100.0f	// 飞船后向加速度(m/s^2)
#define SHIP_ROT_SPEED				(2.0f * PI)	// 飞船旋转速度(degree/second)
#define HOMING_MISSILE_ROT_SPEED	(PI / 4.0f)	// 导弹旋转速度(degree/second)


#define FLAG_ACTIVE					0x00000001  // 活动对象标志

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
// 游戏对象基类/结构
typedef struct
{
	unsigned long		type;		// 游戏对象类型
	AEGfxVertexList*	pMesh;		// 形状
}GameObjBase;

// 游戏对象类/结构
typedef struct
{
	GameObjBase*		pObject;	// 指向基类（原始形状和类型）
	unsigned long		flag;		// 活动标志
	float				scale;		// 尺寸
	AEVec2				posCurr;	// 当前位置
	AEVec2				velCurr;	// 当前速度
	float				dirCurr;	// 机头指向
	AEMtx33				transform;	// 变换矩阵：每一帧都需要为每一个对象计算
	int					live;		//	生命
	float				speed;
}GameObj;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables: 
//------------------------------------------------------------------------------
// 游戏对象基类（类型）列表
static GameObjBase		sGameObjBaseList[GAME_OBJ_BASE_NUM_MAX];	// 该数组中的元素是游戏对象基类的实例：形状和类型
static unsigned long	sGameObjBaseNum;							// 已定义的游戏对象基类

																	// 游戏对象列表
static GameObj			sGameObjList[GAME_OBJ_NUM_MAX];				// 该数组中的元素是游戏对象的实例
static unsigned long	sGameObjNum;								// 游戏对象的个数

																	// 飞船对象：因为是Player，所以单独声明，方便程序设计
static GameObj* spShip;

// 剩余的飞船lives (lives 0 = game over)
static long				sShipLives;

// the score = 击毁的小行星asteroid个数
static unsigned long	sScore;

// 只允许一个巡航导弹生成，因此设置一个生成标志
static int flag_missile = 0;
static int missile_target = -1;  // 导弹目标在对象列表中的下标
static long Launch_bl_s = 0, Launch_bl_e = 0;
static long create_enemy_s = 0, create_enemy_e = 0;
static int	Skills = 3;
static AEGfxVertexList*	BgMesh,*mesh_lev1;;
static AEGfxTexture *pTexSp, *pTexBl, *pTexEnemy, *pTexBoss, *pTexBg1, *pTexSkill1, *pTexEbl, *pTexLev;;		// 对象2的纹理
static int			WhenBoss;
static int			FlyMode = 0;
static float		BULLET_SPEED = 100.0f;	// 子弹沿当前方向的速度 (m/s)
static GameObj      *Bg1;
static long Firstime, Lastime;
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
// 创建/删除游戏对象

static GameObj*		gameObjCreate(unsigned long type, float scale, AEVec2* pPos, AEVec2* pVel, float dir);
static void			gameObjDestroy(GameObj* pInst);
static void			CreateEneMy(int count, int quadrant);
static void			CreateBoss(int type);
static void			LaunchBullte(int, float);
static void			CreateSkill();
static float		getDirCur(GameObj *pTarget, GameObj *pInst);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

void Load2(void)
{
	Firstime = clock();
	Lastime = clock();
	GameObjBase* pObjBase;

	// 初始化游戏对象基类的实例列表
	memset(sGameObjBaseList, 0, sizeof(GameObjBase) * GAME_OBJ_BASE_NUM_MAX);
	sGameObjBaseNum = 0;

	// 初始化游戏对象类的实例列表
	memset(sGameObjList, 0, sizeof(GameObj) * GAME_OBJ_NUM_MAX);
	sGameObjNum = 0;

	// 创建基类的实例	
	// =====================
	// 飞船
	// =====================
	pObjBase = sGameObjBaseList + sGameObjBaseNum++;
	pObjBase->type = TYPE_SHIP;
	AEGfxMeshStart();
	//AEGfxTriAdd(
	//	-0.5f, 0.5f, 0x01FF0000, 0.0f, 0.0f,
	//	-0.5f, -0.5f, 0xFFFF0000, 0.0f, 0.0f,
	//	0.5f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		-1.0f, -1.0f, 0xFFFFFFFF, 0, 1.0f,
		1.0f, -1.0f, 0xFFFFFFFF, 1.0f, 1.0f,
		-1.0f, 1.0f, 0xFFFFFFFF, 0, 0);
	AEGfxTriAdd(
		1.0f, -1.0f, 0xFFFFFFFF, 1.0f, 1.0f,
		1.0f, 1.0f, 0xFFFFFFFF, 1.0f, 0,
		-1.0f, 1.0f, 0xFFFFFFFF, 0, 0);
	pObjBase->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObjBase->pMesh, "Failed to create object!!");
	//pTexSp = 

	// =======================
	// 子弹：尺寸很小，简化成三角形定义
	// =======================
	pObjBase = sGameObjBaseList + sGameObjBaseNum++;
	pObjBase->type = TYPE_BULLET;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-1.0f, -1.0f, 0, 0, 1.0f,
		1.0f, -1.0f, 0, 1.0f, 1.0f,
		-1.0f, 1.0f, 0, 0, 0);
	AEGfxTriAdd(
		1.0f, -1.0f, 0, 1.0f, 1.0f,
		1.0f, 1.0f, 0, 1.0f, 0,
		-1.0f, 1.0f, 0, 0, 0);

	pObjBase->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObjBase->pMesh, "Failed to create Bullet object!!");

	// =========================
	// 敌军
	// =========================
	pObjBase = sGameObjBaseList + sGameObjBaseNum++;
	pObjBase->type = TYPE_ENEMY;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-1.0f, -1.0f, 0, 0, 1.0f,
		1.0f, -1.0f, 0, 1.0f, 1.0f,
		-1.0f, 1.0f, 0, 0, 0);
	AEGfxTriAdd(
		1.0f, -1.0f, 0, 1.0f, 1.0f,
		1.0f, 1.0f, 0, 1.0f, 0,
		-1.0f, 1.0f, 0, 0, 0);

	pObjBase->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObjBase->pMesh, "Failed to create Asteroid object!!");


	// ========================
	// 导弹：两个三角形拼接的菱形
	// ========================
	pObjBase = sGameObjBaseList + sGameObjBaseNum++;
	pObjBase->type = TYPE_SKill;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-1.0f, -1.0f, 0, 0, 1.0f,
		1.0f, -1.0f, 0, 1.0f, 1.0f,
		-1.0f, 1.0f, 0, 0, 0);
	AEGfxTriAdd(
		1.0f, -1.0f, 0, 1.0f, 1.0f,
		1.0f, 1.0f, 0, 1.0f, 0,
		-1.0f, 1.0f, 0, 0, 0);

	pObjBase->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObjBase->pMesh, "Failed to create Asteroid object!!");

	// =======================
	// 敌人子弹：尺寸很小，简化成三角形定义
	// =======================
	pObjBase = sGameObjBaseList + sGameObjBaseNum++;
	pObjBase->type = TYPE_ENYME_BULLET;

	AEGfxMeshStart();
	AEGfxMeshStart();
	AEGfxTriAdd(
		-1.0f, -1.0f, 0, 0, 1.0f,
		1.0f, -1.0f, 0, 1.0f, 1.0f,
		-1.0f, 1.0f, 0, 0, 0);
	AEGfxTriAdd(
		1.0f, -1.0f, 0, 1.0f, 1.0f,
		1.0f, 1.0f, 0, 1.0f, 0,
		-1.0f, 1.0f, 0, 0, 0);

	pObjBase->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObjBase->pMesh, "Failed to create Bullet object!!");

	// =========================
	// BOSS1
	// =========================
	pObjBase = sGameObjBaseList + sGameObjBaseNum++;
	pObjBase->type = TYPE_BOSS1;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-1.0f, -1.0f, 0, 0, 1.0f,
		1.0f, -1.0f, 0, 1.0f, 1.0f,
		-1.0f, 1.0f, 0, 0, 0);
	AEGfxTriAdd(
		1.0f, -1.0f, 0, 1.0f, 1.0f,
		1.0f, 1.0f, 0, 1.0f, 0,
		-1.0f, 1.0f, 0, 0, 0);

	pObjBase->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObjBase->pMesh, "Failed to create Asteroid object!!");



	//-------------------------------------------------
	//背景bg1
	//pObjBase = sGameObjBaseList + sGameObjBaseNum++;
	//pObjBase->type = TYPE_BG1;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-512.0f, -312.0f, 0, 0, 1.0f,
		512.0f, -312.0f, 0, 1.0f, 1.0f,
		-512.0f, 312.0f, 0, 0, 0);
	AEGfxTriAdd(
		512.0f, -312.0f, 0, 1.0f, 1.0f,
		512.0f, 321.0f, 0, 1.0f, 0,
		-512.0f, 312.0f, 0, 0, 0);

	BgMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(BgMesh, "Failed to create Asteroid object!!");

	/*第一关提示*/
	AEGfxMeshStart();
	AEGfxTriAdd(
		-300.0f, -100.0f, 0, 0, 1.0f,
		300.0f, -100.0f, 0, 1.0f, 1.0f,
		-300.0f, 100.0f, 0, 0, 0);
	AEGfxTriAdd(
		300.0f, -100.0f, 0, 1.0f, 1.0f,
		300.0f, 100.0f, 0, 1.0f, 0,
		-300.0f, 100.0f, 0, 0, 0);

	mesh_lev1 = AEGfxMeshEnd();
	AE_ASSERT_MESG(mesh_lev1, "Failed to create Asteroid object!!");

	pTexSp = AEGfxTextureLoad("res\\player2.png");
	pTexBl = AEGfxTextureLoad("res\\bullet1.png");
	pTexEnemy = AEGfxTextureLoad("res\\enemy1.png");
	pTexBoss = AEGfxTextureLoad("res\\boss.png");
	pTexBg1 = AEGfxTextureLoad("res\\bg3.jpg");
	pTexSkill1 = AEGfxTextureLoad("res\\skill1.png");
	pTexEbl = AEGfxTextureLoad("res\\enemybl1.png");
	pTexLev = AEGfxTextureLoad("res\\lev2.png");
}

void Ini2(void)
{
	WhenBoss = 0;
	Launch_bl_s = clock();
	Skills = 3;
	srand(time(NULL));
	// 为开始画对象做准备
	//AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	//Bg1= gameObjCreate(TYPE_BG1, 10.0f, 0, 0, 0.0f);
	////AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	//AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	//AEGfxTextureSet(pTexBg1, 0, 0.0f);
	//// 设置对象的2D变换矩阵，使用函数：AEGfxSetTransform
	//AEGfxSetTransform(pObj->transform.m);
	//// 绘制当前对象，使用函数：AEGfxMeshDraw
	//AEGfxMeshDraw(pObj->pObject->pMesh, AE_GFX_MDM_TRIANGLES);


	// 对象实例化：游戏开始只有飞船和小行星需要实例化
	// 飞船对象实例化
	spShip = gameObjCreate(TYPE_SHIP, SHIP_SIZE, 0, 0, 0.0f);
	AE_ASSERT(spShip);
	spShip->posCurr.x = 0;
	spShip->posCurr.y = AEGfxGetWinMinY() + 30;

	spShip->dirCurr = PI / 2;
	// 分数及飞船数目初始化
	sScore = 0;
	sShipLives = SHIP_INITIAL_NUM;
	create_enemy_s = clock();
	CreateEneMy(ASTEROID_NUM, 1);
	CreateEneMy(ASTEROID_NUM, -1);
}

void CreateEneMy(int count, int quadrant)
{
	WhenBoss++;
	GameObj* pObj;
	int i;
	// 敌人对象实例化 并 初始化
	for (i = 0; i < count; i++)
	{
		// 实例化
		pObj = gameObjCreate(TYPE_ENEMY, 0.8f, 0, 0, 0.0f);
		//AE_ASSERT(pObj);
		if (pObj == NULL)
		{
			break;
		}
		// 初始化: 坐标位置 朝向和尺寸大小
		pObj->posCurr.x = quadrant*rand() % (int)AEGfxGetWinMaxX();
		pObj->posCurr.y = AEGfxGetWinMaxY();
		// 朝向
		pObj->dirCurr = -PI / 2;
		pObj->scale = 50.0f;
		// create a enyme_bullet
		GameObj* pBullet;

		pBullet = gameObjCreate(TYPE_ENYME_BULLET, 12.0f, 0, 0, 0.0f);
		if (pBullet == NULL)
		{
			break;
		}
		pBullet->posCurr = pObj->posCurr;
		pBullet->dirCurr = getDirCur(spShip, pObj);
	}
}

void CreateSkill()
{
	GameObj * pBullet;
	for (int i = 0; i<60; i++)
	{
		pBullet = gameObjCreate(TYPE_SKill,20.0f, 0, 0, 0.0f);
		if (pBullet == NULL)
		{
			return;
		}
		pBullet->speed = 200.0f;
		pBullet->posCurr.x = spShip->posCurr.x;
		pBullet->posCurr.y = spShip->posCurr.y;
		pBullet->dirCurr = i*PI / 30;
	}

}
void CreateBoss(int type)
{
	GameObj* pObj;
	WhenBoss++;
	int i;
	// 小行星对象实例化 并 初始化
	// 实例化
	pObj = gameObjCreate(type, 1, 0, 0, 0.0f);
	if (pObj == NULL)
	{
		return;
	}
	pObj->speed = 800.0f;
	// 初始化: 坐标位置 朝向和尺寸大小
	pObj->posCurr.x = 0;
	pObj->posCurr.y = AEGfxGetWinMaxY();
	// 朝向
	pObj->dirCurr = -PI / 2;
	pObj->scale = 50.0f;
	// create a enyme_bullet
	GameObj* pBullet;

	pBullet = gameObjCreate(TYPE_ENYME_BULLET, 12.0f, 0, 0, 0.0f);
	if (pObj == NULL)
	{
		return;
	}
	pBullet->posCurr = pObj->posCurr;
	pBullet->dirCurr = getDirCur(spShip, pObj);
}

void LaunchBullte(int type, float scale)
{
	for (int i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj* pInst = sGameObjList + i;
		// 不理会非活动对象
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;
		if (pInst->flag != 0)
		{
			if (pInst->pObject->type == TYPE_ENEMY)
			{
				GameObj * pBullet = gameObjCreate(TYPE_ENYME_BULLET, 12.0f, 0, 0, 0.0f);
				if (pBullet == NULL)
				{
					break;
				}
				pBullet->posCurr = pInst->posCurr;
				pBullet->dirCurr = getDirCur(spShip, pInst);
			}
			else if ((pInst->pObject->type == TYPE_BOSS1))
			{
				for (int j = -5; j<6; j++)
				{
					GameObj * pBullet = gameObjCreate(TYPE_ENYME_BULLET, 12.0f, 0, 0, 0.0f);
					if (pBullet == NULL)
					{
						break;
					}
					pBullet->posCurr.x = pInst->posCurr.x + j * 80;
					pBullet->posCurr.y = pInst->posCurr.y;
					pBullet->dirCurr = getDirCur(spShip, pInst);
				}
				for (int j = -5; j<6; j++)
				{
					GameObj * pBullet = gameObjCreate(TYPE_ENYME_BULLET, 12.0f, 0, 0, 0.0f);
					if (pBullet == NULL)
					{
						break;
					}
					pBullet->posCurr.x = pInst->posCurr.x + j * 80;
					pBullet->posCurr.y = pInst->posCurr.y - 30;
					pBullet->dirCurr = getDirCur(spShip, pInst);
				}
				break;
			}
		}
	}
}


void Update2(void)
{
	Launch_bl_e = clock();			//计时器，2000秒敌方放一次弹
	if (Launch_bl_e - Launch_bl_s>1500)
	{
		Launch_bl_s = Launch_bl_e;
		LaunchBullte(TYPE_ENYME_BULLET, 10.0f);
	}

	create_enemy_e = clock();
	if (create_enemy_e - create_enemy_s>8000)
	{
		create_enemy_s = create_enemy_e;
		CreateEneMy(ASTEROID_NUM, 1);
		CreateEneMy(ASTEROID_NUM, -1);
	}

	if (WhenBoss == 4)
	{
		CreateBoss(TYPE_BOSS1);
	}
	// ======================
	// 帧时间：Unity中的dt
	// ======================
	double frameTime = AEFrameRateControllerGetFrameTime();

	// =========================
	// 游戏逻辑响应输入
	// =========================

	// 状态切换
	if (AEInputCheckTriggered('R'))
	{
		Next = GS_Restart;
		return;
	}
	if (AEInputCheckTriggered(VK_ESCAPE))
	{
		if (MessageBox(NULL, "是否退出", "提示", MB_OKCANCEL) == IDOK)
		{
			Next = GS_Quit;
			return;
		}

	}
	if (AEInputCheckTriggered('2'))
	{
		Next = GS_L2;
		return;
	}

	// 飞船移动
	if (AEInputCheckCurr(VK_UP) && spShip->posCurr.y<AEGfxGetWinMaxY())
	{
		spShip->dirCurr = PI / 2;
		AEVec2 added;
		AEVec2Set(&added, cosf(spShip->dirCurr), sinf(spShip->dirCurr));
		AEVec2Add(&spShip->posCurr, &spShip->posCurr, &added);
		spShip->velCurr.x = 5;
		spShip->velCurr.y = 5;

		// 根据新速度更新位置
		//spShip->posCurr.x += added.x * spShip->velCurr.x * 0.95f;
		spShip->posCurr.y += added.y * spShip->velCurr.y * 0.95f;
	}

	if (AEInputCheckCurr(VK_DOWN) && spShip->posCurr.y>AEGfxGetWinMinY())
	{
		spShip->dirCurr = PI / 2;
		AEVec2 added;
		AEVec2Set(&added, cosf(spShip->dirCurr), sinf(spShip->dirCurr));
		AEVec2Add(&spShip->posCurr, &spShip->posCurr, &added);
		spShip->velCurr.x = 5;
		spShip->velCurr.y = -5;

		// 位置更新
		//spShip->posCurr.x -= added.x * spShip->velCurr.x * 0.95f;
		spShip->posCurr.y += added.y * spShip->velCurr.y * 0.95f;
	}

	if (AEInputCheckCurr(VK_LEFT) && spShip->posCurr.x>AEGfxGetWinMinX())
	{
		spShip->dirCurr = PI / 2;
		AEVec2 added;
		AEVec2Set(&added, cosf(spShip->dirCurr * 2), sinf(spShip->dirCurr * 2));
		AEVec2Add(&spShip->posCurr, &spShip->posCurr, &added);
		spShip->velCurr.x = -5;
		//spShip->velCurr.y = -3;
		// 位置更新
		spShip->posCurr.x -= added.x * spShip->velCurr.x * 0.95f;
	}

	if (AEInputCheckCurr(VK_RIGHT) && spShip->posCurr.x<AEGfxGetWinMaxX())
	{
		spShip->dirCurr = PI / 2;
		AEVec2 added;
		AEVec2Set(&added, cosf(0), sinf(0));
		AEVec2Add(&spShip->posCurr, &spShip->posCurr, &added);
		spShip->velCurr.x = 5;
		spShip->velCurr.y = 5;

		// 位置更新
		spShip->posCurr.x += added.x * spShip->velCurr.x * 0.95f;
		//spShip->posCurr.y += added.y * spShip->velCurr.y * 0.95f;
	}

	// 空格键射击(创建一个子弹对象)
	if (AEInputCheckTriggered(VK_SPACE))
	{
		//PlaySound(TEXT("sound\\bullet1.wav"), NULL, SND_FILENAME | SND_ASYNC);
		// create a bullet
		GameObj * pBullet = gameObjCreate(TYPE_BULLET, 10.0f, 0, 0, 0.0f);
		if (pBullet != NULL)
		{
			pBullet->posCurr = spShip->posCurr;
			pBullet->dirCurr = spShip->dirCurr;
			pBullet->speed = 200.0f;
		}


		pBullet = gameObjCreate(TYPE_BULLET, 10.0f, 0, 0, 0.0f);
		if (pBullet != NULL)
		{
			pBullet->posCurr.x = spShip->posCurr.x - 20;
			pBullet->posCurr.y = spShip->posCurr.y;
			pBullet->dirCurr = spShip->dirCurr;
			pBullet->speed = 200.0f;
		}

		pBullet = gameObjCreate(TYPE_BULLET, 10.0f, 0, 0, 0.0f);
		if (pBullet != NULL)
		{
			pBullet->posCurr.x = spShip->posCurr.x + 20;
			pBullet->posCurr.y = spShip->posCurr.y;
			pBullet->dirCurr = spShip->dirCurr;
			pBullet->speed = 200.0f;
		}

	}

	// M键发射导弹
	if (AEInputCheckTriggered('D') && Skills>0)
	{
		Skills--;
		CreateSkill();
	}

	// ==================================================
	// 更新所有其它（非player控制）活动对象的（位置等）
	// ==================================================
	for (int i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj* pInst = sGameObjList + i;
		AEVec2 added;

		// 遇到非活动对象，不处理
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		// 更新敌人位置
		if (pInst->pObject->type == TYPE_ENEMY)
		{
			AEVec2Set(&added, 0.5* cosf(-PI / 2 * (rand() % 10 / 10)), sinf(-PI / 2));
			AEVec2Add(&pInst->posCurr, &pInst->posCurr, &added);
		}

		if (pInst->pObject->type == TYPE_BOSS1)
		{
			if (FlyMode == 0)
			{
				AEVec2Set(&added, 2 * cosf(-PI / 2 * (rand() % 10 / 10)), sinf(-PI / 2));
				if (pInst->posCurr.y<0)
				{
					FlyMode = 1;
				}
			}
			else if (FlyMode == 1)
			{
				AEVec2Set(&added, 2 * cosf(-PI / 2 * (rand() % 10 / 10)), sinf(PI / 2));
				if (pInst->posCurr.y>AEGfxGetWinMaxY() - 30)
				{
					FlyMode = 0;
				}
			}
			AEVec2Add(&pInst->posCurr, &pInst->posCurr, &added);
			/*FILE *stream;
			AllocConsole();
			freopen_s(&stream, "CONOUT$", "w", stdout);
			printf("y:%f\n", pInst->posCurr.y);*/
		}

		// 更新子弹位置
		if (pInst->pObject->type == TYPE_BULLET)
		{
			AEVec2Set(&added, pInst->speed * (float)(frameTime)* cosf(pInst->dirCurr), pInst->speed * (float)(frameTime)* sinf(pInst->dirCurr));
			AEVec2Add(&pInst->posCurr, &pInst->posCurr, &added);
		}

		if (pInst->pObject->type == TYPE_ENYME_BULLET)
		{
			AEVec2Set(&added, pInst->speed * (float)(frameTime)* cosf(pInst->dirCurr), pInst->speed * (float)(frameTime)* sinf(pInst->dirCurr));
			AEVec2Add(&pInst->posCurr, &pInst->posCurr, &added);
		}

		// 更新导弹位置
		if (pInst->pObject->type == TYPE_SKill)
		{
			/*AEVec2Set(&added, 100.0f * (float)(frameTime) * cosf(pInst->dirCurr), 100.0f * (float)(frameTime) * sinf(pInst->dirCurr));
			AEVec2Add(&pInst->posCurr, &pInst->posCurr, &added);*/
			AEVec2Set(&added, pInst->speed * (float)(frameTime)* cosf(pInst->dirCurr), pInst->speed * (float)(frameTime)* sinf(pInst->dirCurr));
			AEVec2Add(&pInst->posCurr, &pInst->posCurr, &added);
		}
	}

	// ===================================
	// 其它更新
	// Example:
	//		-- 超出屏幕在对向出现（飞船 小行星）
	//		-- 超出屏幕删除（子弹）
	//		-- 导弹：计算新方向
	// ===================================
	Check();
}


static void Check()
{

	unsigned long i;
	float round;

	// ==========================================================================================
	// 获取窗口四条边的坐标，当窗口发生移动或缩放，以下值会发生变化
	// ==========================================================================================
	float winMaxX = AEGfxGetWinMaxX();
	float winMaxY = AEGfxGetWinMaxY();
	float winMinX = AEGfxGetWinMinX();
	float winMinY = AEGfxGetWinMinY();
	for (i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj* pInst = sGameObjList + i;

		// 不理会非活动对象
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		// 飞船：Wrap
		if (pInst->pObject->type == TYPE_SHIP)
		{
			pInst->posCurr.x = AEWrap(pInst->posCurr.x, winMinX - SHIP_SIZE, winMaxX + SHIP_SIZE);
			pInst->posCurr.y = AEWrap(pInst->posCurr.y, winMinY - SHIP_SIZE, winMaxY + SHIP_SIZE);
			continue;
		}

		// 敌方小兵：Wrap
		if (pInst->pObject->type == TYPE_ENEMY || pInst->pObject->type == TYPE_BOSS1)
		{
			pInst->posCurr.x = AEWrap(pInst->posCurr.x, winMinX - pInst->scale, winMaxX + pInst->scale);
			pInst->posCurr.y = AEWrap(pInst->posCurr.y, winMinY - pInst->scale, winMaxY + pInst->scale);
			continue;
		}

		// 删除超出屏幕边界的子弹
		if ((pInst->posCurr.x < winMinX) || (pInst->posCurr.x > winMaxX) || (pInst->posCurr.y < winMinY) || (pInst->posCurr.y > winMaxY))
			pInst->flag = 0;
	}

	// ====================
	// 碰撞检测
	// ====================
	for (i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj* pInst = sGameObjList + i;

		// 不处理非活动对象
		if ((pInst->flag && FLAG_ACTIVE) == 0)
			continue;

		// 敌人 与 飞船 / 子弹/ 技能 的碰撞检测
		if (pInst->pObject->type == TYPE_ENEMY || pInst->pObject->type == TYPE_ENYME_BULLET || pInst->pObject->type == TYPE_BOSS1)
		{
			for (int j = 0; j < GAME_OBJ_NUM_MAX; j++)
			{
				GameObj * pInstOther = sGameObjList + j;

				// 跳过非活动对象和小行星自身
				if ((pInstOther->flag && FLAG_ACTIVE) == 0 || (pInstOther->pObject->type == TYPE_ENEMY))
					continue;

				// asteroid vs. ship
				if (pInstOther->pObject->type == TYPE_SHIP)
				{
					// 碰撞检测
					if (AETestCircleToCircle(&(pInst->posCurr), pInst->scale, &(pInstOther->posCurr), pInstOther->scale))
					{
						spShip->live -= 25;
						if (spShip->live <= 0)
						{
							sShipLives -= 1;// 飞船击毁
						}
						else if (pInst->pObject->type != TYPE_BOSS1)
						{
							pInst->flag = 0;
							continue;
						}
						else
						{
							continue;
						}
						if (sShipLives <= 0)
						{
							// 重新开始关卡
							Next = GS_Over;
						}
						else
						{
							//PlaySound(TEXT("sound\\boom.wav"), NULL, SND_FILENAME | SND_ASYNC);
							// 更新位置
							spShip->live = 100;
							pInstOther->posCurr.x = 0;
							pInstOther->posCurr.y = winMinY + 30.0f;
							CreateSkill();
						}
					}
					continue;
				}
				// asteroid vs. bullet
				if (pInstOther->pObject->type == TYPE_BULLET)
				{
					//round = sqrt(powf(pInst->posCurr.y - pInstOther->posCurr.y, 2) + powf(pInst->posCurr.x - pInstOther->posCurr.x, 2));
					/*FILE *stream;
					AllocConsole();
					freopen_s(&stream, "CONOUT$", "w", stdout);
					printf("y:%f\n", round);*/
					//round = round-pInst->scale - pInstOther->scale ;
					// 发生碰撞，则两者都销毁AETestCircleToCircle(&(pInst->posCurr),pInst->scale,&(pInstOther->posCurr),pInstOther->scale)

					if (IsCrash(pInst->posCurr, pInstOther->posCurr, pInst->scale, pInstOther->scale))
					{
						pInst->live -= 2;
						if (pInst->pObject->type == TYPE_BOSS1&&pInst->live <= 0)
						{
							pInst->flag = 0;
							Next = GS_WIN;
						}
						else if (pInst->pObject->type != TYPE_BOSS1)
						{
							pInst->flag = 0;
						}
						pInstOther->flag = 0;
					}
					continue;
				}

				// asteroid vs. missile
				if (pInstOther->pObject->type == TYPE_SKill)
				{
					// collision detection
					if (AETestCircleToCircle(&(pInst->posCurr), pInst->scale, &(pInstOther->posCurr), pInstOther->scale))
					{
						pInst->live -= 5;
						if (pInst->pObject->type == TYPE_BOSS1&&pInst->live <= 0)
						{
							pInst->flag = 0;
							Next = GS_WIN;
						}
						else if (pInst->pObject->type == TYPE_BOSS1)
						{
							pInstOther->flag = 0;
						}
						else if (pInst->pObject->type != TYPE_BOSS1)
						{
							pInst->flag = 0;
						}
						//pInstOther->flag = 0;
					}
				}
			}
		}

	}

	// =====================================
	// 计算所有对象的2D变换矩阵
	// =====================================
	for (i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		AEMtx33		 trans, rot, scale;
		GameObj* pInst = sGameObjList + i;

		// 不处理非活动对象
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		// 缩放矩阵
		AEMtx33Scale(&scale, pInst->scale, pInst->scale);
		// 旋转矩阵
		AEMtx33Rot(&rot, pInst->dirCurr);
		// 平移矩阵
		AEMtx33Trans(&trans, pInst->posCurr.x, pInst->posCurr.y);
		// 以正确的顺序连乘以上3个矩阵形成2维变换矩阵
		AEMtx33Concat(&(pInst->transform), &trans, &rot);
		AEMtx33Concat(&(pInst->transform), &(pInst->transform), &scale);
	}
}

float getDirCur(GameObj *pTarget, GameObj *pInst)
{
	AEVec2 newv;
	float angle;
	AEVec2Sub(&newv, &(pTarget->posCurr), &(pInst->posCurr));
	if (newv.x != 0)
		angle = atanf(newv.y / newv.x);
	else
		angle = 0;

	// 差向量在第二、三象限
	if ((newv.x < 0 && newv.y < 0) || (newv.x < 0 && newv.y > 0))
		angle += PI;
	// 差向量在第四象限
	if (newv.x > 0 && newv.y < 0)
		angle = 2 * PI + angle;
	return angle;
}

void Draw2(void)
{
	unsigned long i;
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(0, 0);
	AEGfxTextureSet(pTexBg1, 0, 0.0f);
	AEGfxSetTransparency(1);
	AEGfxMeshDraw(BgMesh, AE_GFX_MDM_TRIANGLES);

	if (Lastime - Firstime<1500)
	{
		Lastime = clock();
		AEGfxTextureSet(pTexLev, 0.0f, 0.0f);
		AEGfxMeshDraw(mesh_lev1, AE_GFX_MDM_TRIANGLES);
		clock();
	}

	// 逐个绘制对象列表中的所有对象
	for (i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj* pInst = sGameObjList + i;

		// 跳过非活动对象
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;
		if (pInst->pObject->type == TYPE_SHIP)
		{
			//AEGfxSetBlendMode(AE_GFX_BM_BLEND);
			//AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxTextureSet(pTexSp, 0, 0.0f);
		}
		else if (pInst->pObject->type == TYPE_BULLET)
		{
			//AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxTextureSet(pTexBl, 0, 0.0f);
		}
		else if (pInst->pObject->type == TYPE_ENEMY)
		{
			//AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxTextureSet(pTexEnemy, 0, 0.0f);
		}
		else if (pInst->pObject->type == TYPE_BOSS1)
		{
			//AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxTextureSet(pTexBoss, 0, 0.0f);
		}
		else if (pInst->pObject->type == TYPE_SKill)
		{
			//AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxTextureSet(pTexSkill1, 0, 0.0f);
		}
		/*	else if (pInst->pObject->type == TYPE_BG1)
		{

		}*/
		else
		{
			AEGfxTextureSet(pTexEbl, 0, 0.0f);
			//AEGfxSetRenderMode(AE_GFX_RM_COLOR);
			//AEGfxSetBlendMode(AE_GFX_RM_COLOR);
		}
		// 设置对象的2D变换矩阵，使用函数：AEGfxSetTransform
		AEGfxSetTransform(pInst->transform.m);
		// 绘制当前对象，使用函数：AEGfxMeshDraw
		AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
	}
}

void Free2(void)
{
	int i = 0;

	// 使用函数gameObjDestroy删除列表中的对象
	for (i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj* pInst = sGameObjList + i;
		gameObjDestroy(pInst);
	}
}

void Unload2(void)
{
	AEGfxTextureUnload(pTexBg1);
	AEGfxTextureUnload(pTexSp);
	AEGfxTextureUnload(pTexBl);
	AEGfxTextureUnload(pTexEnemy);
	AEGfxTextureUnload(pTexBoss);
	AEGfxTextureUnload(pTexSkill1);
	AEGfxMeshFree(BgMesh);
	AEGfxTextureUnload(pTexEbl);

	AEGfxMeshFree(mesh_lev1);
	AEGfxTextureUnload(pTexLev);
	// 卸载对象形状定义资源，使用函数：AEGfxMeshFree
	for (int i = 0; i < GAME_OBJ_BASE_NUM_MAX; i++)
	{
		GameObjBase* pGamObjBase = sGameObjBaseList + i;
		AEGfxMeshFree(pGamObjBase->pMesh);
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
GameObj* gameObjCreate(unsigned long type, float scale, AEVec2* pPos, AEVec2* pVel, float dir)
{
	AEVec2 zero = { 0.0f, 0.0f };

	//AE_ASSERT_PARM(type < sGameObjBaseNum);

	// 找游戏对象列表中没用过的位置
	for (int i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj* pInst = sGameObjList + i;

		// 找非活动对象的位置
		if (pInst->flag == 0)
		{
			// 找到了
			pInst->pObject = sGameObjBaseList + type;
			pInst->flag = FLAG_ACTIVE;
			pInst->scale = scale;
			pInst->posCurr = pPos ? *pPos : zero;
			pInst->velCurr = pVel ? *pVel : zero;
			pInst->dirCurr = dir;
			pInst->live = 100;
			pInst->speed = 100.0f;
			// 返回新创建的对象实例
			return pInst;
		}
	}

	// 位置满了
	return NULL;
}

// ---------------------------------------------------------------------------

void gameObjDestroy(GameObj* pInst)
{
	// 若已经销毁
	if (pInst->flag == 0)
		return;

	// 销毁
	pInst->flag = 0;
}