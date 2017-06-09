/* �ؿ�1  */
#include "System.h"
#include "Level1.h"
#define GAME_OBJ_BASE_NUM_MAX	32			// �������ͣ�������ࣩ��Ŀ����
#define GAME_OBJ_NUM_MAX		512			// ������Ŀ����
#define SHIP_INITIAL_NUM		3			// �ɴ���lives��Ŀ
#define Num_Enemy				3			// ������Ŀ/��λʱ��
#define SHIP_SIZE				40.0f		// �ɴ��ߴ�
#define FLAG_ACTIVE				1			// ������־

static int FlyMode = 0;
static GameObjBase		sGameObjBaseList[GAME_OBJ_BASE_NUM_MAX];	// �������е�Ԫ������Ϸ��������ʵ������״������
static unsigned long	sGameObjBaseNum;							// �Ѷ������Ϸ�������
// ��Ϸ�����б�
static GameObj			sGameObjList[GAME_OBJ_NUM_MAX];				// �������е�Ԫ������Ϸ�����ʵ��
static unsigned long	sGameObjNum;								// ��Ϸ����ĸ���
static GameObj* spShip;
static Timer timer_eshoot;
static Timer timer_ecreate;
static Timer timer_drawtag;
static Timer aShoot;

static AEGfxVertexList*	BgMesh,*mesh_lev1,*mesh_progress,*mesh_live;
static AEGfxTexture *pTexSp, *pTexBl, *pTexEnemy, *pTexBoss, *pTexBg1, *pTexSkill1, *pTexEbl, *pTexLev, *pTexprogess;
static AEGfxTexture *live0, *live1, *live2, *live3, *live4, *live5;		// ����2������
static AEGfxTexture *q0, *q1, *q2, *q3, *q4, *q5;						// ����2������
static AEGfxTexture *prop0, *prop1, *prop2, *prop3, *prop4, *prop5;
static int			WhenBoss;
static Matrix mpro_boss,mpro_sp,mlive_matrix,mq_matrix;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
// ����/ɾ����Ϸ����

