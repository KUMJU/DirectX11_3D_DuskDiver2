#pragma once

#include "Monster.h"

BEGIN(Client)


class CEnemy01 final : public CMonster
{
public:
	enum class EENEMY01_ATTACK {
		ENEMY01_ATTACK_1,
		ENEMY01_ATTACK_2,
		ENEMY01_ATTACK_3,
		ENEMY01_ATTACK_END
	};

public:
	CEnemy01();
	virtual ~CEnemy01() = default;

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
	_bool CalcDistanceOption() override;

public:
	virtual void OnHit() override;
	virtual void SetSpawnState() override;

	void ResetState();


private:
	_uint m_iWalkPatternNum = 0;	

public:
	static shared_ptr<CEnemy01> Create();

};

END