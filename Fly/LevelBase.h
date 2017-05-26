#pragma once
class LevelBase
{
public:
	virtual ~LevelBase() = default;
	virtual void Load();
	virtual void Init();
	virtual void Updata();
	virtual void Draw();
	virtual void Free();
	virtual void UnLoad();
};

//class SystemManage
//{
//	void(*pLoad)(void);
//	void(*pIni)(void);
//	void(*pUpdate)(void);
//	void(*pDraw)(void);
//	void(*pFree)(void);
//	void(*pUnload)(void);
//public:
//	SystemManage(void Load(), void Init(), void Update(), void Draw(), void Free(), void UnLoad());
//};
//
//inline SystemManage::SystemManage(void Load(), void Init(), void Update(), void Draw(), void Free(), void UnLoad())
//{
//	Load();
//}
