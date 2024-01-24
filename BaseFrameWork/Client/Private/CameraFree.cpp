#include "pch.h"
#include "CameraFree.h"
#include "GameInstance.h"

CCameraFree::CCameraFree()
	:CCamera()
{
}

CCameraFree::~CCameraFree()
{
}

HRESULT CCameraFree::Initialize(_bool _IsEnable, CAMERAFREE_DESC* _pCamFreeDesc, CAMERA_DESC* _pCamDes, CTransform::TRANSFORM_DESC* _TransDesc)
{
	m_fMouseSensor = _pCamFreeDesc->fMouseSensor;

	if(FAILED(__super::Initialize(_pCamDes, _TransDesc)))
		return E_FAIL;

	m_IsEnabled = _IsEnable;

	return S_OK;
}

void CCameraFree::PriorityTick(_float fTimeDelta)
{
	if (!m_IsEnabled)
		return;


	if (m_bKeyDeb) {
		m_fKeyDebTime += fTimeDelta;

		if (m_fKeyDebTime > 0.2f) {
			m_bKeyDeb = false;
			m_fKeyDebTime = 0.f;
		}
	}

	KeyInput();

	if (GetKeyState(VK_UP) & 0x8000)
	{
		_bool jump = false;
		m_pTransformCom->GoStraight(fTimeDelta, nullptr, jump);
	}

	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		m_pTransformCom->GoBackward(fTimeDelta);
	}

	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		m_pTransformCom->GoLeft(fTimeDelta);
	}

	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		m_pTransformCom->GoRight(fTimeDelta);
	}

	if (!m_IsCamLock) {

		_long MouseMove = 0;

		if (MouseMove = CGameInstance::GetInstance()->GetDIMouseMove(DIMS_X))
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMove * m_fMouseSensor);

		if (MouseMove = CGameInstance::GetInstance()->GetDIMouseMove(DIMS_Y))
			m_pTransformCom->Turn(m_pTransformCom->GetState(CTransform::STATE_RIGHT), fTimeDelta * MouseMove * m_fMouseSensor);

	}

	if (FAILED(SetUpTransformMatices()))
		return;

}

void CCameraFree::Tick(_float fTimeDelta)
{
}

void CCameraFree::LateTick(_float fTimeDelta)
{
}

HRESULT CCameraFree::Render()
{
	return S_OK;

}

void CCameraFree::KeyInput()
{
	if (m_bKeyDeb)
		return;

	//Lock On & Off
	if (GetKeyState('L') & 0x8000)
	{
		m_IsCamLock = !m_IsCamLock;
		m_bKeyDeb = true;
	}
}

shared_ptr<CCameraFree> CCameraFree::Create(_bool _IsEnable, CAMERAFREE_DESC* _pCamFreeDesc, CAMERA_DESC* _pCamDes, CTransform::TRANSFORM_DESC* _TransDesc)
{
	shared_ptr<CCameraFree> pInstance = make_shared<CCameraFree>();

	if (FAILED(pInstance->Initialize(_IsEnable, _pCamFreeDesc, _pCamDes, _TransDesc)))
		MSG_BOX("Failed to Create : CCamFree");


	return pInstance;
}
