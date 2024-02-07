#include "pch.h"
#include "MonsterTrigger.h"
#include "BattleSystem.h"

CMonsterTrigger::CMonsterTrigger()
{
}

HRESULT CMonsterTrigger::Initialize(list<CMonsterPool::SPAWN_INFO>* _info, _float3 _vTriggerPos)
{
	m_eTriggerType = ETRIGGER_TYPE::TRI_MONSTER;

	CCollider::COLLIDER_DESC colDesc = {};
	colDesc.vCenter = _float3(0.f, 0.4f, 0.f);
	colDesc.fRadius = 1.f;

	__super::Initialize(colDesc, _vTriggerPos);

	m_SpawnMonsterList = *_info;

	return S_OK;
}

void CMonsterTrigger::PriorityTick(_float _fTimeDelta)
{
}

void CMonsterTrigger::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

}

void CMonsterTrigger::LateTick(_float _fTimeDelta)
{
	__super::LateTick(_fTimeDelta);

}

HRESULT CMonsterTrigger::Render()
{
	__super::Render();

	return S_OK;
}

void CMonsterTrigger::OnCollide(CGameObject::EObjType _eObjType, shared_ptr<CCollider> _pCollider)
{

	if (EObjType::OBJ_PROJ == _eObjType) {
		CMonsterPool::GetInstance()->ActiveMonster(m_SpawnMonsterList);
		CBattleSystem::GetInstance()->BattleStart();

		m_IsActive = false;
	}

}

shared_ptr<CMonsterTrigger> CMonsterTrigger::Create(list<CMonsterPool::SPAWN_INFO>* _info, _float3 _vTriggerPos)
{
	shared_ptr<CMonsterTrigger> pTrigger =  make_shared<CMonsterTrigger>();

	if (FAILED(pTrigger->Initialize(_info, _vTriggerPos))) {
		MSG_BOX("Create to Failed : CMonsterTrigger");
	}

	return pTrigger;
}
