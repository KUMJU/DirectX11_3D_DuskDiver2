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
	_bool CalcDistanceOption() override;


private:

	void ResetState();

private:

	_uint m_iNextPattern = 0;

	_bool m_bDefenceMode = false;
	_bool m_bDefenceAble = true;
	_float m_fDefenceCoolTime = 0.f;

	_int m_iDefenceGage = 50;

	_float m_JumpTimeCheck = 0.f;

	/////////������ �÷��̾� ����x , �����ϰ� �ڸ����� ��� ///////

	_bool m_bWait = false;
	_float m_fWaitTime = 0.f;

protected:

	virtual void OnHit() override;
	virtual void SetSpawnState() override;

public:
	static shared_ptr<CEnemy02> Create();


};

