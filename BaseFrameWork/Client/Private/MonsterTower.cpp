#include "pch.h"
#include "MonsterTower.h"
#include "GameInstance.h"

#include "Collider.h"

#include "Model.h"
#include "Shader.h"

CMonsterTower::CMonsterTower()
{
}

HRESULT CMonsterTower::Initialize()
{
	__super::Initialize(TEXT("TowerA"),nullptr);

	m_pRingModel = CGameInstance::GetInstance()->GetModel(TEXT("TowerAa"));


	CCollider::COLLIDER_DESC TowerCollDesc = {};

	TowerCollDesc.vCenter = _float3(0.f, 1.f, 0.f);
	TowerCollDesc.vExtents = _float3(0.5f, 1.f, 0.5f);
	TowerCollDesc.fRadius = 1.f;

	CTransform::TRANSFORM_DESC desc = {};
	desc.fRotationPerSet = 2.f;
	m_pRingTransform = CTransform::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), &desc);
	m_pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_SPHERE, TowerCollDesc);
	m_pCollider->SetOwner(shared_from_this());

	//TestData////

	_vector vPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);

	CMonsterPool::SPAWN_INFO info1 = {};
	info1.iMonsterType = 1;
	info1.vMonsterPos = _vector({ -1.f, 10.f, -90.f });
		//vPos + _vector({2.f, 0.f, 0.f, 0.f});
	
	m_SpawnMonsterList.push_back(info1);

	return S_OK;
}

void CMonsterTower::PriorityTick(_float _fTimeDelta)
{
	m_pRingTransform->SetState(CTransform::STATE_POSITION, m_pTransformCom->GetState(CTransform::STATE_POSITION));
	m_pRingTransform->Turn({ 0.f, 1.f, 0.f, 0.f }, _fTimeDelta);
}

void CMonsterTower::Tick(_float _fTimeDelta)
{

	if (!m_IsActived) {
		m_pCollider->Tick(m_pTransformCom->GetWorldMatrix());
		CGameInstance::GetInstance()->AddCollider(CCollisionMgr::COL_MONSTER, m_pCollider);
	}
	//collision Manager Add

}

void CMonsterTower::LateTick(_float _fTimeDelta)
{
	__super::LateTick(_fTimeDelta);

}

HRESULT CMonsterTower::Render()
{

	m_pCollider->Render();

	__super::Render();

	//타워 고리 Render
	_uint iNumMeshes = m_pRingModel->GetNumMeshes();

	if (FAILED(m_pRingTransform->BindShaderResource(m_pShader, "g_WorldMatrix")))
		return E_FAIL;

	_float4x4 ViewMat = CGameInstance::GetInstance()->GetTransformFloat4x4(CPipeLine::D3DTS_VIEW);

	if (FAILED(m_pShader->BindMatrix("g_ViewMatrix", &ViewMat)))
		return E_FAIL;

	_float4x4 ProjMat = CGameInstance::GetInstance()->GetTransformFloat4x4(CPipeLine::D3DTS_PROJ);

	if (FAILED(m_pShader->BindMatrix("g_ProjMatrix", &ProjMat)))
		return E_FAIL;

	_float4 CamPos = CGameInstance::GetInstance()->GetCamPosition();

	if (FAILED(m_pShader->BindRawValue("g_vCamPosition", &CamPos, sizeof(_float4))))
		return E_FAIL;

	const LIGHT_DESC* pLightDesc = CGameInstance::GetInstance()->GetLightDesc(0);
	if (nullptr == pLightDesc)
		return E_FAIL;


	for (size_t i = 0; i < iNumMeshes; i++) {


		if (FAILED(m_pRingModel->BindMaterialShaderResource(m_pShader, (_uint)i, aiTextureType::aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pShader->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pRingModel->Render((_uint)i)))
			return E_FAIL;
	}

	return S_OK;
}

void CMonsterTower::OnCollide(EObjType _eObjType, shared_ptr<CCollider> _pCollider)
{
	if (EObjType::OBJ_PROJ == _eObjType) {

		if (m_IsActived)
			return;

		m_IsActived = true;
		CMonsterPool::GetInstance()->ActiveMonster(m_SpawnMonsterList);


	}

	//플레이어 : 밀어내기 처리
	//플레이어 투사체 : onActive 


}

void CMonsterTower::TowerActive()
{
}

shared_ptr<CMonsterTower> CMonsterTower::Create()
{
	shared_ptr<CMonsterTower> pInstance = make_shared<CMonsterTower>();

	if (FAILED(pInstance->Initialize()))
		MSG_BOX("Failed to Create : MonsterTower");


	return pInstance;
}
