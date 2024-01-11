#include "pch.h"
#include "CameraMgr.h"

#include "ThirdPersonCam.h"
#include "CameraFree.h"

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
