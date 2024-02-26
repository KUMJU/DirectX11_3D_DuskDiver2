#pragma once

#include "EventTrigger.h"

BEGIN(Client)

class CSceneTriggerInsert : public CEventTrigger
{
public:
	CSceneTriggerInsert();
	virtual ~CSceneTriggerInsert() = default;

public:
	virtual HRESULT Initialize(_float3 _vTriggerPos);
	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	virtual void OnCollide(CGameObject::EObjType _eObjType, shared_ptr<CCollider> _pCollider) override;
	_bool m_bCollided = false;
	_float m_fEventAccTime = 0.f;

public:
	static shared_ptr<CSceneTriggerInsert> Create(_float3 _vTriggerPos);

};

END