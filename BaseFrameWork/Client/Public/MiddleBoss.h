#pragma once

#include "Monster.h"

BEGIN(Client)

class CMiddleBoss final : public CMonster
{
public:
	CMiddleBoss();
	virtual ~CMiddleBoss() = default;

public:
	virtual HRESULT Initialize(CTransform::TRANSFORM_DESC* _pDesc);
	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void AttackPattern(_uint _iAtkNum) override;
	virtual void IfEmptyAnimList() override;

};


END
