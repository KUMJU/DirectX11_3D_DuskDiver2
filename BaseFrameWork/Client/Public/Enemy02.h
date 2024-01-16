#pragma once

#include "Client_Defines.h"
#include "Monster.h"


class CEnemy02 :public CMonster
{
public:
	CEnemy02();
	virtual ~CEnemy02() = default;


public:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

private:
	virtual void AttackPattern(_uint _iAtkNum) override;
	void IdlePattern(_uint _iAtkNum);
	void WalkPattern(_uint _iWalkNum);

	void IfEmptyAnimList() override;
	void CalcDistanceOption() override;

public:
	static shared_ptr<CEnemy02> Create();


};

