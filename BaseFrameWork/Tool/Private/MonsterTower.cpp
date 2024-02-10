#include "pch.h"
#include "MonsterTower.h"
#include "GameInstance.h"

#include "Collider.h"

#include "Model.h"
#include "Shader.h"

#include "MonsterTrigger.h"
#include "MinigameTrigger.h"

#include "MinigameHockey.h"

CMonsterTower::CMonsterTower()
{
}

HRESULT CMonsterTower::Initialize(_uint _iTowerIdx)
{
	m_iTowerIdx = _iTowerIdx;
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

	TowerEventSetting();

	return S_OK;
}

void CMonsterTower::PriorityTick(_float _fTimeDelta)
{
	m_pRingTransform->SetState(CTransform::STATE_POSITION, m_pTransformCom->GetState(CTransform::STATE_POSITION));
	m_pRingTransform->Turn({ 0.f, 1.f, 0.f, 0.f }, _fTimeDelta);
}

void CMonsterTower::Tick(_float _fTimeDelta)
{

	m_pCollider->Tick(m_pTransformCom->GetWorldMatrix());
	CGameInstance::GetInstance()->AddCollider(CCollisionMgr::COL_MONSTER, m_pCollider);
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

	//g_vColor

	//Ÿ�� ���� Render
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

	_float4 vColor; 

	//Ÿ�� Ȱ��ȭ
	if (m_IsActived) {
		vColor = { 0.9f, 0.4f, 0.6f , 1.f };
	}
	else {
		vColor = { 0.6f, 0.6f, 0.2f, 1.f };
	}

	if (FAILED(m_pShader->BindRawValue("g_vColor", &vColor, sizeof(_float4))))
		return E_FAIL;


	const LIGHT_DESC* pLightDesc = CGameInstance::GetInstance()->GetLightDesc(0);
	if (nullptr == pLightDesc)
		return E_FAIL;


	for (size_t i = 0; i < iNumMeshes; i++) {


		if (FAILED(m_pRingModel->BindMaterialShaderResource(m_pShader, (_uint)i, aiTextureType::aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pShader->Begin(1)))
			return E_FAIL;

		if (FAILED(m_pRingModel->Render((_uint)i)))
			return E_FAIL;
	}

	return S_OK;
}

void CMonsterTower::OnCollide(EObjType _eObjType, shared_ptr<CCollider> _pCollider)
{
	if (EObjType::OBJ_PROJ == _eObjType) {
		//Trigger -> Active 
		if (m_IsActived)
			return;

		m_IsActived = true;
	}

	//�÷��̾� : �о�� ó��
	//�÷��̾� ����ü : onActive 


}

void CMonsterTower::TowerActive()
{
}

void CMonsterTower::TowerEventSetting()
{

	if (0 == m_iTowerIdx) {

		CMonsterPool::SPAWN_INFO info1 = {};
		info1.iMonsterType = 1;
		info1.vMonsterPos = _vector({ -10.f, 20.f, -90.f });
		m_SpawnMonsterList.push_back(info1);

		info1.iMonsterType = 0;
		info1.vMonsterPos = _vector({ 12.f, 20.f, -90.f });
		m_SpawnMonsterList.push_back(info1);

		shared_ptr<CMonsterTrigger> pTrigger = CMonsterTrigger::Create(&m_SpawnMonsterList, {0.f, 15.f, -94.f });
		CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_Event"), pTrigger);

	}
	else if (1 == m_iTowerIdx) {

		shared_ptr<CMinigameHockey> pMinigame = CMinigameHockey::Create();
		shared_ptr<CMinigameTrigger> pTrigger = CMinigameTrigger::Create(pMinigame, {0.f, 32.f, -305.f});
		CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_Map"), pTrigger);
		CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_Event"), pMinigame);

	}
	else if (2 == m_iTowerIdx) {
		CMonsterPool::SPAWN_INFO info1 = {};
		info1.iMonsterType = 1;

		info1.vMonsterPos = _vector({ -75.f, 35.f, -298.f });
		m_SpawnMonsterList.push_back(info1);

		info1.iMonsterType = 0;
		info1.vMonsterPos = _vector({ -75.f, 35.f, -310.f });
		m_SpawnMonsterList.push_back(info1);

		info1.iMonsterType = 1;
		info1.vMonsterPos = _vector({ -70.f, 35.f, -306.f });
		m_SpawnMonsterList.push_back(info1);

		shared_ptr<CMonsterTrigger> pTrigger = CMonsterTrigger::Create(&m_SpawnMonsterList, { -74.f, 35.f, -306.f });
		CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_Event"), pTrigger);
	}
	else if (3 == m_iTowerIdx) {



	}

}

shared_ptr<CMonsterTower> CMonsterTower::Create(_uint _iTowerIdx)
{
	shared_ptr<CMonsterTower> pInstance = make_shared<CMonsterTower>();

	if (FAILED(pInstance->Initialize(_iTowerIdx)))
		MSG_BOX("Failed to Create : MonsterTower");


	return pInstance;
}