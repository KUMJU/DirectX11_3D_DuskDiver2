#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CMinigame abstract : public CGameObject
{
public:
	CMinigame();
	virtual ~CMinigame() = default;

public:
	virtual HRESULT Initialize();

	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	void GameStart();
	virtual void GameEnd();

protected:
	_bool m_bDone = false;
	_bool m_bProcessing = false;

};



END