#pragma once

#include "Client_Defines.h"
#include "Monster.h"


BEGIN(Client)

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


public:
	void StartSpecialPattern();
	void EndSpecialPattern();


private:
	virtual void SetSpawnState() override;

	void CalcSuperArmorTimeBoss(_float _fTimeDelta);

protected:
	virtual void OnHit() override;


private:
	_uint m_iTestNum = 0;


	_float m_fPattern4DelayTime = 10.f;
	_float m_fPatternCheckTime = 0.f;

	_bool m_bLaserOn = false;
	
	_bool m_bLaserGaugeSeDone = false;
	_bool m_bLaserSeDone = false;


	_bool m_bSetOriginLook = true;
	_float m_fTurnTime = 0.f;

	_vector m_vOriginLookVec = { 0.f, 0.f, 0.f, 0.f };


private:
	shared_ptr<class CEffectPreset> m_pLaserEffectPreset = nullptr;
	shared_ptr<class CEffectPreset> m_pLaserEffectPreset2 = nullptr;

private:

	shared_ptr<class CBossHPBar> m_pHPBar = nullptr;

private:
	shared_ptr<CCollider> m_pCollider2 = nullptr;

private:
	shared_ptr<CEffectPreset> m_pSpawnEffect = nullptr;

// Special Pattern 
private:

	_bool m_bSpecialPatternStart = false;
	shared_ptr<class CSpecialBossPattern> m_pSpecialPattern = nullptr;

public:
	static shared_ptr<CFinalBoss> Create();


};


END