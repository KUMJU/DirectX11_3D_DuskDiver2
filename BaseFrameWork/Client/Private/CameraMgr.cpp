#include "pch.h"
#include "CameraMgr.h"

#include "ThirdPersonCam.h"
#include "CameraFree.h"

#include "GameMgr.h"
#include "Player.h"

IMPLEMENT_SINGLETON(CCameraMgr)

CCameraMgr::CCameraMgr()
{
}

void CCameraMgr::SwitchingCamera(ECAMERATYPE _eType)
{

	if (_eType == m_eCurrentCamType)
		return;


	switch (_eType)
	{
	case Client::CCameraMgr::ECAMERATYPE::THIRDPERSON:
		if (!m_pDefualtCam)
			break;
		m_pMainCam->SetEnable(false);
		m_pDefualtCam->SetEnable(true);
		
		m_pMainCam = m_pDefualtCam;

		break;
	case Client::CCameraMgr::ECAMERATYPE::FREE:
		if (!m_pFreeCam)
			break;

		m_pMainCam->SetEnable(false);
		m_pFreeCam->SetEnable(true);

		m_pMainCam = m_pFreeCam;

		break;
	case Client::CCameraMgr::ECAMERATYPE::EVENT:

		if (!m_pEventCam)
			break;

		m_pMainCam->SetEnable(false);
		m_pEventCam->SetEnable(true);

		m_pMainCam = m_pEventCam;

		break;
	case Client::CCameraMgr::ECAMERATYPE::ENUM_END:
		break;
	default:
		break;
	}

	m_eCurrentCamType = _eType;


}

void CCameraMgr::SetCamObject(ECAMERATYPE _eType, shared_ptr<CGameObject> _pInstance)
{
	if (!_pInstance)
		return;

	switch (_eType)
	{
	case Client::CCameraMgr::ECAMERATYPE::THIRDPERSON:
		m_pDefualtCam = dynamic_pointer_cast<CThirdPersonCam>(_pInstance);
		m_pMainCam = m_pDefualtCam;
		break;
	case Client::CCameraMgr::ECAMERATYPE::FREE:
		m_pFreeCam = dynamic_pointer_cast<CCameraFree>(_pInstance);
		break;
	case Client::CCameraMgr::ECAMERATYPE::EVENT:
		m_pEventCam = dynamic_pointer_cast<CEventCamera>(_pInstance);
		break;
	case Client::CCameraMgr::ECAMERATYPE::ENUM_END:
		break;
	default:
		break;
	}

}

_float4 CCameraMgr::GetCamLook()
{
	if (ECAMERATYPE::THIRDPERSON != m_eCurrentCamType)
		return XMFLOAT4();

	return m_pDefualtCam->GetCamLookVector();
}

_vector CCameraMgr::GetCamPos()
{
	if (!m_pDefualtCam)
		return _vector();

	return dynamic_pointer_cast<CTransform>(m_pDefualtCam->GetComponent(TEXT("Com_Transform")))->GetState(CTransform::STATE_POSITION);
}

void CCameraMgr::SetShakingMode(_float _fShakingPow, _float _fShakingTime, _bool m_bXDir)
{
	if (ECAMERATYPE::THIRDPERSON != m_eCurrentCamType)
		return;

	m_pDefualtCam->SetCameraShaking(_fShakingPow, _fShakingTime, m_bXDir);

}

void CCameraMgr::FocusPlayer(_vector _vPos, _float _fHeight)
{
	m_pDefualtCam->FocusPlayer(_vPos, _fHeight);
}

void CCameraMgr::StartPlrCamEvent(const wstring& _strEventKey)
{
	m_pDefualtCam->StartCameraEvent(_strEventKey);
}

void CCameraMgr::AddEventPreset(const wstring& _strName, vector<CEventCamera::EVENT_INFO> _info)
{
	m_pEventCam->AddPreset(_strName, _info);
}

void CCameraMgr::StartEvent(const wstring& _strName)
{
	CGameMgr::GetInstance()->GetPlayer()->SetOnMinigame(true);

	m_pEventCam->StartEvent(_strName);
	SwitchingCamera(ECAMERATYPE::EVENT);
}

void CCameraMgr::SetFreeCamPos(_vector _vPos, _vector _vLook)
{
	m_pFreeCam->SetFreeCamPos(_vPos, _vLook);

	if (m_eCurrentCamType != ECAMERATYPE::FREE) {
		SwitchingCamera(ECAMERATYPE::FREE);
	}

}
