#pragma once
/*�ؿ�1�����Ժͺ�������*/

#ifndef _Level3
#define _Level3
#include "LevelBase.h"
class Level3 :public LevelBase
{
public:
	void Load() override;
	void Init()override;
	void Updata()override;
	void Draw()override;
	void Free()override;
	void UnLoad()override;
};
void Check();
#endif