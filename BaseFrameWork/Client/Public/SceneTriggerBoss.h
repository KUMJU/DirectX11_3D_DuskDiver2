#pragma once

#include "EventTrigger.h"
#include "MonsterPool.h"

BEGIN(Client)

class CSceneTriggerBoss : public CEventTrigger
{
public:
	CSceneTriggerBoss();
	virtual ~CSceneTriggerBoss() = default;

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

//Event Checker
private:
	_bool m_bSpawnBoss = false;
	_bool m_bBossSpawnDone = false;

private:
	void SpawnBoss();


private:
	list<CMonsterPool::SPAWN_INFO> m_SpawnMonsterList;

public:
	static shared_ptr<CSceneTriggerBoss> Create(_float3 _vTriggerPos);


};

END