static GameObj*	gameObjCreate (unsigned long type, float scale, Vec2* pPos, Vec2* pVel, float dir);
static void		gameObjDestroy(GameObj* pInst);
static void		CreateEneMy(int count, int quadrant);
static void		CreateBoss(int type);
static void		LaunchBullte(int, float);
static void		CreateSkill();
static float	getDirCur(GameObj *pTarget, GameObj *pInst);
static void		BossSkill(GameObj* &pInst);
static void		CreatProp(GameObj* & pInst);
static void		getProp(int tag);
static void		initSpShip();
void Level1::Load()
{
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
	IsNull(pObjBase->pMesh);
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
	IsNull(pObjBase->pMesh);

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
	IsNull(pObjBase->pMesh);


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
	IsNull(pObjBase->pMesh);

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
	IsNull(pObjBase->pMesh);

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
	IsNull(pObjBase->pMesh);


	// =========================
	// prop
	// =========================
	pObjBase = sGameObjBaseList + sGameObjBaseNum++;
	pObjBase->type = TYPE_PROP;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-70.0f, -69.0f, 0, 0, 1.0f,
		70.0f, -69.0f, 0, 1.0f, 1.0f,
		-70.0f, 69.0f, 0, 0, 0);
	AEGfxTriAdd(
		70.0f, -69.0f, 0, 1.0f, 1.0f,
		70.0f, 69.0f, 0, 1.0f, 0,
		-70.0f, 69.0f, 0, 0, 0);
	pObjBase->pMesh = AEGfxMeshEnd();
	IsNull(pObjBase->pMesh);


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
	IsNull(BgMesh);


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
	IsNull(mesh_lev1);

	AEGfxMeshStart();
	AEGfxTriAdd(
		-300.0f, -15.0f, 0, 0, 1.0f,
		300.0f, -15.0f, 0, 1.0f, 1.0f,
		-300.0f, 15.0f, 0, 0, 0);
	AEGfxTriAdd(
		300.0f, -15.0f, 0, 1.0f, 1.0f,
		300.0f, 15.0f, 0, 1.0f, 0,
		-300.0f, 15.0f, 0, 0, 0);

	mesh_progress = AEGfxMeshEnd();
	IsNull(mesh_progress);

	AEGfxMeshStart();
	AEGfxTriAdd(
		-72.0f, -62.0f, 0, 0, 1.0f,
		72.0f, -62.0f, 0, 1.0f, 1.0f,
		-72.0f, 62.0f, 0, 0, 0);
	AEGfxTriAdd(
		72.0f, -62.0f, 0, 1.0f, 1.0f,
		72.0f, 62.0f, 0, 1.0f, 0,
		-72.0f, 62.0f, 0, 0, 0);

	mesh_live = AEGfxMeshEnd();
	IsNull(mesh_live);

	pTexSp = AEGfxTextureLoad("res\\player1.png");
	pTexBl = AEGfxTextureLoad("res\\bullet1.png");
	pTexEnemy = AEGfxTextureLoad("res\\enemy1.png");
	pTexBoss = AEGfxTextureLoad("res\\boss.png");
	pTexBg1 = AEGfxTextureLoad("res\\bg5.jpg");
	pTexSkill1 = AEGfxTextureLoad("res\\skill1.png");
	pTexEbl = AEGfxTextureLoad("res\\enemybl1.png");
	pTexLev = AEGfxTextureLoad("res\\lev1.png");
	pTexprogess= AEGfxTextureLoad("res\\progress.png");
	live0= AEGfxTextureLoad("res\\live0.png");
	live1 = AEGfxTextureLoad("res\\live1.png");
	live2 = AEGfxTextureLoad("res\\live2.png");
	live3 = AEGfxTextureLoad("res\\live3.png");
	live4 = AEGfxTextureLoad("res\\live4.png");
	live5 = AEGfxTextureLoad("res\\live5.png");
	q0 = AEGfxTextureLoad("res\\scount0.png");
	q1 = AEGfxTextureLoad("res\\scount1.png");
	q2 = AEGfxTextureLoad("res\\scount2.png");
	q3 = AEGfxTextureLoad("res\\scount3.png");
	q4 = AEGfxTextureLoad("res\\scount4.png");
	q5 = AEGfxTextureLoad("res\\scount5.png");
	prop0= AEGfxTextureLoad("res\\blood.png");
	prop1 = AEGfxTextureLoad("res\\blue.png");
	prop2 = AEGfxTextureLoad("res\\propb.png");
	prop3 = AEGfxTextureLoad("res\\props.png");
	prop4 = AEGfxTextureLoad("res\\propa.png");
}

void Level1::Init()
{
	aShoot.Start();
	WhenBoss = 0;
	timer_eshoot.Start();
	timer_ecreate.Start();
	timer_drawtag.Start();
	
	srand(time(NULL));
	// Ϊ��ʼ��������׼��
	//AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	// ����ʵ��������Ϸ��ʼֻ�зɴ���С������Ҫʵ����
	// �ɴ�����ʵ����
	spShip = gameObjCreate(TYPE_SHIP, SHIP_SIZE, 0, 0, 0.0f);
	AE_ASSERT(spShip);
	initSpShip();
	// �������ɴ���Ŀ��ʼ��
	sShipLives = SHIP_INITIAL_NUM;
	Skills = 3;
	CreateEneMy(Num_Enemy, 1);
	CreateEneMy(Num_Enemy, -1);

	Matrix trans, rot, scale;
	MatrixScale(scale, 0.5f, 0.5f);
	MatrixRot(rot, 0);
	MatrixTranslate(trans, AEGfxGetWinMinX()+40, AEGfxGetWinMaxY() - 40);
	// ����ȷ��˳����������3�������γ�2ά�任����
	MatrixConcat(mlive_matrix, trans, rot);
	MatrixConcat(mlive_matrix, mlive_matrix, scale);

	MatrixScale(scale, 0.5f, 0.5f);
	MatrixRot(rot, 0);
	MatrixTranslate(trans, AEGfxGetWinMinX() + 120, AEGfxGetWinMaxY() - 40);
	// ����ȷ��˳����������3�������γ�2ά�任����
	MatrixConcat(mq_matrix, trans, rot);
	MatrixConcat(mq_matrix, mq_matrix, scale);
}

