#pragma once

#include "Monster.h"

BEGIN(Client)

class CMiddleBoss final : public CMonster
{
public:
	CMiddleBoss();
	virtual ~CMiddleBoss() = default;

public:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

private:
	virtual void AttackPattern(_uint _iAtkNum) override;
	virtual void IfEmptyAnimList() override;
	virtual _bool CalcDistanceOption() override;

	void WalkPattern();

	_uint test= 0;


	void TurnToPlayer();
private:



public:

	static shared_ptr<CMiddleBoss> Create();

};


END
