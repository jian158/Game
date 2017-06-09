#pragma once

#ifndef _LevelBase
class LevelBase
{
public:
	virtual ~LevelBase() = default;
	virtual void Load()=0;
	virtual void Init()=0;
	virtual void Updata()=0;
	virtual void Draw()=0;
	virtual void Free()=0;
	virtual void UnLoad()=0;
};
extern int sShipLives, Skills, spShipBullet, autoShoot;
#endif
