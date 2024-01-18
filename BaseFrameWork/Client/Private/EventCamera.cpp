#include "pch.h"
#include "EventCamera.h"

#include "CameraMgr.h"
#include "GameMgr.h"
#include "Player.h"

CEventCamera::CEventCamera()
	:CCamera()
{
}

CEventCamera::~CEventCamera()
{
}

HRESULT CEventCamera::Initialize()
{
	SetPrevEvent();

	CTransform::TRANSFORM_DESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORM_DESC));

	TransformDesc.fSpeedPerSet = 10.0f;
	TransformDesc.fRotationPerSet = XMConvertToRadians(90.0f);

	CAMERA_DESC pCamDesc = {};
	pCamDesc.vEye = {0.f, 0.f, 0.f , 1.f};
	pCamDesc.vAt = { 0.f, 0.f, 0.f , 1.f };
	pCamDesc.fFovy = XMConvertToRadians(60.0f);
	pCamDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	pCamDesc.fNear = 0.2f;
	pCamDesc.fFar = 1000.f;

	__super::Initialize(&pCamDesc);

	_float4x4 identity;
	XMStoreFloat4x4(&identity, XMMatrixIdentity());

	m_pTransformCom->SetWorldMatrix(identity);

	m_IsEnabled = false;

	return S_OK;
}

void CEventCamera::PriorityTick(_float fTimeDelta)
{
	if (!m_IsEnabled)
		return;

}

void CEventCamera::Tick(_float fTimeDelta)
{
	if (!m_IsEnabled)
		return;

	(m_EventList)[m_iCurrentIdx].fCurrentDuration += fTimeDelta;

	if ((m_EventList)[m_iCurrentIdx].fCurrentDuration >= (m_EventList)[m_iCurrentIdx].fDuration) {

		(m_EventList)[m_iCurrentIdx].fCurrentDuration = 0.f;
		++m_iCurrentIdx;

		if (m_iCurrentIdx >= m_iMaxSize) {
			EndEvent();
			return;
		}
		else {
			EventInitialize();
		}
	}

	CCamera::SetUpTransformMatices();
	

}

void CEventCamera::LateTick(_float fTimeDelta)
{
	if (!m_IsEnabled)
		return;

	_vector vLook = m_pTransformCom->GetState(CTransform::STATE_LOOK);
	_vector vPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);


	switch ((m_EventList)[m_iCurrentIdx].eCamFunc)
	{
	case CEventCamera::FUNC_ZOOMIN:

		vPos += vLook * ((m_EventList)[m_iCurrentIdx].fSpeed * fTimeDelta);
		vPos.m128_f32[1] += (m_EventList)[m_iCurrentIdx].fDistance.y * fTimeDelta;

		m_pTransformCom->SetState(CTransform::STATE_POSITION, vPos);

		break;
	case CEventCamera::FUNC_ZOOMOUT:

		vPos -= vLook * ((m_EventList)[m_iCurrentIdx].fSpeed * fTimeDelta);

		vPos.m128_f32[1] += (m_EventList)[m_iCurrentIdx].fDistance.y * fTimeDelta;
		m_pTransformCom->SetState(CTransform::STATE_POSITION, vPos);

		


		break;
	case CEventCamera::FUNC_FOCUSING:
		FocusObject();
		break;
	case CEventCamera::FUNC_MOVING:
		break;
	case CEventCamera::FUNC_SHAKING:
		break;
	case CEventCamera::FUNC_IDLE:
		break;
	case CEventCamera::ENUM_END:
		break;
	default:
		break;
	}


}

HRESULT CEventCamera::Render()
{
	if (!m_IsEnabled)
		return S_OK;

	return S_OK;
}

void CEventCamera::AddPreset(const wstring& _strName, vector<EVENT_INFO> _info)
{
	if (m_PresetList.end() == m_PresetList.find(_strName)) {
		m_PresetList.insert({_strName, _info });
	}
}

void CEventCamera::ResetPreset()
{
}

void CEventCamera::StartEvent(const wstring& _strEventName)
{
	auto iter = m_PresetList.find(_strEventName);
	
	if (m_PresetList.end() != iter) {
		m_EventList = iter->second;
		m_iMaxSize = m_EventList.size();
		m_bEventStart = true;

		EventInitialize();
	}

}

