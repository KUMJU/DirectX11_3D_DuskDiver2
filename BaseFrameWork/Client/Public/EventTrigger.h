#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Collider.h"

BEGIN(Engine)

class CEventTrigger : public CGameObject
{
public:
	CEventTrigger();
	virtual ~CEventTrigger() = default;

public:
	virtual HRESULT Initialize(CCollider::COLLIDER_DESC _desc, _float3 _vTriggerPos);
	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	shared_ptr<CCollider> m_pCollider = nullptr;

};

END