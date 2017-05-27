#pragma once
#ifndef _Over
#define _Over
#include "LevelBase.h"
class GameOver :public LevelBase
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
