#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CMinigameMole : public CGameObject
{
public:
	CMinigameMole();
	virtual ~CMinigameMole() = default;

public:
	virtual HRESULT Initialize(CTransform::TRANSFORM_DESC* _pDesc);

	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();


public:
	static shared_ptr<CMinigameMole> Create();
};

END