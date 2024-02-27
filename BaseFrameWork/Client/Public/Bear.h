#pragma once

#include "Monster.h"

BEGIN(Client)


//사실상 NPC에 가까운데 피타격이 가능해야하기 때문에 
//기능적으로 몬스터에 분류하면 편할 거 같아서 몬스터 상속으로 만듦

class CBear : public CMonster
{
public:
	CBear();
	virtual ~CBear() = default;

public:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	void ChangeModel(_int _iModelNum);

public:
	virtual void OnHit() override;
	virtual void SetSpawnState() override;

	virtual void AttackPattern(_uint _iAtkNum) {};
	virtual void IfEmptyAnimList(void) {};
	virtual _bool CalcDistanceOption(void) { return true;};

	void MovingFront(_vector _vDstPos);
	void SetShaking(_float _fShakeDistance, _float _fShakeTime);
	void StartTurn() { m_bTurn = true; }

private:
	void Shaking(_float _fTimeDelta);

private:
	_float m_fMovingTime = false;
	_bool m_bMove = false;
	_vector m_vMoveDistancePerTick = _vector();
	_vector m_vDstPos = _vector();

	_bool m_bFirstShaking = false;

	_bool m_bTurn = false;

	_int m_iHitNum = 0;
	_bool m_bRunaway = false;
	_float m_fDialogAccTime = 0.f;
private:

	_bool m_bShaking = false;
	_float m_fXOffset = 0.f;
	_float m_fTotalOffset = 0.3f;
	_float m_fShakingSpeed = 0.3f;
	_float m_fShakingTotalTime = 0.f;
	_vector m_vBeforeShakingPos = _vector();

	_bool m_bChangeDir = false;
	_float m_iDir = -1.f;

	_float m_fShakingAccTime = 0.f;
	_float m_fShakingTotalAccTime = 0.f;

private:
	shared_ptr<class CSceneTriggerInsert> m_pTrigger = nullptr;

private:
	//맞으면 얼굴이 바뀐다 x0 < 이런식으로..
	shared_ptr<CModel> m_pHitModel = nullptr;

public:
	static shared_ptr<CBear> Create();

};

END