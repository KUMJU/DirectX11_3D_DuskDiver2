#pragma once

#include "Monster.h"

class CEnemy03 final : public CMonster
{
public:
	CEnemy03();
	virtual ~CEnemy03() = default;

public:
	virtual HRESULT Initialize(CTransform::TRANSFORM_DESC* _pDesc);
	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();


private:


};

