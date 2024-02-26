#pragma once

#include "EventTrigger.h"

BEGIN(Client)

class CSceneTriggerCoin : public CEventTrigger
{

public:
	CSceneTriggerCoin();
	virtual ~CSceneTriggerCoin() = default;


public:
	virtual HRESULT Initialize(_float3 _vTriggerPos);
	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	void SetCameraEvent();

private:
	virtual void OnCollide(CGameObject::EObjType _eObjType, shared_ptr<CCollider> _pCollider) override;
	_bool m_bCollided = false;
	_float m_fEventAccTime = 0.f;
	_bool m_bUIOn = false;

public:
	static shared_ptr<CSceneTriggerCoin> Create(_float3 _vTriggerPos);


};

END