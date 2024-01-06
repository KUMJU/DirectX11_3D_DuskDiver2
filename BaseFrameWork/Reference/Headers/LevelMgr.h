#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class CLevelMgr final
{
public:
	CLevelMgr();
	~CLevelMgr() = default;

public:
	HRESULT Initialize();
	void Tick(_float fTimeDelta);
	//디버깅 or 확인용 폰트 출력
	HRESULT Render();
	HRESULT Open_Level(_uint iLevelIndex, shared_ptr<class CLevel> pNewLevel);

public:
	_uint GetCurrentLevel() { return m_iLevelIndex; }

private:
	_uint m_iLevelIndex = { 0 };
	shared_ptr<class CLevel> m_pCurrentLevel = { nullptr };

public:

	static shared_ptr<CLevelMgr> Create();
	void Free();

};

END
