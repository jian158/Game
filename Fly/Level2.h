/* 关卡2的属性和函数声明*/

#ifndef _Level2
#define _Level2
#include "LevelBase.h"
class Level2 :public LevelBase
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