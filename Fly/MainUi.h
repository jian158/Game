#pragma once
#ifndef _MainUi
#define _MainUi

#include "LevelBase.h"
#include "AEEngine/AEEngine.h"

class MainUi:public LevelBase
{
	AEGfxVertexList *BgMesh;
	AEGfxTexture	*pTexBg;
public:
	void Load() override;
	void Init()override;
	void Updata()override;
	void Draw()override;
	void Free()override;
	void UnLoad()override;
};
#endif
