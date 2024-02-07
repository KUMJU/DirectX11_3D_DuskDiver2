#pragma once

#include "Tool_Defines.h"
#include "EventTrigger.h"
#include "MonsterPool.h"

BEGIN(Tool)

class CMonsterTrigger : public CEventTrigger
{
public:
	CMonsterTrigger();
	virtual ~CMonsterTrigger() = default;

public:
	virtual HRESULT Initialize(list<CMonsterPool::SPAWN_INFO>* _info, _float3 _vTriggerPos);
	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void OnCollide(CGameObject::EObjType _eObjType, shared_ptr<CCollider> _pCollider) override;

private:
	list<CMonsterPool::SPAWN_INFO> m_SpawnMonsterList;

public:
	static shared_ptr<CMonsterTrigger> Create(list<CMonsterPool::SPAWN_INFO>* _info, _float3 _vTriggerPos);

};

END