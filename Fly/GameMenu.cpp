#include "GameMenu.h"
#include "System.h"

static AEGfxVertexList*	BgMesh, *Mesh_about;
static AEGfxTexture *pTexBg, *pTex_about;
static int IshowAbout;
void GameMenu::Load()
{
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
	IsNull(BgMesh);

	AEGfxMeshStart();
	AEGfxTriAdd(
		-300.0f, -180.0f, 0, 0, 1.0f,
		300.0f, -180.0f, 0, 1.0f, 1.0f,
		-300.0f, 180.0f, 0, 0, 0);
	AEGfxTriAdd(
		300.0f, -180.0f, 0, 1.0f, 1.0f,
		300.0f, 180.0f, 0, 1.0f, 0,
		-300.0f, 180.0f, 0, 0, 0);

	Mesh_about = AEGfxMeshEnd();
	IsNull(Mesh_about);

	pTexBg = AEGfxTextureLoad("res\\menu.jpg");
	pTex_about= AEGfxTextureLoad("res\\remote.jpg");
}
void GameMenu::Init()
{
	IshowAbout = 0;
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
}
void GameMenu::Updata()
{
	if (AEInputCheckTriggered('1'))
	{
		MenuManage->Next = GS_RETURN;
		return;
	}
	if (AEInputCheckTriggered(VK_ESCAPE))
	{
		if (MessageBox(NULL, "是否退出", "提示", MB_OKCANCEL) == IDOK)
		{
			exit(0);
		}
	}
	if (AEInputCheckTriggered('2'))
	{
		IshowAbout = 1;
		return;
	}
	if (AEInputCheckTriggered('3'))
	{
		manage->IsPlaying = !manage->IsPlaying;
		manage->OpenOrCloseSound(manage->IsPlaying);
		return;
	}
	if (AEInputCheckTriggered(VK_RETURN))
	{
		IshowAbout = 0;
		return;
	}
}
void GameMenu::Draw()
{
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	//AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(0, 0);
	AEGfxTextureSet(pTexBg, 0, 0.0f);
	AEGfxSetTransparency(1);
	//AEGfxSetBlendColor(0.0f, 0.0f, 0.0f,0.0f);
	//// 绘制当前对象，使用函数：AEGfxMeshDraw
	AEGfxMeshDraw(BgMesh, AE_GFX_MDM_TRIANGLES);
	if (IshowAbout==1)
	{
		AEGfxTextureSet(pTex_about, 0, 0.0f);
		AEGfxMeshDraw(Mesh_about, AE_GFX_MDM_TRIANGLES);
	}
}
void GameMenu::Free()
{

}
void GameMenu::UnLoad()
{
	AEGfxMeshFree(BgMesh);
	AEGfxTextureUnload(pTexBg);
	AEGfxMeshFree(Mesh_about);
	AEGfxTextureUnload(pTex_about);
}