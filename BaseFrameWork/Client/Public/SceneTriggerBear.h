#pragma once

#include "EventTrigger.h"

BEGIN(Client)

class CSceneTriggerBear : public CEventTrigger
{
public:
	CSceneTriggerBear();
	virtual ~CSceneTriggerBear() = default;

public:
	virtual HRESULT Initialize(_float3 _vTriggerPos);
	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	void SetCameraEvent();

private:
	_bool m_bTitleVisible = false;
	_float m_fEventAccTime = 0.f;

	_float m_fBearDropSpeed = 0.f;
	_vector m_vInitPos = _vector();

private:
	virtual void OnCollide(CGameObject::EObjType _eObjType, shared_ptr<CCollider> _pCollider) override;
	_bool m_bCollided = false;

private:
	shared_ptr<class CBear> m_pBear = nullptr;

public:
	static shared_ptr<CSceneTriggerBear> Create(_float3 _vTriggerPos);


};

END