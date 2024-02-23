#pragma once

#include "Monster.h"

BEGIN(Client)


//��ǻ� NPC�� ���� ��Ÿ���� �����ؾ��ϱ� ������ 
//��������� ���Ϳ� �з��ϸ� ���� �� ���Ƽ� ���� ������� ����

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
	//������ ���� �ٲ�� x0 < �̷�������..
	shared_ptr<CModel> m_pHitModel = nullptr;

public:
	static shared_ptr<CBear> Create();

};

END