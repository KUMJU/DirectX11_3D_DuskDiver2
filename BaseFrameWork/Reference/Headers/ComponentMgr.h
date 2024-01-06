#pragma once

#include "Engine_Defines.h"
#include "VIRect.h"
#include "Shader.h"
#include "Texture.h"

BEGIN(Engine)

class CComponentMgr
{
public:
	CComponentMgr();
	~CComponentMgr();

public:
	HRESULT Initialize(_uint _iNumLevels);
	HRESULT AddPrototype(_uint _iLevelIndex, const wstring& _strPrototypeTag, shared_ptr<class CComponent> _pPrototype);
	shared_ptr<class CComponent> CloneComponent(_uint _iLevelIndex, const wstring& _strProtoTag, shared_ptr<void> _pArg);

private:
	_uint m_iNumLevels = { 0 };

	map<const wstring, shared_ptr<class CComponent>>* m_pPrototypes;
	typedef map<const wstring, shared_ptr<class CComponent>> PROTOTYPES;


private:
	shared_ptr<class CComponent> FindPrototype(_uint _iLevelIndex, const wstring& _strProtoTag);

public:
	static shared_ptr<CComponentMgr> Create(_uint _iNumLevels);

public:
	void Clear(_uint _iLevelNum);
	void Free();


};

END