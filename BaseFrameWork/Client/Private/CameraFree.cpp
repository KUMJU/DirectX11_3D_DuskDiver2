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

HRESULT CCameraFree::Initialize(CAMERAFREE_DESC* _pCamFreeDesc, CAMERA_DESC* _pCamDes, CTransform::TRANSFORM_DESC* _TransDesc)
{
	m_fMouseSensor = _pCamFreeDesc->fMouseSensor;

	if(FAILED(__super::Initialize(_pCamDes, _TransDesc)))
		return E_FAIL;

	return S_OK;
}

void CCameraFree::PriorityTick(_float fTimeDelta)
{

	if (GetKeyState('W') & 0x8000)
	{
		m_pTransformCom->GoStraight(fTimeDelta);
	}

	if (GetKeyState('S') & 0x8000)
	{
		m_pTransformCom->GoBackward(fTimeDelta);
	}

	if (GetKeyState('A') & 0x8000)
	{
		m_pTransformCom->GoLeft(fTimeDelta);
	}

	if (GetKeyState('D') & 0x8000)
	{
		m_pTransformCom->GoRight(fTimeDelta);
	}

	_long MouseMove = 0;

	if(MouseMove = CGameInstance::GetInstance()->GetDIMouseMove(DIMS_X))
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMove * m_fMouseSensor);

	if (MouseMove = CGameInstance::GetInstance()->GetDIMouseMove(DIMS_Y))
		m_pTransformCom->Turn(m_pTransformCom->GetState(CTransform::STATE_RIGHT), fTimeDelta * MouseMove * m_fMouseSensor);

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

shared_ptr<CCameraFree> CCameraFree::Create(CAMERAFREE_DESC* _pCamFreeDesc, CAMERA_DESC* _pCamDes, CTransform::TRANSFORM_DESC* _TransDesc)
{
	shared_ptr<CCameraFree> pInstance = make_shared<CCameraFree>();

	if (FAILED(pInstance->Initialize(_pCamFreeDesc, _pCamDes, _TransDesc)))
		MSG_BOX("Failed to Create : CCamFree");

	return pInstance;
}
