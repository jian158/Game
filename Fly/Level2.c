/* �ؿ�1  */

#include <stdio.h>
#include "GameStateList.h"
#include "System.h"
#include "Level2.h"
#include "AEEngine.h"
#include "AEInput.h"
#include "MyMath.h"
#include <time.h>

#define GAME_OBJ_BASE_NUM_MAX	32			// �������ͣ�������ࣩ��Ŀ����
#define GAME_OBJ_NUM_MAX		512		// ������Ŀ����

#define SHIP_INITIAL_NUM			3		// �ɴ���lives��Ŀ
#define ASTEROID_NUM				3		// С������Ŀ
#define SHIP_SIZE					40.0f	// �ɴ��ߴ�
#define SHIP_ACCEL_FORWARD			50.0f	// �ɴ�ǰ����ٶ�(m/s^2)
#define SHIP_ACCEL_BACKWARD			-100.0f	// �ɴ�������ٶ�(m/s^2)
#define SHIP_ROT_SPEED				(2.0f * PI)	// �ɴ���ת�ٶ�(degree/second)
#define HOMING_MISSILE_ROT_SPEED	(PI / 4.0f)	// ������ת�ٶ�(degree/second)


#define FLAG_ACTIVE					0x00000001  // ������־

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables: 
//------------------------------------------------------------------------------
// ��Ϸ������ࣨ���ͣ��б�
static GameObjBase		sGameObjBaseList[GAME_OBJ_BASE_NUM_MAX];	// �������е�Ԫ������Ϸ��������ʵ������״������
static unsigned long	sGameObjBaseNum;							// �Ѷ������Ϸ�������

																	// ��Ϸ�����б�
static GameObj			sGameObjList[GAME_OBJ_NUM_MAX];				// �������е�Ԫ������Ϸ�����ʵ��
static unsigned long	sGameObjNum;								// ��Ϸ����ĸ���

																	// �ɴ�������Ϊ��Player�����Ե�������������������
static GameObj* spShip;

// ʣ��ķɴ�lives (lives 0 = game over)
static long				sShipLives;

// the score = ���ٵ�С����asteroid����
static unsigned long	sScore;

