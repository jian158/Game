/*�ؿ�1�����Ժͺ�������*/

#ifndef _Level4
#define _Level4
#include "LevelBase.h"
class Level4 :public LevelBase
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