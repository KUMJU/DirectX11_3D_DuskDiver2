#include "pch.h"
#include "ThirdPersonCam.h"

#include "GameInstance.h"
#include "GameMgr.h"

CThirdPersonCam::CThirdPersonCam()
{
}

CThirdPersonCam::~CThirdPersonCam()
{
}

HRESULT CThirdPersonCam::Initialize()
{
	CTransform::TRANSFORM_DESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORM_DESC));

	m_TargetPlayer = CGameMgr::GetInstance()->GetPlayer();
	_vector vPlayerPos = dynamic_pointer_cast<CTransform>(m_TargetPlayer->GetComponent(TEXT("Com_Transform")))->GetState(CTransform::STATE_POSITION);
	_vector vCamPos = { vPlayerPos.m128_f32[0] , vPlayerPos.m128_f32[1] , vPlayerPos.m128_f32[2] - 5.f, 1.f };

	TransformDesc.fSpeedPerSet = 10.0f;
	TransformDesc.fRotationPerSet = XMConvertToRadians(90.0f);

	_float4 fEye, fAt;

	XMStoreFloat4(&fEye, vPlayerPos);
	XMStoreFloat4(&fAt, vCamPos);

	CAMERA_DESC pCamDesc = {};
	pCamDesc.vEye = fEye;
	pCamDesc.vAt = fAt;
	pCamDesc.fFovy = XMConvertToRadians(60.0f);
	pCamDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	pCamDesc.fNear = 0.2f;
	pCamDesc.fFar = 1000.f;

	if(FAILED(__super::Initialize(&pCamDesc, &TransformDesc)))
		return E_FAIL;

	m_fRadius = 3.f;
	m_fMinRad = 2.f;
	m_fMaxRad = 3.5f;
	m_fMinAzimuth = 0.f;
	m_fMaxAzimuth = 360.f;

	m_fMinElevation = 0.f;
	m_fMaxElevation = 50.f;

	m_fMouseSensor = 0.4f;
	m_fHeight = 0.3f;

	return S_OK;
}

void CThirdPersonCam::PriorityTick(_float fTimeDelta)
{

	if (!m_IsInitDone) {
		_vector vPlayerPos = dynamic_pointer_cast<CTransform>(m_TargetPlayer->GetComponent(TEXT("Com_Transform")))->GetState(CTransform::STATE_POSITION);
		SphericalCoordinates(vPlayerPos);
		m_IsInitDone = true;

		//카메라 position 세팅
		m_fAzimuth = XMConvertToRadians(-90.f);   //
		m_fElevation = XMConvertToRadians(38.5f);
		
		m_fRadius = 6.f;

		_vector vCameraPos = ToCartesian() + vPlayerPos;

		m_pTransformCom->SetState(CTransform::STATE_POSITION, XMVectorSetW(vCameraPos, 1.f));
		XMStoreFloat3(&m_vCameraEye, vPlayerPos);
		m_pTransformCom->LookAt(XMVectorSetY(vPlayerPos, XMVectorGetY(vPlayerPos) + m_fHeight));
		XMStoreFloat3(&m_vCameraAt, XMVectorSetY(vPlayerPos, XMVectorGetY(vPlayerPos) + m_fHeight));
	}

	//CCamera::SetUpTransformMatices();
}

void CThirdPersonCam::Tick(_float fTimeDelta)
{
	//플레이어가 움직임
}

void CThirdPersonCam::LateTick(_float fTimeDelta)
{
	//Lerp 넣어서 부드러운 카메라 만들기

	_vector vCamPos = {};
	_vector vPlayerPos = dynamic_pointer_cast<CTransform>(m_TargetPlayer->GetComponent(TEXT("Com_Transform")))->GetState(CTransform::STATE_POSITION);
	_long MouseMoveX, MouseMoveY = 0;

	if (MouseMoveX = CGameInstance::GetInstance()->GetDIMouseMove(DIMS_X))
		if (MouseMoveY = CGameInstance::GetInstance()->GetDIMouseMove(DIMS_Y))

	SphericalRotate(fTimeDelta * (_float)MouseMoveX * m_fMouseSensor, fTimeDelta * (_float)MouseMoveY * m_fMouseSensor);
	vCamPos = ToCartesian();
	vCamPos += vPlayerPos;

	XMStoreFloat3(&m_vCameraEye, vCamPos);
	m_pTransformCom->SetState(CTransform::STATE_POSITION, XMVectorSetW(vCamPos, 1.f));
	m_pTransformCom->LookAt(XMVectorSetY(vPlayerPos, XMVectorGetY(vPlayerPos) + m_fHeight));
	XMStoreFloat3(&m_vCameraAt, XMVectorSetY(vPlayerPos, XMVectorGetY(vPlayerPos) + m_fHeight));

	CCamera::SetUpTransformMatices();


}

HRESULT CThirdPersonCam::Render()
{
	return S_OK;
}


shared_ptr<CThirdPersonCam> CThirdPersonCam::Create()
{
	shared_ptr<CThirdPersonCam> pInstance = make_shared<CThirdPersonCam>();

	if (FAILED(pInstance->Initialize()))
		MSG_BOX("Create to Failed : CThirdPersonCam");

	return pInstance;
}

void CThirdPersonCam::SphericalCoordinates(_fvector _vTargetPos)
{

	m_fMinAzimuthRad = XMConvertToRadians(m_fMinAzimuth);
	m_fMaxAzimuthRad = XMConvertToRadians(m_fMaxAzimuth);

	m_fMinElevationRad = XMConvertToRadians(m_fMinElevation);
	m_fMaxElevationRad = XMConvertToRadians(m_fMaxElevation);

	_vector vCamPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);

	m_fAzimuth = atan2f(XMVectorGetZ(vCamPos), XMVectorGetX(vCamPos));
	m_fElevation = asinf(XMVectorGetY(vCamPos) / m_fRadius);

	m_fT = m_fRadius * cosf(m_fElevation);
	_vector vNewCamPos = { m_fT * cosf(m_fAzimuth) , m_fRadius * sinf(m_fElevation) , m_fT * sinf(m_fAzimuth) };
	vNewCamPos += _vTargetPos;

	m_pTransformCom->SetState(CTransform::STATE_POSITION, vNewCamPos);

}

void CThirdPersonCam::SphericalRotate(_float _fAzimuth, _float _fElevation)
{
	m_fAzimuth += _fAzimuth;
	m_fElevation += _fElevation;

	//MinMax 범위 제한을 위한 부분

	// Same Unity Repeat
	m_fAzimuth = m_fAzimuth - ((_uint)(m_fAzimuth / 360.f)) * 360.f;

	//Clamp
	if (m_fElevation >= m_fMaxElevationRad)
	{
		m_fElevation = m_fMaxElevationRad;
	}
	else if (m_fElevation <= m_fMinElevationRad)
	{
		m_fElevation = m_fMinElevationRad;
	}

}

_vector CThirdPersonCam::ToCartesian()
{
	_float t = m_fRadius * cosf(m_fElevation);
	_vector vNewCamPos = { m_fT * cosf(m_fAzimuth) , m_fRadius * sinf(m_fElevation)  , m_fT * sinf(m_fAzimuth) };
	return vNewCamPos;
}


void CThirdPersonCam::LockOn()
{
}

void CThirdPersonCam::CameraEffect()
{
}
