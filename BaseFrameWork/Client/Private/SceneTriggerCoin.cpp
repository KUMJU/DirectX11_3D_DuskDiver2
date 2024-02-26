#include "pch.h"
#include "SceneTriggerCoin.h"

#include "UIMgr.h"
#include "EventCamera.h"
#include "CameraMgr.h"


CSceneTriggerCoin::CSceneTriggerCoin()
{
}

HRESULT CSceneTriggerCoin::Initialize(_float3 _vTriggerPos)
{
	CCollider::COLLIDER_DESC tColDesc = {};
	tColDesc.fRadius = 3.f;
	tColDesc.vCenter = { 0.f , 0.f, 0.f };

	__super::Initialize(tColDesc, _vTriggerPos);
	m_IsTouchAble = true;
	SetCameraEvent();

	return S_OK;
}

void CSceneTriggerCoin::PriorityTick(_float _fTimeDelta)
{
}

void CSceneTriggerCoin::Tick(_float _fTimeDelta)
{

	if (!m_IsEnabled)
		return;

	if (m_bCollided) {
		m_fEventAccTime += _fTimeDelta;

		if (m_fEventAccTime >= 11.f && !m_bUIOn) {
			CUIMgr::GetInstance()->StartCoinQuest();
			CUIMgr::GetInstance()->StartQuest(TEXT("동전을 모아 기계를 작동한다"));
			m_IsActive = false;
		}
	}

	__super::Tick(_fTimeDelta);

}

void CSceneTriggerCoin::LateTick(_float _fTimeDelta)
{
	__super::LateTick(_fTimeDelta);

}

HRESULT CSceneTriggerCoin::Render()
{
	return S_OK;
}

void CSceneTriggerCoin::SetCameraEvent()
{
	vector<CEventCamera::EVENT_INFO> StartEventVector;

	CEventCamera::EVENT_INFO StartEvent = {};


	StartEvent.eCamFunc = CEventCamera::FUNC_FOCUSING;
	StartEvent.vStart = { -3.f, 26.f, -139.f };
	StartEvent.vEnd = { -3.f, 26.f, -140.f };
	StartEvent.fDuration = 3.f;
	StartEvent.fCurrentDuration = 0.f;

	StartEventVector.push_back(StartEvent);

	CCameraMgr::GetInstance()->AddEventPreset(TEXT("StartCoinQuest"), StartEventVector);


}

void CSceneTriggerCoin::OnCollide(CGameObject::EObjType _eObjType, shared_ptr<CCollider> _pCollider)
{

	if (!m_IsEnabled)
		return;

	if (m_bCollided)
		return;

	//플레이어 충돌
	if (CGameObject::EObjType::OBJ_PLAYER == _eObjType) {
		CUIMgr::GetInstance()->StartDialog(TEXT("StartCoinQuest"));
		CCameraMgr::GetInstance()->StartEvent(TEXT("StartCoinQuest"));
		m_bCollided = true;
	}
}

shared_ptr<CSceneTriggerCoin> CSceneTriggerCoin::Create(_float3 _vTriggerPos)
{
	shared_ptr<CSceneTriggerCoin> pInstance = make_shared<CSceneTriggerCoin>();

	if (FAILED(pInstance->Initialize(_vTriggerPos)))
		MSG_BOX("Create to Failed : CSceneTriggerStart");

	return pInstance;
}
