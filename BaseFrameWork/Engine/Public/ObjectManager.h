#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class CGameObject;

class CObjectManager
{
public:
	CObjectManager();
	~CObjectManager();

public:
	HRESULT Initialize(_uint _iNumLevels);
	void PriorityTick(_float fTimeDelta);
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);

public:
	HRESULT AddObject(_uint iLevelIndex, const wstring& strLayerTag, shared_ptr<CGameObject> _pGameObject);

public:
	shared_ptr<class CLayers> FindLayer(_uint _iLevelIndex, const wstring& _strLayerTag);

private:
	_uint m_iNumLevels = { 0 };
	map<const wstring, class shared_ptr<class CLayers>>* m_pLayers = { nullptr };
	typedef map<const wstring, shared_ptr<class CLayers>> LAYERS;

public:
	static shared_ptr<CObjectManager> Create(_uint iNumLevels);
	void Clear(_uint _iLevelNum);
	void Free();
};

END
