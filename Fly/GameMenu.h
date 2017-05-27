#pragma once
#ifndef _Menu
#define _Menu
#include "LevelBase.h"
class GameMenu:public LevelBase
{
public:
	void Load() override;
	void Init()override;
	void Updata()override;
	void Draw()override;
	void Free()override;
	void UnLoad()override;
};
#endif
