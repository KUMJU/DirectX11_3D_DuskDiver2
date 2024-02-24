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


private:
	void Shaking(_float _fTimeDelta);

private:
	_float m_fMovingTime = false;
	_bool m_bMove = false;
	_vector m_vMoveDistancePerTick = _vector();
	_vector m_vDstPos = _vector();

private:

	_bool m_bShaking = false;
	_float m_fXOffset = 0.f;
	_float m_fTotalOffset = 0.3f;
	_float m_fShakingSpeed = 2.f;

	_bool m_bChangeDir = false;
	_float m_iDir = -1.f;

	_float m_fShakingAccTime = 0.f;


private:
	//맞으면 얼굴이 바뀐다 x0 < 이런식으로..
	shared_ptr<CModel> m_pHitModel = nullptr;

public:
	static shared_ptr<CBear> Create();

};

END