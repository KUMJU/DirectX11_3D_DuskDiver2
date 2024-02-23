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
	virtual _bool CalcDistanceOption(void) { return true;  };
private:
	//맞으면 얼굴이 바뀐다 x0 < 이런식으로..
	shared_ptr<CModel> m_pHitModel = nullptr;

public:
	static shared_ptr<CBear> Create();

};

END