// ֻ����һ��Ѳ���������ɣ��������һ�����ɱ�־
static int flag_missile = 0;
static int missile_target = -1;  // ����Ŀ���ڶ����б��е��±�
static long Launch_bl_s = 0, Launch_bl_e = 0;
static long create_enemy_s = 0, create_enemy_e = 0;
static int	Skills = 3;
static AEGfxVertexList*	BgMesh,*mesh_lev1;;
static AEGfxTexture *pTexSp, *pTexBl, *pTexEnemy, *pTexBoss, *pTexBg1, *pTexSkill1, *pTexEbl, *pTexLev;;		// ����2������
static int			WhenBoss;
static int			FlyMode = 0;
static float		BULLET_SPEED = 100.0f;	// �ӵ��ص�ǰ������ٶ� (m/s)
static GameObj      *Bg1;
static long Firstime, Lastime;
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
// ����/ɾ����Ϸ����

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

	// ��ʼ����Ϸ��������ʵ���б�
	memset(sGameObjBaseList, 0, sizeof(GameObjBase) * GAME_OBJ_BASE_NUM_MAX);
	sGameObjBaseNum = 0;

	// ��ʼ����Ϸ�������ʵ���б�
	memset(sGameObjList, 0, sizeof(GameObj) * GAME_OBJ_NUM_MAX);
	sGameObjNum = 0;

	// ���������ʵ��	
	// =====================
	// �ɴ�
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
	// �ӵ����ߴ��С���򻯳������ζ���
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
	// �о�
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
	// ����������������ƴ�ӵ�����
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
	// �����ӵ����ߴ��С���򻯳������ζ���
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
	//����bg1
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

	/*��һ����ʾ*/
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
	// Ϊ��ʼ��������׼��
	//AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	//Bg1= gameObjCreate(TYPE_BG1, 10.0f, 0, 0, 0.0f);
	////AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	//AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	//AEGfxTextureSet(pTexBg1, 0, 0.0f);
	//// ���ö����2D�任����ʹ�ú�����AEGfxSetTransform
	//AEGfxSetTransform(pObj->transform.m);
	//// ���Ƶ�ǰ����ʹ�ú�����AEGfxMeshDraw
	//AEGfxMeshDraw(pObj->pObject->pMesh, AE_GFX_MDM_TRIANGLES);


	// ����ʵ��������Ϸ��ʼֻ�зɴ���С������Ҫʵ����
	// �ɴ�����ʵ����
	spShip = gameObjCreate(TYPE_SHIP, SHIP_SIZE, 0, 0, 0.0f);
	AE_ASSERT(spShip);
	spShip->posCurr.x = 0;
	spShip->posCurr.y = AEGfxGetWinMinY() + 30;

	spShip->dirCurr = PI / 2;
	// �������ɴ���Ŀ��ʼ��
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
	// ���˶���ʵ���� �� ��ʼ��
	for (i = 0; i < count; i++)
	{
		// ʵ����
		pObj = gameObjCreate(TYPE_ENEMY, 0.8f, 0, 0, 0.0f);
		//AE_ASSERT(pObj);
		if (pObj == NULL)
		{
			break;
		}
		// ��ʼ��: ����λ�� ����ͳߴ��С
		pObj->posCurr.x = quadrant*rand() % (int)AEGfxGetWinMaxX();
		pObj->posCurr.y = AEGfxGetWinMaxY();
		// ����
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
	// С���Ƕ���ʵ���� �� ��ʼ��
	// ʵ����
	pObj = gameObjCreate(type, 1, 0, 0, 0.0f);
	if (pObj == NULL)
	{
		return;
	}
	pObj->speed = 800.0f;
	// ��ʼ��: ����λ�� ����ͳߴ��С
	pObj->posCurr.x = 0;
	pObj->posCurr.y = AEGfxGetWinMaxY();
	// ����
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
		// �����ǻ����
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
	Launch_bl_e = clock();			//��ʱ����2000��з���һ�ε�
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
	// ֡ʱ�䣺Unity�е�dt
	// ======================
	double frameTime = AEFrameRateControllerGetFrameTime();

	// =========================
	// ��Ϸ�߼���Ӧ����
	// =========================

	// ״̬�л�
	if (AEInputCheckTriggered('R'))
	{
		Next = GS_Restart;
		return;
	}
	if (AEInputCheckTriggered(VK_ESCAPE))
	{
		if (MessageBox(NULL, "�Ƿ��˳�", "��ʾ", MB_OKCANCEL) == IDOK)
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

	// �ɴ��ƶ�
	if (AEInputCheckCurr(VK_UP) && spShip->posCurr.y<AEGfxGetWinMaxY())
	{
		spShip->dirCurr = PI / 2;
		AEVec2 added;
		AEVec2Set(&added, cosf(spShip->dirCurr), sinf(spShip->dirCurr));
		AEVec2Add(&spShip->posCurr, &spShip->posCurr, &added);
		spShip->velCurr.x = 5;
		spShip->velCurr.y = 5;

		// �������ٶȸ���λ��
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

		// λ�ø���
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
		// λ�ø���
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

		// λ�ø���
		spShip->posCurr.x += added.x * spShip->velCurr.x * 0.95f;
		//spShip->posCurr.y += added.y * spShip->velCurr.y * 0.95f;
	}

	// �ո�����(����һ���ӵ�����)
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

	// M�����䵼��
	if (AEInputCheckTriggered('D') && Skills>0)
	{
		Skills--;
		CreateSkill();
	}

	// ==================================================
	// ����������������player���ƣ������ģ�λ�õȣ�
	// ==================================================
	for (int i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj* pInst = sGameObjList + i;
		AEVec2 added;

		// �����ǻ���󣬲�����
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		// ���µ���λ��
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

		// �����ӵ�λ��
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

		// ���µ���λ��
		if (pInst->pObject->type == TYPE_SKill)
		{
			/*AEVec2Set(&added, 100.0f * (float)(frameTime) * cosf(pInst->dirCurr), 100.0f * (float)(frameTime) * sinf(pInst->dirCurr));
			AEVec2Add(&pInst->posCurr, &pInst->posCurr, &added);*/
			AEVec2Set(&added, pInst->speed * (float)(frameTime)* cosf(pInst->dirCurr), pInst->speed * (float)(frameTime)* sinf(pInst->dirCurr));
			AEVec2Add(&pInst->posCurr, &pInst->posCurr, &added);
		}
	}

	// ===================================
	// ��������
	// Example:
	//		-- ������Ļ�ڶ�����֣��ɴ� С���ǣ�
	//		-- ������Ļɾ�����ӵ���
	//		-- �����������·���
	// ===================================
	Check();
}


static void Check()
{

	unsigned long i;
	float round;

	// ==========================================================================================
	// ��ȡ���������ߵ����꣬�����ڷ����ƶ������ţ�����ֵ�ᷢ���仯
	// ==========================================================================================
	float winMaxX = AEGfxGetWinMaxX();
	float winMaxY = AEGfxGetWinMaxY();
	float winMinX = AEGfxGetWinMinX();
	float winMinY = AEGfxGetWinMinY();
	for (i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj* pInst = sGameObjList + i;

		// �����ǻ����
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		// �ɴ���Wrap
		if (pInst->pObject->type == TYPE_SHIP)
		{
			pInst->posCurr.x = AEWrap(pInst->posCurr.x, winMinX - SHIP_SIZE, winMaxX + SHIP_SIZE);
			pInst->posCurr.y = AEWrap(pInst->posCurr.y, winMinY - SHIP_SIZE, winMaxY + SHIP_SIZE);
			continue;
		}

		// �з�С����Wrap
		if (pInst->pObject->type == TYPE_ENEMY || pInst->pObject->type == TYPE_BOSS1)
		{
			pInst->posCurr.x = AEWrap(pInst->posCurr.x, winMinX - pInst->scale, winMaxX + pInst->scale);
			pInst->posCurr.y = AEWrap(pInst->posCurr.y, winMinY - pInst->scale, winMaxY + pInst->scale);
			continue;
		}

		// ɾ��������Ļ�߽���ӵ�
		if ((pInst->posCurr.x < winMinX) || (pInst->posCurr.x > winMaxX) || (pInst->posCurr.y < winMinY) || (pInst->posCurr.y > winMaxY))
			pInst->flag = 0;
	}

	// ====================
	// ��ײ���
	// ====================
	for (i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj* pInst = sGameObjList + i;

		// ������ǻ����
		if ((pInst->flag && FLAG_ACTIVE) == 0)
			continue;

		// ���� �� �ɴ� / �ӵ�/ ���� ����ײ���
		if (pInst->pObject->type == TYPE_ENEMY || pInst->pObject->type == TYPE_ENYME_BULLET || pInst->pObject->type == TYPE_BOSS1)
		{
			for (int j = 0; j < GAME_OBJ_NUM_MAX; j++)
			{
				GameObj * pInstOther = sGameObjList + j;

				// �����ǻ�����С��������
				if ((pInstOther->flag && FLAG_ACTIVE) == 0 || (pInstOther->pObject->type == TYPE_ENEMY))
					continue;

				// asteroid vs. ship
				if (pInstOther->pObject->type == TYPE_SHIP)
				{
					// ��ײ���
					if (AETestCircleToCircle(&(pInst->posCurr), pInst->scale, &(pInstOther->posCurr), pInstOther->scale))
					{
						spShip->live -= 25;
						if (spShip->live <= 0)
						{
							sShipLives -= 1;// �ɴ�����
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
							// ���¿�ʼ�ؿ�
							Next = GS_Over;
						}
						else
						{
							//PlaySound(TEXT("sound\\boom.wav"), NULL, SND_FILENAME | SND_ASYNC);
							// ����λ��
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
					// ������ײ�������߶�����AETestCircleToCircle(&(pInst->posCurr),pInst->scale,&(pInstOther->posCurr),pInstOther->scale)

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
	// �������ж����2D�任����
	// =====================================
	for (i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		AEMtx33		 trans, rot, scale;
		GameObj* pInst = sGameObjList + i;

		// ������ǻ����
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		// ���ž���
		AEMtx33Scale(&scale, pInst->scale, pInst->scale);
		// ��ת����
		AEMtx33Rot(&rot, pInst->dirCurr);
		// ƽ�ƾ���
		AEMtx33Trans(&trans, pInst->posCurr.x, pInst->posCurr.y);
		// ����ȷ��˳����������3�������γ�2ά�任����
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

	// �������ڵڶ���������
	if ((newv.x < 0 && newv.y < 0) || (newv.x < 0 && newv.y > 0))
		angle += PI;
	// �������ڵ�������
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

	// ������ƶ����б��е����ж���
	for (i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj* pInst = sGameObjList + i;

		// �����ǻ����
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
		// ���ö����2D�任����ʹ�ú�����AEGfxSetTransform
		AEGfxSetTransform(pInst->transform.m);
		// ���Ƶ�ǰ����ʹ�ú�����AEGfxMeshDraw
		AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
	}
}

void Free2(void)
{
	int i = 0;

	// ʹ�ú���gameObjDestroyɾ���б��еĶ���
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
	// ж�ض�����״������Դ��ʹ�ú�����AEGfxMeshFree
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

	// ����Ϸ�����б���û�ù���λ��
	for (int i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj* pInst = sGameObjList + i;

		// �ҷǻ�����λ��
		if (pInst->flag == 0)
		{
			// �ҵ���
			pInst->pObject = sGameObjBaseList + type;
			pInst->flag = FLAG_ACTIVE;
			pInst->scale = scale;
			pInst->posCurr = pPos ? *pPos : zero;
			pInst->velCurr = pVel ? *pVel : zero;
			pInst->dirCurr = dir;
			pInst->live = 100;
			pInst->speed = 100.0f;
			// �����´����Ķ���ʵ��
			return pInst;
		}
	}

	// λ������
	return NULL;
}

// ---------------------------------------------------------------------------

void gameObjDestroy(GameObj* pInst)
{
	// ���Ѿ�����
	if (pInst->flag == 0)
		return;

	// ����
	pInst->flag = 0;
}