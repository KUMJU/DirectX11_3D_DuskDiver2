#include "pch.h"
#include "SceneTriggerStart.h"

#include "CameraMgr.h"
#include "EventCamera.h"
#include "GameInstance.h"

#include "UIMgr.h"

CSceneTriggerStart::CSceneTriggerStart()
{
}

HRESULT CSceneTriggerStart::Initialize(_float3 _vTriggerPos)
{
	CCollider::COLLIDER_DESC tColDesc = {};
	tColDesc.fRadius = 3.f;
	tColDesc.vCenter = { 0.f , 0.f, 0.f };

	__super::Initialize(tColDesc, _vTriggerPos);
	m_IsTouchAble = true;

	SetCameraEvent();

	return S_OK;
}

void CSceneTriggerStart::PriorityTick(_float _fTimeDelta)
{
}

void CSceneTriggerStart::Tick(_float _fTimeDelta)
{


	if (m_bCollided) {
		m_fEventAccTime += _fTimeDelta;

		if (m_fEventAccTime >= 2.f && !m_bFade) {
			CCameraMgr::GetInstance()->StartEvent(TEXT("StartEventTest"));
			m_bFade = true;
			CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_BGM);
			CGameInstance::GetInstance()->PlayBGM(TEXT("BGM_MainTheme.wav"), 1.f);
		}


		if (m_fEventAccTime >= 14.f && !m_bTitleVisible) {
			CUIMgr::GetInstance()->StartQuest(TEXT("오락실 내부를 조사한다"));
			m_bTitleVisible = true;

		
		}

		if (m_fEventAccTime >= 19.f) {
			CUIMgr::GetInstance()->HUDOn();
			m_IsActive = false;
		}
	}



	__super::Tick(_fTimeDelta);
}

void CSceneTriggerStart::LateTick(_float _fTimeDelta)
{
	__super::LateTick(_fTimeDelta);

}

HRESULT CSceneTriggerStart::Render()
{
    return S_OK;
}

void CSceneTriggerStart::SetCameraEvent()
{
	vector<CEventCamera::EVENT_INFO> StartEventVector;

	CEventCamera::EVENT_INFO StartEvent = {};


	StartEvent.eCamFunc = CEventCamera::FUNC_MOVING;
	StartEvent.vStart = { 150.f, 41.5f, -303.5f };
	StartEvent.vEnd = { 150.f, 41.5f, -299.f };
	StartEvent.fDistance = { 1.5f, 0.f, 0.f };
	StartEvent.fSpeed = 9.f;
	StartEvent.fDuration = 3.f;
	StartEvent.fCurrentDuration = 0.f;


	StartEventVector.push_back(StartEvent);


	StartEvent.eCamFunc = CEventCamera::FUNC_MOVING;
	StartEvent.vStart = { 75.f, 41.5f, -303.5f };
	StartEvent.vEnd = { 88.f, 41.5f, -299.f };
	StartEvent.fDistance = { 1.5f, 0.f, 0.f };
	StartEvent.fSpeed = 2.f;
	StartEvent.fDuration = 3.f;
	StartEvent.fCurrentDuration = 0.f;


	StartEventVector.push_back(StartEvent);

	StartEvent.eCamFunc = CEventCamera::FUNC_MOVING;
	StartEvent.vStart = { -100.f, 45.5f, -303.5f };
	StartEvent.vEnd = { -100.f, 45.5f, -299.f };
	StartEvent.fDistance = { 1.f, 0.f, -1.f };
	StartEvent.fSpeed = 4.f;
	StartEvent.fDuration = 3.f;
	StartEvent.fCurrentDuration = 0.f;


	StartEventVector.push_back(StartEvent);

	StartEvent.eCamFunc = CEventCamera::FUNC_MOVING;
	StartEvent.vStart = { 0.f, 45.5f, -400.5f };
	StartEvent.vEnd = { 0.f, 22.5f, -100.f };
	StartEvent.fDistance = { 0.f, 0.f, -1.f };
	StartEvent.fSpeed = 1.4f;
	StartEvent.fDuration = 5.f;
	StartEvent.fCurrentDuration = 0.f;


	StartEventVector.push_back(StartEvent);

	StartEvent.eCamFunc = CEventCamera::FUNC_IDLE;
	StartEvent.fDuration = 3.f;
	StartEvent.fCurrentDuration = 0.f;

	StartEventVector.push_back(StartEvent);


	CCameraMgr::GetInstance()->AddEventPreset(TEXT("StartEventTest"), StartEventVector);



}

void CSceneTriggerStart::OnCollide(CGameObject::EObjType _eObjType, shared_ptr<CCollider> _pCollider)
{
	if (!m_IsEnabled)
		return;

	if (m_bCollided)
		return;

	//플레이어 충돌
	if (CGameObject::EObjType::OBJ_PLAYER == _eObjType) {
		shared_ptr<CGameObject> pPlayer = _pCollider->GetOwner();
		CUIMgr::GetInstance()->StartScreenEffect(CUIScreenEffect::TYPE_FADE);
		m_bCollided = true;
	}


}

shared_ptr<CSceneTriggerStart> CSceneTriggerStart::Create( _float3 _vTriggerPos)
{
	shared_ptr<CSceneTriggerStart> pInstance = make_shared<CSceneTriggerStart>();

	if (FAILED(pInstance->Initialize(_vTriggerPos)))
		MSG_BOX("Create to Failed : CSceneTriggerStart");

    return pInstance;
}
