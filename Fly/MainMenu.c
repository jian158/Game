#include "MainMenu.h"
#include "AEEngine.h"
#include "GameStateList.h"
#include <winuser.h>
static AEGfxVertexList*	BgMesh;
static AEGfxTexture *pTexBg;
void LoadMain(void)
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
	AE_ASSERT_MESG(BgMesh, "Failed to create Asteroid object!!");
	pTexBg = AEGfxTextureLoad("res\\BgMain.jpg");
}
void InitMenu(void)
{
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
}
LPMSG Msg;
void UpdateMenu(void)
{
	if (AEInputCheckTriggered('1'))
	{
			Next = GS_L1;
			return;
	}
	else if (AEInputCheckTriggered(VK_F1))
	{
		MessageBox(NULL, "make for 用心创造快乐", "关于", MB_OK);
	}
	else if (AEInputCheckTriggered(VK_ESCAPE))
	{
		if (MessageBox(NULL, "是否退出", "提示", MB_OKCANCEL) == IDOK)
		{
			Next = GS_Quit;
			return;
		}
		
	}
}
void DrawMenu(void)
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
}
void FreeMenu(void)
{
	
}
void UnloadMenu(void)
{
	AEGfxMeshFree(BgMesh);
	AEGfxTextureUnload(pTexBg);
}