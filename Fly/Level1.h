/*关卡1的属性和函数声明*/

#ifndef _Level1
#define _Level1
#include "LevelBase.h"
class Level1 :public LevelBase
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