void Level1::Updata()
{
	
	if (autoShoot&&aShoot.getLength()>350)
	{
		aShoot.Reset();
		for (int i = -spShipBullet; i <= spShipBullet; i++)
		{
			GameObj * pBullet = gameObjCreate(TYPE_BULLET, 10.0f, 0, 0, 0.0f);
			if (pBullet != NULL)
			{
				pBullet->posCurr.x = spShip->posCurr.x + i * 20;
				pBullet->posCurr.y = spShip->posCurr.y;
				pBullet->dirCurr = spShip->dirCurr;
			}
		}
	}		
	
	if (timer_eshoot.getLength()>3000)//��ʱ����2000��з���һ�ε�
	{
		timer_eshoot.Reset();
		LaunchBullte(TYPE_ENYME_BULLET, 6.0f);
	}


	if (timer_ecreate.getLength()>10000)
	{
		timer_ecreate.Reset();
		CreateEneMy(Num_Enemy, 1);
		CreateEneMy(Num_Enemy, -1);
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
	if (GetKeyState(VK_TAB) & 0x8000)
	{
		MenuManage->setLevel(GS_MENU);
		MenuManage->Run();
	}
	if (AEInputCheckTriggered('R'))
	{
		manage->Next = GS_Restart;
		return;
	}
	if (AEInputCheckTriggered('2'))
	{
		manage->Next = GS_L2;
		return;
	}
	if (AEInputCheckTriggered(VK_ESCAPE))
	{
		if (MessageBox(NULL, "�Ƿ��˳�", "��ʾ", MB_OKCANCEL) == IDOK)
		{
			manage->Next = GS_Quit;
			return;
		}

	}
	// �ɴ��ƶ�
	if (AEInputCheckCurr(VK_UP) && spShip->posCurr.y<AEGfxGetWinMaxY())
	{
		spShip->dirCurr = PI / 2;
		spShip->posCurr.y += spShip->speed * 0.95f;
	}

	if (AEInputCheckCurr(VK_DOWN) && spShip->posCurr.y>AEGfxGetWinMinY())
	{
		spShip->dirCurr = PI / 2;
		spShip->posCurr.y -= spShip->speed * 0.95f;
	}

	if (AEInputCheckCurr(VK_LEFT) && spShip->posCurr.x>AEGfxGetWinMinX())
	{
		spShip->dirCurr = PI / 2;
		spShip->posCurr.x -= spShip->speed * 0.95f;
	}

	if (AEInputCheckCurr(VK_RIGHT) && spShip->posCurr.x<AEGfxGetWinMaxX())
	{
		spShip->dirCurr = PI / 2;
		spShip->posCurr.x += spShip->speed * 0.95f;
		//spShip->posCurr.y += added.y * spShip->velCurr.y * 0.95f;
	}

	// �ո�����(����һ���ӵ�����)
	if (AEInputCheckTriggered(VK_SPACE))
	{
//		FILE *stream;
//		AllocConsole();
//		freopen_s(&stream, "CONOUT$", "w", stdout);
//		printf("frame:%lf",frameTime);
		manage->PlayShoot();
		for (int i=-spShipBullet;i<= spShipBullet;i++)
		{
			GameObj * pBullet = gameObjCreate(TYPE_BULLET, 10.0f, 0, 0, 0.0f);
			if (pBullet != NULL)
			{
				pBullet->posCurr.x = spShip->posCurr.x+i*20;
				pBullet->posCurr.y = spShip->posCurr.y;
				pBullet->dirCurr = spShip->dirCurr;
			}
		}
	}

	// D����
	if (AEInputCheckTriggered('D')&& Skills>0)
	{
		Skills--;
		CreateSkill();
	}

	// ==================================================
	// ����������������player���ƣ������ģ�λ�õȣ�
	// ==================================================
	Vec2 added;
	for (int i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj* pInst = sGameObjList + i;

		// �����ǻ���󣬲�����
		if ((pInst->flag & FLAG_ACTIVE) == 0||pInst->pObject->type==TYPE_SHIP)
			continue;

		if (pInst->pObject->type == TYPE_BOSS1)
		{
			if (FlyMode == 0)
			{
				Vec2Set(added, 2 * cosf(-PI / 2 * (rand() % 10 / 10)), sinf(-PI / 2));
				if (pInst->posCurr.y<0)
				{
					FlyMode = 1;
				}
			}
			else if (FlyMode == 1)
			{
				Vec2Set(added, 2 * cosf(-PI / 2 * (rand() % 10 / 10)), sinf(PI / 2));
				if (pInst->posCurr.y>AEGfxGetWinMaxY() - 30)
				{
					FlyMode = 0;
				}
			}
			Vec2Add(pInst->posCurr, pInst->posCurr, added);
			continue;
		}

		// ���µ���λ��
		if (pInst->pObject->type == TYPE_ENEMY)
		{
			Vec2Set(added, 0.5* cosf(-PI / 2 * (rand() % 10 / 10)), sinf(-PI / 2));
			Vec2Add(pInst->posCurr, pInst->posCurr, added);
		}

		// �����ӵ�λ��
		else
		{
			Vec2Set(added, pInst->speed * (float)(frameTime)* cosf(pInst->dirCurr), pInst->speed * (float)(frameTime)* sinf(pInst->dirCurr));
			Vec2Add(pInst->posCurr, pInst->posCurr, added);
		}
	}

	// ===================================
	//		-- ������Ļɾ�����ӵ���
	//		-- �����������·���
	// ===================================
	Check();
}

void Level1::Draw()
{
	unsigned long i;
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	//AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(0, 0);

	AEGfxTextureSet(pTexBg1, 0, 0.0f);
	//// ���Ƶ�ǰ����ʹ�ú�����AEGfxMeshDraw
	AEGfxMeshDraw(BgMesh, AE_GFX_MDM_TRIANGLES);

	if (timer_drawtag.getLength()<1500)
	{
//		timer_drawtag.End();
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
			AEGfxTextureSet(pTexSp, 0, 0.0f);
		}
		else if (pInst->pObject->type == TYPE_BULLET)
		{
			AEGfxTextureSet(pTexBl, 0, 0.0f);
		}
		else if (pInst->pObject->type == TYPE_ENEMY)
		{
			AEGfxTextureSet(pTexEnemy, 0, 0.0f);
		}
		else if (pInst->pObject->type == TYPE_BOSS1)
		{
			AEGfxTextureSet(pTexBoss, 0, 0.0f);
		}
		else if (pInst->pObject->type == TYPE_SKill)
		{
			//AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxTextureSet(pTexSkill1, 0, 0.0f);
		}
		else if (pInst->pObject->type == TYPE_PROP)
		{
			switch (pInst->tag)
			{
			case 0:
				AEGfxTextureSet(prop0, 0, 0.0f);
				break;
			case 1:
				AEGfxTextureSet(prop1, 0, 0.0f);
				break;
			case 2:
				AEGfxTextureSet(prop2, 0, 0.0f);
				break;
			case 3:
				AEGfxTextureSet(prop3, 0, 0.0f);
				break;
			case 4:
				AEGfxTextureSet(prop4, 0, 0.0f);
				break;
			}
		}
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
	AEGfxTextureSet(pTexprogess, 0, 0);
	AEGfxSetTransform(mpro_boss.m);
	AEGfxMeshDraw(mesh_progress, AE_GFX_MDM_TRIANGLES);

	switch (sShipLives)
	{
	case 0:
		AEGfxTextureSet(live0, 0, 0.0f);
		break;
	case 1:
		AEGfxTextureSet(live1, 0, 0.0f);
		break;
	case 2:
		AEGfxTextureSet(live2, 0, 0.0f);
		break;
	case 3:
		AEGfxTextureSet(live3, 0, 0.0f);
		break;
	case 4:
		AEGfxTextureSet(live4, 0, 0.0f);
		break;
	case 5:
		AEGfxTextureSet(live5, 0, 0.0f);
		break;
	default:
		AEGfxTextureSet(live3, 0, 0.0f);
	}
	AEGfxSetTransform(mlive_matrix.m);
	AEGfxMeshDraw(mesh_live, AE_GFX_MDM_TRIANGLES);

	switch (Skills)
	{
	case 0:
		AEGfxTextureSet(q0, 0, 0.0f);
		break;
	case 1:
		AEGfxTextureSet(q1, 0, 0.0f);
		break;
	case 2:
		AEGfxTextureSet(q2, 0, 0.0f);
		break;
	case 3:
		AEGfxTextureSet(q3, 0, 0.0f);
		break;
	case 4:
		AEGfxTextureSet(q4, 0, 0.0f);
		break;
	case 5:
		AEGfxTextureSet(q5, 0, 0.0f);
		break;
	default:
		AEGfxTextureSet(q3, 0, 0.0f);
	}
	AEGfxSetTransform(mq_matrix.m);
	AEGfxMeshDraw(mesh_live, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(pTexprogess, 0, 0);
	AEGfxSetTransform(mpro_sp.m);
	AEGfxMeshDraw(mesh_progress, AE_GFX_MDM_TRIANGLES);
}

void Level1::Free()
{
	// ʹ�ú���gameObjDestroyɾ���б��еĶ���
	for (int i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj* pInst = sGameObjList + i;
		gameObjDestroy(pInst);
	}
}

void Level1::UnLoad()
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
	AEGfxMeshFree(mesh_progress);
	AEGfxTextureUnload(pTexprogess);
	AEGfxMeshFree(mesh_live);
	AEGfxTextureUnload(live1);
	AEGfxTextureUnload(live2);
	AEGfxTextureUnload(live3);
	AEGfxTextureUnload(prop0);
	AEGfxTextureUnload(prop1);
	AEGfxTextureUnload(prop2);
	AEGfxTextureUnload(prop3);
	AEGfxTextureUnload(prop4);
	// ж�ض�����״������Դ��ʹ�ú�����AEGfxMeshFree
	for (int i = 0; i < GAME_OBJ_BASE_NUM_MAX; i++)
	{
		GameObjBase* pGamObjBase = sGameObjBaseList + i;
		AEGfxMeshFree(pGamObjBase->pMesh);
	}
}



void CreateEneMy(int count,int quadrant)
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
		if (pObj==NULL)
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
	for (int i=0;i<30;i++)
	{
		pBullet = gameObjCreate(TYPE_SKill, 15.0f, 0, 0, 0.0f);
		if (pBullet == NULL)
		{
			return;
		}
		pBullet->speed = 200.0f;
		pBullet->posCurr.x = spShip->posCurr.x;
		pBullet->posCurr.y = spShip->posCurr.y;
		pBullet->dirCurr = i*PI/15;
	}

}
void CreateBoss(int type)
{
	GameObj* pObj;
	WhenBoss++;
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

void LaunchBullte(int type,float scale)
{
	for (int i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj* pInst = sGameObjList + i;
		// �����ǻ����
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;
		if (pInst->flag != 0 )
		{
			if (pInst->pObject->type == TYPE_ENEMY)
			{
				GameObj * pBullet = gameObjCreate(TYPE_ENYME_BULLET,12.0f, 0, 0, 0.0f);
				if (pBullet == NULL)
				{
					break;
				}
				pBullet->posCurr = pInst->posCurr;
				pBullet->dirCurr = getDirCur(spShip, pInst);
			}
			else if ((pInst->pObject->type == TYPE_BOSS1))
			{
				BossSkill(pInst);
			}
		}
	}
}


static void Check()
{

	unsigned long i;

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
		
		// �ɴ�
		if (pInst->pObject->type == TYPE_SHIP)
		{
			pInst->posCurr.x = AEWrap(pInst->posCurr.x, winMinX - SHIP_SIZE, winMaxX + SHIP_SIZE);
			pInst->posCurr.y = AEWrap(pInst->posCurr.y, winMinY - SHIP_SIZE, winMaxY + SHIP_SIZE);
			continue;
		}

		// �з�С����Wrap
		if (pInst->pObject->type == TYPE_ENEMY|| pInst->pObject->type==TYPE_BOSS1)
		{
			pInst->posCurr.x = AEWrap(pInst->posCurr.x, winMinX - pInst->scale, winMaxX + pInst->scale);
			pInst->posCurr.y = AEWrap(pInst->posCurr.y, winMinY - pInst->scale, winMaxY + pInst->scale);
			continue;
		}

		// ɾ��������Ļ�߽���ӵ�
		if ( (pInst->posCurr.x < winMinX) || (pInst->posCurr.x > winMaxX) || (pInst->posCurr.y < winMinY) || (pInst->posCurr.y > winMaxY) )
				pInst->flag = 0;
	}

	// ====================
	// ��ײ���
	// ====================
	for (i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj* pInst = sGameObjList + i;

		// ������ǻ����
		if ((pInst->flag & FLAG_ACTIVE) == 0|| pInst->pObject->type==TYPE_SHIP|| pInst->pObject->type == TYPE_SKill||pInst->pObject->type == TYPE_BULLET)
			continue;
		
		// ���� �� �ɴ� / �ӵ�/ ���� ����ײ���
			for ( int j = 0; j < GAME_OBJ_NUM_MAX; j++)
			{
				GameObj * pInstOther = sGameObjList + j;

				// �����ǻ�����С��������
				if ( (pInstOther->flag & FLAG_ACTIVE) == 0 || (pInstOther->pObject->type == TYPE_ENEMY) )
					continue;

				// asteroid vs. ship
				if ( pInstOther->pObject->type == TYPE_SHIP)
				{
					// ��ײ���
					if (IsCrash(pInst->posCurr, pInstOther->posCurr, pInst->scale, pInstOther->scale))
					{
						spShip->live -= 25;
						if (spShip->live<=0)
						{
							sShipLives -= 1;// �ɴ�����
						}
						else if (pInst->pObject->type == TYPE_PROP)
						{
							spShip->live += 25;
							getProp(pInst->tag);
							pInst->flag = 0;
							continue;
						} 
						else if (pInst->pObject->type!=TYPE_BOSS1)
						{
							pInst->flag = 0;
							continue;
						}
						else
							continue;
						if ( sShipLives < 0 )
							manage->Next = GS_Over;
						else
						{	
							// ����λ��
							manage->PlayBoom();
							initSpShip();
							CreateSkill();
						}
					}
				}
				if (pInst->pObject->type==TYPE_PROP)
					continue;
				// asteroid vs. bullet
				if ( pInstOther->pObject->type == TYPE_BULLET )
				{				
					if (IsCrash(pInst->posCurr,pInstOther->posCurr,pInst->scale,pInstOther->scale))
					{
						pInst->live -= 2;
						if (pInst->pObject->type==TYPE_BOSS1&&pInst->live<=0)
						{
							pInst->flag = 0;
							manage->Next= GS_L2;
						}
						else if (pInst->pObject->type != TYPE_BOSS1)
						{
							pInst->flag = 0;
							if (pInst->pObject->type ==TYPE_ENEMY )
							{
								CreatProp(pInst);
							}	
						}
						pInstOther->flag = 0;
					}
				}		

				// asteroid vs. missile
				if ( pInstOther->pObject->type == TYPE_SKill )
				{
					if (IsCrash(pInst->posCurr, pInstOther->posCurr, pInst->scale, pInstOther->scale))
					{
						pInst->live -= 10;
						if (pInst->pObject->type == TYPE_BOSS1&&pInst->live <= 0)
						{
							pInst->flag = 0;
							manage->Next = GS_L2;
						}
						else if (pInst->pObject->type == TYPE_BOSS1)
						{
							pInstOther->flag = 0;
						}
						else if (pInst->pObject->type != TYPE_BOSS1)
						{
							if (pInst->pObject->type == TYPE_ENEMY)
							{
								CreatProp(pInst);
							}
							pInst->flag = 0;
						}
					}
				}
		}

	}

	// =====================================
	// �������ж����2D�任����
	// =====================================
	Matrix		 trans, rot, scale;
	MatrixScale(scale, spShip->live/100.0f, 1);
	MatrixRot(rot, 0);
	MatrixTranslate(trans, 0, AEGfxGetWinMaxY() - 30);
	// ����ȷ��˳����������3�������γ�2ά�任����
	MatrixConcat(mpro_sp, trans, rot);
	MatrixConcat(mpro_sp, mpro_sp, scale);
	for (i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		
		GameObj* pInst = sGameObjList + i;
		
		// ������ǻ����
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		// ���ž���
		MatrixScale(scale, pInst->scale, pInst->scale);
		// ��ת����
		MatrixRot(rot, pInst->dirCurr);
		// ƽ�ƾ���
		MatrixTranslate(trans, pInst->posCurr.x, pInst->posCurr.y);
		// ����ȷ��˳����������3�������γ�2ά�任����
		MatrixConcat(pInst->transform, trans, rot);
		MatrixConcat(pInst->transform, pInst->transform, scale);
		if (pInst->pObject->type==TYPE_BOSS1)
		{
			MatrixScale(scale, pInst->live / 500.0f, 0.3f);
			MatrixRot(rot, 0);
			MatrixTranslate(trans, pInst->posCurr.x, pInst->posCurr.y+ pInst->scale+10.0f);
			// ����ȷ��˳����������3�������γ�2ά�任����
			MatrixConcat(mpro_boss, trans, rot);
			MatrixConcat(mpro_boss, mpro_boss, scale);
		}
	}
}

