#pragma once

#include "EventTrigger.h"

BEGIN(Client)

class CSceneTriggerStart : public CEventTrigger
{
public:
	CSceneTriggerStart();
	virtual ~CSceneTriggerStart() = default;

public:
	virtual HRESULT Initialize( _float3 _vTriggerPos);
	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	void SetCameraEvent();

private:

	_bool m_bFade = false;
	_bool m_bTitleVisible = false;

	_float m_fEventAccTime = 0.f;

private:
	virtual void OnCollide(CGameObject::EObjType _eObjType, shared_ptr<CCollider> _pCollider) override;
	_bool m_bCollided = false;


public:
	static shared_ptr<CSceneTriggerStart> Create( _float3 _vTriggerPos);

};

END