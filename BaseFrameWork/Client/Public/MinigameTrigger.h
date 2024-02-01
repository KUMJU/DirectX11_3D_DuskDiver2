#pragma once

#include "Client_Defines.h"
#include "EventTrigger.h"

BEGIN(Client)

class CMinigameTrigger : public CEventTrigger
{
public:
	CMinigameTrigger();
	virtual ~CMinigameTrigger() = default;
	 
public:
	virtual HRESULT Initialize(shared_ptr<class CMinigame> _pMinigame, _float3 _vTriggerPos);
	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

private:

	shared_ptr<class CMinigame> m_pMinigame = nullptr;


public:
	virtual void OnCollide(CGameObject::EObjType _eObjType, shared_ptr<CCollider> _pCollider) override;

public:
	static shared_ptr<CMinigameTrigger> Create(shared_ptr<class CMinigame> _pMinigame, _float3 _vTriggerPos);

};

END