#include "pch.h"
#include "SceneTriggerBear.h"

#include "CameraMgr.h"
#include "EventCamera.h"
#include "GameInstance.h"

#include "UIMgr.h"
#include "MonsterPool.h"
#include "Bear.h"

CSceneTriggerBear::CSceneTriggerBear()
{
}

HRESULT CSceneTriggerBear::Initialize(_float3 _vTriggerPos)
{
	CCollider::COLLIDER_DESC tColDesc = {};
	tColDesc.fRadius = 3.f;
	tColDesc.vCenter = { 0.f , 0.f, 0.f };

	__super::Initialize(tColDesc, _vTriggerPos);
	m_IsTouchAble = true;

	SetCameraEvent();

	m_pBear = CMonsterPool::GetInstance()->GetBearMonster();
	m_pBear->SetPosition({ -88.9394455f, 80.f, -90.f,1.f });
	dynamic_pointer_cast<CTransform>(m_pBear->GetComponent(TEXT("Com_Transform")))->SetScaling(9.f, 9.f , 9.f);
	dynamic_pointer_cast<CTransform>(m_pBear->GetComponent(TEXT("Com_Transform")))->Rotation({ 0.f, 1.f, 0.f }, XMConvertToRadians(90.f));

	m_vInitPos = { -88.9394455f, 80.f, -90.f,1.f };

	return S_OK;
}

void CSceneTriggerBear::PriorityTick(_float _fTimeDelta)
{
}

void CSceneTriggerBear::Tick(_float _fTimeDelta)
{
	if (m_bCollided) {
		m_fEventAccTime += _fTimeDelta;
		if (m_fEventAccTime > 1.f && m_fEventAccTime < 3.f) {

			m_fBearDropSpeed += 0.5f * _fTimeDelta + 0.02f;
			m_vInitPos.m128_f32[1] -= m_fBearDropSpeed;

			if (m_vInitPos.m128_f32[1] < 42.5f) {
				m_vInitPos.m128_f32[1] = 42.5f;
			}

			m_pBear->SetPosition(m_vInitPos);
		}
	}

	__super::Tick(_fTimeDelta);
}

void CSceneTriggerBear::LateTick(_float _fTimeDelta)
{
	__super::LateTick(_fTimeDelta);

}

HRESULT CSceneTriggerBear::Render()
{
    return S_OK;
}

void CSceneTriggerBear::SetCameraEvent()
{
	vector<CEventCamera::EVENT_INFO> StartEventVector;

	CEventCamera::EVENT_INFO StartEvent = {};


	StartEvent.eCamFunc = CEventCamera::FUNC_FOCUSING;
	StartEvent.vStart = { -83.f , 45.f, -85.f };
	StartEvent.vEnd = { -88.f , 45.f, -90.f };
	StartEvent.fDuration = 3.f;
	StartEvent.fCurrentDuration = 0.f;

	StartEventVector.push_back(StartEvent);

	CCameraMgr::GetInstance()->AddEventPreset(TEXT("BearCoinEvent"), StartEventVector);



}

void CSceneTriggerBear::OnCollide(CGameObject::EObjType _eObjType, shared_ptr<CCollider> _pCollider)
{
	if (!m_IsEnabled)
		return;

	if (m_bCollided)
		return;

	//플레이어 충돌
	if (CGameObject::EObjType::OBJ_PLAYER == _eObjType) {
		CCameraMgr::GetInstance()->StartEvent(TEXT("BearCoinEvent"));
		m_bCollided = true;
		m_pBear->SetEnable(true);
	}

}

shared_ptr<CSceneTriggerBear> CSceneTriggerBear::Create(_float3 _vTriggerPos)
{
	shared_ptr<CSceneTriggerBear> pInstance = make_shared<CSceneTriggerBear>();

	if (FAILED(pInstance->Initialize(_vTriggerPos)))
		MSG_BOX("Create to Failed : CSceneTriggerStart");

	return pInstance;
}
