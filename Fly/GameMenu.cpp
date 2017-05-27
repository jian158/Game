#include "GameMenu.h"
#include "System.h"

static AEGfxVertexList*	BgMesh;
static AEGfxTexture *pTexBg;
void GameMenu::Load()
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
	pTexBg = AEGfxTextureLoad("res\\menu.jpg");
}
void GameMenu::Init()
{
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
		if (MessageBox(NULL, "�Ƿ��˳�", "��ʾ", MB_OKCANCEL) == IDOK)
		{
			exit(0);
		}
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
	//// ���Ƶ�ǰ����ʹ�ú�����AEGfxMeshDraw
	AEGfxMeshDraw(BgMesh, AE_GFX_MDM_TRIANGLES);
}
void GameMenu::Free()
{

}
void GameMenu::UnLoad()
{
	AEGfxMeshFree(BgMesh);
	AEGfxTextureUnload(pTexBg);
}