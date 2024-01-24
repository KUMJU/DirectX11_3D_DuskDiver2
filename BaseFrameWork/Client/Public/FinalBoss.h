#pragma once

#include "Client_Defines.h"
#include "Monster.h"

class CFinalBoss final: public CMonster
{
public:
	CFinalBoss();
	virtual ~CFinalBoss() = default;

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

private:
	_uint m_iTestNum = 0;


	_float m_fPattern4DelayTime = 4.f;
	_float m_fPatternCheckTime = 0.f;

	_bool m_bLaserOn = false;
	_bool m_bSetOriginLook = true;
	_float m_fTurnTime = 0.f;

	_vector m_vOriginLookVec = { 0.f, 0.f, 0.f, 0.f };
	


public:
	static shared_ptr<CFinalBoss> Create();


};

