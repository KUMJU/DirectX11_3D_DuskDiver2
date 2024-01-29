#include "pch.h"
#include "MonsterTower.h"
#include "GameInstance.h"

#include "Collider.h"
#include "CollisionMgr.h"

CMonsterTower::CMonsterTower()
{
}

HRESULT CMonsterTower::Initialize()
{

	m_pModelCom = CGameInstance::GetInstance()->GetModel(TEXT("TowerA"));
	m_pRingModel = CGameInstance::GetInstance()->GetModel(TEXT("TowerAa"));


	CCollider::COLLIDER_DESC TowerCollDesc = {};

	TowerCollDesc.vCenter = _float3(0.f, 1.f, 0.f);
	TowerCollDesc.vExtents = _float3(0.3f, 0.5f, 0.3f);

	m_pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_AABB, TowerCollDesc);

	return S_OK;
}

void CMonsterTower::PriorityTick(_float _fTimeDelta)
{
}

void CMonsterTower::Tick(_float _fTimeDelta)
{
	m_pCollider->Tick(m_pTransformCom->GetWorldMatrix());
	//collision Manager Add

}

void CMonsterTower::LateTick(_float _fTimeDelta)
{


}

HRESULT CMonsterTower::Render()
{

	m_pCollider->Render();

	return S_OK;
}

void CMonsterTower::OnCollide(EObjType _eObjType, shared_ptr<CCollider> _pCollider)
{

	//플레이어 : 밀어내기 처리
	//플레이어 투사체 : onActive 


}

shared_ptr<CMonsterTower> CMonsterTower::Create()
{
	shared_ptr<CMonsterTower> pInstance = make_shared<CMonsterTower>();

	if (FAILED(pInstance->Initialize()))
		MSG_BOX("Failed to Create : MonsterTower");


	return pInstance;
}
