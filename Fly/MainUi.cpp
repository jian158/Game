#include "MainUi.h"
#include "System.h"
static AEGfxVertexList *Mesh_about;
static AEGfxTexture *pTex_about,*pTex_remote;
static int IshowAbout,IshowSeson;
void MainUi::Load()
{
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
		-350.0f, -200.0f, 0, 0, 1.0f,
		350.0f, -200.0f, 0, 1.0f, 1.0f,
		-350.0f, 200.0f, 0, 0, 0);
	AEGfxTriAdd(
		350.0f, -200.0f, 0, 1.0f, 1.0f,
		350.0f, 200.0f, 0, 1.0f, 0,
		-350.0f, 200.0f, 0, 0, 0);

	Mesh_about = AEGfxMeshEnd();
	IsNull(Mesh_about);

	pTexBg = AEGfxTextureLoad("res\\BgMain.jpg");
	pTex_about = AEGfxTextureLoad("res\\about.jpg");
	pTex_remote = AEGfxTextureLoad("res\\remote.jpg");
}

void MainUi::Init()
{
	IshowAbout = 0;
	IshowSeson = 0;
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
}

void MainUi::Updata()
{
	if (AEInputCheckTriggered('1'))
	{
		manage->Next = GS_L1;
		return;
	}
	if (AEInputCheckTriggered('2'))
	{
		IshowSeson = 1;
		return;
	}
	else if (AEInputCheckTriggered('3'))
	{
		IshowAbout = 1;
		return;
	}
	else if (AEInputCheckTriggered('4'))
	{
		manage->IsPlaying = !manage->IsPlaying;
		manage->OpenOrCloseSound(manage->IsPlaying);
		return;
	}
	else if (AEInputCheckTriggered('5'))
	{
		if (MessageBox(NULL, "是否退出", "提示", MB_OKCANCEL) == IDOK)
		{
			manage->Next = GS_Quit;
			return;
		}

	}
	else if (AEInputCheckTriggered(VK_RETURN))
	{
		IshowAbout = 0;
		IshowSeson = 0;
	}
}

void MainUi::Draw()
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
	if (IshowAbout == 1)
	{
		AEGfxTextureSet(pTex_about, 0, 0.0f);
		AEGfxMeshDraw(Mesh_about, AE_GFX_MDM_TRIANGLES);
	}
	if (IshowSeson == 1)
	{
		AEGfxTextureSet(pTex_remote, 0, 0.0f);
		AEGfxMeshDraw(Mesh_about, AE_GFX_MDM_TRIANGLES);
	}

}

void MainUi::Free()
{
	
}

void MainUi::UnLoad()
{
	AEGfxMeshFree(BgMesh);
	AEGfxTextureUnload(pTexBg);
	AEGfxMeshFree(Mesh_about);
	AEGfxTextureUnload(pTex_about);
	AEGfxTextureUnload(pTex_remote);
}
