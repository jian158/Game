#pragma once
#ifndef _Win
#define _Win
#include "LevelBase.h"

class Winner :public LevelBase
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