float getDirCur(GameObj *pTarget,GameObj *pInst)
{
	Vec2 newv;
	float angle;
	Vec2Sub(newv, (pTarget->posCurr), (pInst->posCurr));
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

void BossSkill(GameObj* &pInst)
{
	for (int j = 0; j<75; j++)
	{
		GameObj * pBullet = gameObjCreate(TYPE_ENYME_BULLET, 12.0f, 0, 0, 0.0f);
		if (pBullet == NULL)
		{
			return;
		}
		pBullet->posCurr.x = pInst->posCurr.x;
		pBullet->posCurr.y = pInst->posCurr.y;
		pBullet->dirCurr = PI / 15 * j;
		pBullet->speed = j * 3 + 100.0f;
	}
}

void CreatProp(GameObj* & pInst)
{
	srand(time(NULL));
	int prob = rand() % 300;
	if (prob<210)
		return;
	GameObj * prop = gameObjCreate(TYPE_PROP, 0.4f, 0, 0, 0.0f);
	prop->dirCurr = -PI / 2;
	prop->posCurr = pInst->posCurr;
	if (prop == NULL)
		return;
	if (prob > 295)
		prop->tag = 0;
	else if (prob>281)
		prop->tag = 1;
	else if (prob>245)
		prop->tag = 2;
	else if (prob>215)
		prop->tag = 3;
	else	
		prop->tag = 4;
}

void getProp(int tag)
{
	switch (tag)
	{
		case 0:
			sShipLives = ++sShipLives > 5 ? 5 : sShipLives;
			break;
		case 1:
			Skills = ++Skills > 5 ? 5 : Skills;
			break; 
		case 2:
			spShipBullet = ++spShipBullet > 2 ? 2 : spShipBullet;
			break;
		case 3:
			spShip->speed=++spShip->speed>7?7: spShip->speed;
			break;
		case 4:
			autoShoot = 1;
			break;
	}
}

void initSpShip()
{
	spShip->posCurr.x = 0;
	spShip->posCurr.y = AEGfxGetWinMinY() + 30;
	spShip->speed = 3.0f;
	spShip->dirCurr = PI / 2;
	spShipBullet = 0;
	spShip->live = 100;
	autoShoot = 0;
}


//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
GameObj* gameObjCreate(unsigned long type, float scale, Vec2* pPos, Vec2* pVel, float dir)
{
	Vec2 zero = { 0.0f, 0.0f };

	//AE_ASSERT_PARM(type < sGameObjBaseNum);
	
	// ����Ϸ�����б���û�ù���λ��
	for (int i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj* pInst = sGameObjList + i;

		// �ҷǻ�����λ��
		if (pInst->flag == 0)
		{
			// �ҵ���
			pInst->pObject	= sGameObjBaseList + type;
			pInst->flag		= FLAG_ACTIVE;
			pInst->scale	= scale;
			pInst->posCurr	= pPos ? *pPos : zero;
			pInst->velCurr	= pVel ? *pVel : zero;
			pInst->dirCurr	= dir;
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