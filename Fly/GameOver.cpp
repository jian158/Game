
#include "System.h"
#include "GameStateList.h"
#include "GameOver.h"
static AEGfxVertexList*	BgMesh;
static AEGfxTexture *pTexBg;
//extern int Next;
void LoadOver(void)
{
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
	pTexBg = AEGfxTextureLoad("res\\bgover.jpg");
}
void InitOver(void)
{
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
}
void UpdateOver(void)
{
	if (AEInputCheckTriggered(VK_RETURN))
	{
		Next = GS_MAIN;
		return;
	}
}
void DrawOver(void)
{
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	//AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(0, 0);
	AEGfxTextureSet(pTexBg, 0, 0.0f);
	AEGfxSetTransparency(1);
	//AEGfxSetBlendColor(0.0f, 0.0f, 0.0f,0.0f);
	//// ���Ƶ�ǰ����ʹ�ú�����AEGfxMeshDraw
	AEGfxMeshDraw(BgMesh, AE_GFX_MDM_TRIANGLES);
}
void FreeOver(void)
{

}
void UnloadOver(void)
{
	AEGfxMeshFree(BgMesh);
	AEGfxTextureUnload(pTexBg);
}