void CEventCamera::EndEvent()
{
	m_EventList = {};
	m_bEventStart = true;
	m_iCurrentIdx = 0;
	m_iMaxSize = 0;
	CCameraMgr::GetInstance()->SwitchingCamera(CCameraMgr::ECAMERATYPE::THIRDPERSON);
}

void CEventCamera::SetInfoList(list<EVENT_INFO> _info)
{
}

void CEventCamera::LoadPreset(const _tchar* _szName)
{
}

void CEventCamera::FocusObject()
{
	



}

void CEventCamera::EventInitialize()
{

	switch ((m_EventList)[m_iCurrentIdx].eCamFunc)
	{
	case CEventCamera::FUNC_ZOOMIN:




		break;
	case CEventCamera::FUNC_ZOOMOUT:
		break;
	case CEventCamera::FUNC_FOCUSING:

		if ((m_EventList)[m_iCurrentIdx].pTarget) {

			_vector vTargetPos = (m_EventList)[m_iCurrentIdx].pTarget->GetState(CTransform::STATE_POSITION);
			_vector vTargetLook = (m_EventList)[m_iCurrentIdx].pTarget->GetState(CTransform::STATE_LOOK);

			_vector vCamPos = vTargetPos + vTargetLook* (m_EventList)[m_iCurrentIdx].fDistance.x ;
			vCamPos.m128_f32[1] = (m_EventList)[m_iCurrentIdx].fDistance.y;
			vTargetPos.m128_f32[1] = (m_EventList)[m_iCurrentIdx].fDistance.y;

			XMVectorSetW(vCamPos, 1.f);

			m_pTransformCom->SetState(CTransform::STATE_POSITION, vCamPos);
			m_pTransformCom->LookAt(vTargetPos);
		}

		break;
	case CEventCamera::FUNC_MOVING:
		break;
	case CEventCamera::FUNC_SHAKING:
		break;
	case CEventCamera::FUNC_IDLE:
		break;
	case CEventCamera::ENUM_END:
		break;
	
	default:
		break;
	}


}

void CEventCamera::SetPrevEvent()
{
	/***버스트 변신***/
	
	EVENT_INFO info;
	vector<EVENT_INFO> BurstVector;

	info.eCamFunc = ECAMERA_FUNCTION::FUNC_FOCUSING;
	info.fDuration = 0.7f;
	info.fCurrentDuration = 0.f;
	info.pTarget = dynamic_pointer_cast<CTransform>(CGameMgr::GetInstance()->GetPlayer()->GetComponent(TEXT("Com_Transform")));
	info.fDistance = {1.5f, 1.f, 1.5f};

	BurstVector.push_back(info);

	info.eCamFunc = ECAMERA_FUNCTION::FUNC_ZOOMIN;
	info.fDuration = 1.2f;
	info.fCurrentDuration = 0.f;
	info.fSpeed = 0.5f;
	info.fDistance = { 0.f, 0.f, 0.f };

	BurstVector.push_back(info);

	info.eCamFunc = ECAMERA_FUNCTION::FUNC_IDLE;
	info.fDuration = 0.3f;
	info.fCurrentDuration = 0.f;

	//Lerp

	BurstVector.push_back(info);

	info.eCamFunc = ECAMERA_FUNCTION::FUNC_ZOOMOUT;
	info.fDuration = 0.4f;
	info.fCurrentDuration = 0.f;
	info.fSpeed = 2.f;
	info.fDistance = { 0.f , 0.5f, 0.f };
	//Lerp

	BurstVector.push_back(info);


	info.eCamFunc = ECAMERA_FUNCTION::FUNC_IDLE;
	info.fDuration = 0.5f;
	info.fCurrentDuration = 0.f;

	//Lerp

	BurstVector.push_back(info);

	m_PresetList.insert({ TEXT("Event_Burst"),BurstVector });

	/**************************************************************************************************/


}

shared_ptr<CEventCamera> CEventCamera::Create()
{
	shared_ptr<CEventCamera> pInstance = make_shared<CEventCamera>();

	if (FAILED(pInstance->Initialize()))
		MSG_BOX("Create Failed : CEventCamera");

	return pInstance;
}
