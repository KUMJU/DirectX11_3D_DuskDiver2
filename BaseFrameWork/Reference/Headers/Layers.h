#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class CLayers final
{
public:
	CLayers();
	~CLayers();

public:
	HRESULT Initialize();
	HRESULT AddGameObject(shared_ptr<class CGameObject> _pGameObject);
	void PriorityTick(_float _fTimeDelta);
	void Tick(_float _fTimeDelta);
	void LateTick(_float _fTimeDelta);

public:

	list<shared_ptr<class CGameObject>> GetObjectList() { return m_GameObjects; }

private:
	list<shared_ptr<class CGameObject>> m_GameObjects;

public:
	static shared_ptr<CLayers> Create();
};

END