#pragma once

#include "Client_Defines.h"
#include "EventTrigger.h"

BEGIN(Client)

class CPortal : public CEventTrigger
{
public:
	CPortal();
	virtual ~CPortal() = default;

public:
	virtual HRESULT Initialize(_float3 vWarpPos, _float3 _vTriggerPos);
	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

private:
	//플레이어를 이동시켜줄 지점 
	_float3 vWarpPosition = _float3();


private:
	virtual void OnCollide(CGameObject::EObjType _eObjType, shared_ptr<CCollider> _pCollider) override;

public:
	static shared_ptr<CPortal> Create(_float3 _vWarpPos, _float3 _vTriggerPos);

};

END