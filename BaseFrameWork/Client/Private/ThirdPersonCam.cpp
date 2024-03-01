#include "pch.h"
#include "ThirdPersonCam.h"

#include "GameInstance.h"
#include "GameMgr.h"
#include "Player.h"

CThirdPersonCam::CThirdPersonCam()
{
}

CThirdPersonCam::~CThirdPersonCam()
{
}

HRESULT CThirdPersonCam::Initialize()
{
	ShowCursor(false);

	CTransform::TRANSFORM_DESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORM_DESC));

	m_TargetPlayer = CGameMgr::GetInstance()->GetPlayer();
	m_TargetTransform = dynamic_pointer_cast<CTransform>(m_TargetPlayer->GetComponent(TEXT("Com_Transform")));

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

	m_fRadius = 2.4f;
	m_fMinRad = 2.2f;
	m_fMaxRad = 3.1f;

	m_fMinAzimuth = 0.f;
	m_fMaxAzimuth = 360.f;

	m_fMinElevation = 0.f;
	m_fMaxElevation = 70.f;

	m_fMouseSensor = 0.08f;
	m_fHeight = 0.9f;

	CreateEventInfo();

	return S_OK;
}

void CThirdPersonCam::PriorityTick(_float fTimeDelta)
{
	if (!m_IsEnabled)
		return;

	if (!m_IsInitDone) {
		_vector vPlayerPos = dynamic_pointer_cast<CTransform>(m_TargetPlayer->GetComponent(TEXT("Com_Transform")))->GetState(CTransform::STATE_POSITION);
		SphericalCoordinates(vPlayerPos);
		m_IsInitDone = true;

		m_fLastYPos = vPlayerPos.m128_f32[1];

		//카메라 position 세팅
		m_fAzimuth = XMConvertToRadians(-90.f);   //
		m_fElevation = XMConvertToRadians(38.5f);
		
		//m_fRadius = 6.f;

		_vector vCameraPos = ToCartesian() + vPlayerPos;

		XMStoreFloat4(&m_vPreCamPos, XMVectorSetW(vCameraPos, 1.f));
		m_pTransformCom->SetState(CTransform::STATE_POSITION, XMVectorSetW(vCameraPos, 1.f));
		XMStoreFloat3(&m_vCameraEye, vPlayerPos);
		m_pTransformCom->LookAt(XMVectorSetY(vPlayerPos, XMVectorGetY(vPlayerPos) + m_fHeight));
		XMStoreFloat3(&m_vCamAt, XMVectorSetY(vPlayerPos, XMVectorGetY(vPlayerPos)));

	}

	//CCamera::SetUpTransformMatices();
}

void CThirdPersonCam::Tick(_float fTimeDelta)
{
	if (!m_IsEnabled )
		return;
	//플레이어가 움직임
}

void CThirdPersonCam::LateTick(_float fTimeDelta)
{

	if (!m_IsEnabled)
		return;


	if (m_eCamState == ECAM_DEFAULT) {
		_vector vCamPos = {};
		_vector vPlayerPos = dynamic_pointer_cast<CTransform>(m_TargetPlayer->GetComponent(TEXT("Com_Transform")))->GetState(CTransform::STATE_POSITION);
		_long MouseMoveX, MouseMoveY = 0;

		if (MouseMoveX = CGameInstance::GetInstance()->GetDIMouseMove(DIMS_X))
			if (MouseMoveY = CGameInstance::GetInstance()->GetDIMouseMove(DIMS_Y))
				SphericalRotate(fTimeDelta * (_float)MouseMoveX * m_fMouseSensor, fTimeDelta * (_float)MouseMoveY * m_fMouseSensor);
		vCamPos = ToCartesian();

		XMVectorSetY(vPlayerPos, m_fLastYPos);
		vCamPos += vPlayerPos;

		//선형 보간
		XMStoreFloat3(&m_vCameraEye, vCamPos);
		vCamPos = XMVectorSetW(vCamPos, 1.f);
		//러프 수치를 0.8로 고치니까 카메라 덜컹거림이 사라짐
		// y만 별도보정 ?
		vCamPos = XMVectorLerp(XMLoadFloat4(&m_vPreCamPos), vCamPos, 0.5f);
		XMStoreFloat4(&m_vPreCamPos, vCamPos);

		CalcLookVectors(vCamPos, vPlayerPos);

		m_pTransformCom->SetState(CTransform::STATE_POSITION, vCamPos);
		m_pTransformCom->LookAt(XMVectorSetY(vPlayerPos, XMVectorGetY(vPlayerPos) + m_fHeight));
		                                          
		MouseFix();

		if (m_bShaking) {
			ShakeCamera(fTimeDelta);
		}


	}
	else if (m_eCamState == ECAM_EVENT) {

		m_fEventAccTime += fTimeDelta;

		if ((*m_CurrentCamEvent)[m_iCurrentEventIdx].bRotation) {
			RotateCamera(fTimeDelta);
		}

		if ((*m_CurrentCamEvent)[m_iCurrentEventIdx].bZoomin) {
			ZoomIn(fTimeDelta);
		}

		if ((*m_CurrentCamEvent)[m_iCurrentEventIdx].bZoomOut) {
			ZoomOut(fTimeDelta);
		}

		if ((*m_CurrentCamEvent)[m_iCurrentEventIdx].bMoving) {
			MovingCamera(fTimeDelta);
		}


		if ((*m_CurrentCamEvent)[m_iCurrentEventIdx].bFocus) {
			FocusCamera(fTimeDelta);
		}

		if (m_bShaking) {
			if (!m_bSetInitShaking) {
				m_vInitPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
				m_bSetInitShaking = true;
			}


			ShakeCamera(fTimeDelta);
		}


		if (m_fEventAccTime >= (*m_CurrentCamEvent)[m_iCurrentEventIdx].fAccTime) {
			++m_iCurrentEventIdx;
			m_fEventAccTime = 0.f;
			m_bComputeMoving = false;
			m_bSetInitShaking = false;
			m_fShakingPow = 1.f;
			m_bYShaking = false;
			

			if (m_iCurrentEventIdx == (*m_CurrentCamEvent).size()) {

				m_eCamState = ECAM_DEFAULT;
				m_fEventAccTime = 0.f;
				m_fCurrentFovy = 60.f;
				m_fFovy = XMConvertToRadians(60.f);
				m_iCurrentEventIdx = 0;
				m_CurrentCamEvent = nullptr;
					
				XMStoreFloat4(&m_vPreCamPos , m_pTransformCom->GetState(CTransform::STATE_POSITION));

			}
			else {

				if ((*m_CurrentCamEvent)[m_iCurrentEventIdx].bShaking) {

					m_bShaking = true;
					m_fTotalShakingTime = (*m_CurrentCamEvent)[m_iCurrentEventIdx].fShakingTime;
				}

			}
		}
	}
	else if (m_eCamState == ECAM_LERP) {

		m_fEventAccTime += fTimeDelta;

		if (m_fEventAccTime > m_fLerpTime) {

			if (m_bLerpStart) {
				m_fEventAccTime = m_fLerpTime;
			}
			else {
				m_fEventAccTime = m_fLerpTime;
				m_eCamState = ECAMSTATE::ECAM_DEFAULT;
				m_fEventAccTime = 0.f;
				return;
			}

		}

		_vector vLerpPos = XMVectorLerp(m_vLerpSrcPos, m_vLerpEndPos, m_fEventAccTime / m_fLerpTime);
		m_pTransformCom->SetState(CTransform::STATE_POSITION, XMVectorSetW(vLerpPos, 1.f));

		if (m_bShaking) {
			m_vInitPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
			ShakeCamera(fTimeDelta);
		}

	}
	else {

		if (m_bZoomLerp) {
			m_fZoomLerpTime += fTimeDelta;

			if (m_fZoomLerpTime >= 0.05f) {
				m_fFovy = m_fDstFovy;
				m_fZoomLerpTime = 0.f;
				m_bZoomLerp = false;
			}
			else {

				float fFovyRate = (m_fDstFovy - m_fSrcFovy) * (m_fZoomLerpTime / 0.05f);
				m_fFovy = fFovyRate + m_fSrcFovy;
			}


		}

		if (m_bShaking) {
			ShakeCamera(fTimeDelta);
		}
	}

	CCamera::SetUpTransformMatices();

}

HRESULT CThirdPersonCam::Render()
{
	return S_OK;
}

void CThirdPersonCam::ShakeCamera(_float _fTimeDelta)
{

	m_fShakingAccTime += _fTimeDelta;
	if (m_fShakingAccTime > m_fTotalShakingTime && 
		m_eCamState != ECAMSTATE::ECAM_FOCUSING)
	{
		m_fShakingAccTime = 0.f;
		m_bShaking = false;
		m_bSetInitShaking = false;
	}

	_float x = (rand() % 1000 - 500) / 10000.f;
	_float y = (rand() % 1000 - 500) / 10000.f;

	_vector vCameraShake;

	if (m_bYShaking) {
		vCameraShake = XMVector3Normalize(m_pTransformCom->GetState(CTransform::STATE_UP))* y * m_fShakingPow;
	}
	else {
		vCameraShake = XMVector3Normalize(m_pTransformCom->GetState(CTransform::STATE_UP)) * y * m_fShakingPow + XMVector3Normalize(m_pTransformCom->GetState(CTransform::STATE_RIGHT)) * x * m_fShakingPow;
	}


	if (ECAMSTATE::ECAM_DEFAULT == m_eCamState) {
		_vector vCameraPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
		m_pTransformCom->SetState(CTransform::STATE_POSITION, vCameraPos + vCameraShake);

	}
	else if(ECAMSTATE::ECAM_EVENT == m_eCamState || 
			ECAMSTATE::ECAM_LERP == m_eCamState ||
			ECAMSTATE::ECAM_FOCUSING == m_eCamState)
	{
		m_pTransformCom->SetState(CTransform::STATE_POSITION, m_vInitPos + vCameraShake);
	}

}

void CThirdPersonCam::ShakeCameraPos(_float _fTimeDelta)
{
	float fDistance = m_fShakingPow * _fTimeDelta;



}

void CThirdPersonCam::SetFOV(_float _fFov)
{
	m_eCamState = ECAM_LERP;
	m_fFovy = XMConvertToRadians(_fFov);
}

void CThirdPersonCam::FocusingPlr(_vector _vCamPos)
{
	m_pTransformCom->SetState(CTransform::STATE_POSITION, _vCamPos);
	m_pTransformCom->LookAt(dynamic_pointer_cast<CTransform>(m_TargetPlayer->GetComponent(TEXT("Com_Transform")))->GetState(CTransform::STATE_POSITION) + _vector({0.f, 0.4f, 0.f, 0.f}));

	m_eCamState = ECAMSTATE::ECAM_FOCUSING;
}

void CThirdPersonCam::SetFovLerp(_float _fRaidan)
{
	m_bZoomLerp = true;
	m_fZoomLerpTime = 0.f;

	m_fSrcFovy = m_fFovy;
	m_fDstFovy = XMConvertToRadians(_fRaidan);

}

void CThirdPersonCam::SetPositionLerpMove(_vector _vPos, _float _fAccTime)
{
	m_eCamState = ECAM_LERP;

	m_bLerpStart = true;

	m_fEventAccTime = 0.f;
	m_vLerpSrcPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
	m_vLerpEndPos = _vPos;
	m_fLerpTime = _fAccTime;

}

void CThirdPersonCam::SetLerpMoveComeBack(_float _fAccTime)
{
	m_eCamState = ECAM_LERP;

	m_bLerpStart = false;

	m_fEventAccTime = 0.f;
	m_vLerpEndPos = m_vLerpSrcPos;
	m_vLerpSrcPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
	m_fLerpTime = _fAccTime;
}


shared_ptr<CThirdPersonCam> CThirdPersonCam::Create()
{
	shared_ptr<CThirdPersonCam> pInstance = make_shared<CThirdPersonCam>();

	if (FAILED(pInstance->Initialize()))
		MSG_BOX("Create to Failed : CThirdPersonCam");

	return pInstance;
}

void CThirdPersonCam::StartCameraEvent(const wstring& _strEventKey)
{
	auto iter = m_EventInfos.find(_strEventKey);
	
	/*초기화*/
	m_fFovy = XMConvertToRadians(60.f);
	m_bShaking = false;
	m_fEventAccTime = 0.f;

	if (iter != m_EventInfos.end()) {
		m_CurrentCamEvent = &(iter->second);
		SetInitializePosition(_strEventKey);
		m_eCamState = ECAM_EVENT;
	}

}

void CThirdPersonCam::FocusPlayer(_vector _vCamPos, _float _fHeight)
{
	m_eCamState = ECAM_EVENT;
	m_pTransformCom->SetState(CTransform::STATE_POSITION, _vCamPos);
	_vector vPlrPos = m_TargetTransform->GetState(CTransform::STATE_POSITION);

	vPlrPos = XMVectorSetY(vPlrPos, vPlrPos.m128_f32[1] + _fHeight);
	m_pTransformCom->LookAt(vPlrPos);

	m_fCurrentEventTime = 4.f;

}

void CThirdPersonCam::ZoomIn(_float _fDeltaTime)
{

	m_fCurrentFovy -= (*m_CurrentCamEvent)[m_iCurrentEventIdx].fSpeed * _fDeltaTime;

	if (m_fCurrentFovy < (*m_CurrentCamEvent)[m_iCurrentEventIdx].fDistanceOffset) {
		m_fCurrentFovy = (*m_CurrentCamEvent)[m_iCurrentEventIdx].fDistanceOffset;
	}

	m_fFovy = XMConvertToRadians(m_fCurrentFovy);



}

void CThirdPersonCam::ZoomOut(_float _fDeltaTime)
{
	m_fCurrentFovy += (*m_CurrentCamEvent)[m_iCurrentEventIdx].fSpeed * _fDeltaTime;

	if (m_fCurrentFovy > (*m_CurrentCamEvent)[m_iCurrentEventIdx].fDistanceOffset) {
		m_fCurrentFovy = (*m_CurrentCamEvent)[m_iCurrentEventIdx].fDistanceOffset;
	}

	m_fFovy = XMConvertToRadians(m_fCurrentFovy);

}

void CThirdPersonCam::RotateCamera(_float _fDeltaTime)
{

}

void CThirdPersonCam::MovingCamera(_float _fDeltaTime)
{

	_vector vPlrPos = m_TargetTransform->GetState(CTransform::STATE_POSITION);
	_vector vPlrLook = m_TargetTransform->GetState(CTransform::STATE_LOOK);

	_vector vCurrentPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);


	if (!m_bComputeMoving) {
		_vector vTargetPos = vPlrPos + (vPlrLook * (*m_CurrentCamEvent)[m_iCurrentEventIdx].fDistanceOffset) + _vector({ 0.f, (*m_CurrentCamEvent)[m_iCurrentEventIdx].fYOffset, 0.f, 0.f });
		m_vDistancePerTick = (vTargetPos -vCurrentPos) / (*m_CurrentCamEvent)[m_iCurrentEventIdx].fAccTime;
		m_bComputeMoving = true;

	}

	vCurrentPos += (m_vDistancePerTick * _fDeltaTime);

	m_pTransformCom->SetState(CTransform::STATE_POSITION, vCurrentPos);
	m_pTransformCom->LookAt(vPlrPos + _vector({ 0.f, (*m_CurrentCamEvent)[m_iCurrentEventIdx].fYOffset, 0.f, 0.f }));
}

void CThirdPersonCam::FocusCamera(_float _fDeltaTime)
{

	if (!m_bComputeMoving) {
		_vector vPlrPos = m_TargetTransform->GetState(CTransform::STATE_POSITION);
		_vector vPlrLook = m_TargetTransform->GetState(CTransform::STATE_LOOK);

		_vector vCurrentPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
		_vector vTargetPos = vPlrPos + (vPlrLook * (*m_CurrentCamEvent)[m_iCurrentEventIdx].fDistanceOffset) + _vector({ 0.f, (*m_CurrentCamEvent)[m_iCurrentEventIdx].fYOffset, 0.f, 0.f });
		m_pTransformCom->SetState(CTransform::STATE_POSITION, vTargetPos);
		m_pTransformCom->LookAt(vPlrPos + _vector({ 0.f, (*m_CurrentCamEvent)[m_iCurrentEventIdx].fYOffset, 0.f, 0.f }));
		m_bComputeMoving = true;

	}


}

void CThirdPersonCam::CreateEventInfo()
{

	/*EventInit*/

	vector<tagEventDesc> BurstEvent = {};
	tagEventDesc eventDesc1 = {};
	tagEventDesc eventDesc2 = {};
	tagEventDesc eventDesc3 = {};
	tagEventDesc eventDesc4 = {};
	tagEventDesc eventDesc5 = {};


	{
		eventDesc1.bZoomin = true;
		eventDesc1.bZoomOut = false;
		eventDesc1.bRotation = false;
		eventDesc1.bMoving = false;
		eventDesc1.bFocus = false;
		eventDesc1.bShaking = false;


		eventDesc1.fAccTime = 2.f;
		eventDesc1.fSpeed = 10.f;
		eventDesc1.fDistanceOffset = 30.f;
		eventDesc1.fRadian = 0.f;
		eventDesc1.fYOffset = 1.f;



		eventDesc2.bZoomin = false;
		eventDesc2.bZoomOut = false;
		eventDesc2.bRotation = false;
		eventDesc2.bMoving = false;
		eventDesc2.bFocus = false;
		eventDesc2.bShaking = false;


		eventDesc2.fAccTime = 0.5f;
		eventDesc2.fSpeed = 0.f;
		eventDesc2.fDistanceOffset = 0.f;
		eventDesc2.fRadian = 0.f;
		eventDesc2.fYOffset = 0.f;


		eventDesc3.bZoomin = false;
		eventDesc3.bZoomOut = true;
		eventDesc3.bRotation = false;
		eventDesc3.bMoving = false;
		eventDesc3.bFocus = false;
		eventDesc3.bShaking = false;


		eventDesc3.fAccTime = 2.f;
		eventDesc3.fSpeed = 70.f;
		eventDesc3.fDistanceOffset = 70.f;
		eventDesc3.fRadian = 0.f;
		eventDesc3.fYOffset = 1.f;

		BurstEvent.push_back(eventDesc1);
		BurstEvent.push_back(eventDesc2);
		BurstEvent.push_back(eventDesc3);

	}

	m_EventInfos.emplace(TEXT("BurstTransform"), BurstEvent);

	vector<tagEventDesc> SuperSkill = {};

	{	
	eventDesc1.bZoomin = true;
	eventDesc1.bZoomOut = false;
	eventDesc1.bRotation = false;
	eventDesc1.bMoving = false;
	eventDesc1.bFocus = false;
	eventDesc1.bShaking = false;


	eventDesc1.fAccTime = 0.3f;
	eventDesc1.fSpeed = 50.f;
	eventDesc1.fDistanceOffset = 30.f;
	eventDesc1.fRadian = 0.f;
	eventDesc1.fYOffset = 0.f;

	//////////////////////////////////////////////////
	eventDesc4.bZoomin = false;
	eventDesc4.bZoomOut = false;
	eventDesc4.bRotation = false;
	eventDesc4.bMoving = false;
	eventDesc4.bFocus = false;
	eventDesc4.bShaking = false;

			 
	eventDesc4.fAccTime = 0.9f;
	eventDesc4.fSpeed = 0.f;
	eventDesc4.fDistanceOffset = 0.f;
	eventDesc4.fRadian = 0.f;
	eventDesc4.fYOffset = 0.f;

	///////////////////////////////////////////////
	eventDesc2.bZoomin = false;
	eventDesc2.bZoomOut = false;
	eventDesc2.bRotation = false;
	eventDesc2.bMoving = true;
	eventDesc2.bFocus = false;
	eventDesc2.bShaking = false;


	eventDesc2.fDistanceOffset = 2.4f;
	eventDesc2.fAccTime = 0.1f;
	eventDesc2.fYOffset = 1.1f;

	eventDesc3.bZoomin = false;
	eventDesc3.bZoomOut = false;
	eventDesc3.bRotation = false;
	eventDesc3.bMoving = false;
	eventDesc3.bFocus = false;
	eventDesc3.bShaking = false;

	eventDesc3.fAccTime = 1.f;

	eventDesc5.bZoomin = false;
	eventDesc5.bZoomOut = false;
	eventDesc5.bRotation = false;
	eventDesc5.bMoving = false;
	eventDesc5.bFocus = true;
	eventDesc5.bShaking = true;
			 
	eventDesc5.fAccTime = 2.f;
	eventDesc5.fDistanceOffset = -2.3f;
	eventDesc5.fYOffset = 1.1f;
	eventDesc5.fShakingTime = 1.5f;

	SuperSkill.push_back(eventDesc1);
	SuperSkill.push_back(eventDesc4);
	SuperSkill.push_back(eventDesc2);
	SuperSkill.push_back(eventDesc3);
	SuperSkill.push_back(eventDesc5);


	
	}


	m_EventInfos.emplace(TEXT("SuperSkill1"), SuperSkill);



	/*Final Attack */

	vector<tagEventDesc> FinalAttack = {};


	{
		eventDesc1.bZoomin = false;
		eventDesc1.bZoomOut = false;
		eventDesc1.bRotation = false;
		eventDesc1.bMoving = false;
		eventDesc1.bFocus = true;
		eventDesc1.bShaking = false;

		eventDesc1.fAccTime = 0.1f;
		eventDesc1.fDistanceOffset = 1.7f;
		eventDesc1.fYOffset = 1.f;

		eventDesc5.bZoomin = false;
		eventDesc5.bZoomOut = false;
		eventDesc5.bRotation = false;
		eventDesc5.bMoving = false;
		eventDesc5.bFocus = true;
		eventDesc5.bShaking = false;

		eventDesc5.fAccTime = 1.7f;
		eventDesc5.fDistanceOffset = 3.7f;
		eventDesc5.fYOffset = 1.f;

		eventDesc2.bZoomin = true;
		eventDesc2.bZoomOut = false;
		eventDesc2.bRotation = false;
		eventDesc2.bMoving = false;
		eventDesc2.bFocus = false;
		eventDesc2.bShaking = false;

		eventDesc2.fAccTime = 1.f;
		eventDesc2.fSpeed = 50.f;
		eventDesc2.fDistanceOffset = 30.f;
		eventDesc2.fRadian = 0.f;
		eventDesc2.fYOffset = 0.f;

		eventDesc3.bZoomin = false;
		eventDesc3.bZoomOut = false;
		eventDesc3.bRotation = false;
		eventDesc3.bMoving = false;
		eventDesc3.bFocus = false;
		eventDesc3.bShaking = false;
				 
		eventDesc3.fAccTime = 1.f;

		eventDesc4.bZoomin = false;
		eventDesc4.bZoomOut = false;
		eventDesc4.bRotation = false;
		eventDesc4.bMoving = false;
		eventDesc4.bFocus = true;
		eventDesc4.bShaking = false;
				 
		eventDesc4.fAccTime = 7.f;
		eventDesc4.fDistanceOffset = -4.7f;
		eventDesc4.fYOffset = 1.3f;



		FinalAttack.push_back(eventDesc1);
		FinalAttack.push_back(eventDesc2);
		FinalAttack.push_back(eventDesc3);
		FinalAttack.push_back(eventDesc4);
	//	FinalAttack.push_back(eventDesc5);

	}

	m_EventInfos.emplace(TEXT("FinalAttack"), FinalAttack);


}

void CThirdPersonCam::SetInitializePosition(const wstring& _strEventKey)
{

	if (TEXT("BurstTransform") == _strEventKey) {
		/*Initialize Camera Position*/
		_vector vPlrPos = m_TargetTransform->GetState(CTransform::STATE_POSITION);
		_vector vPlrLook = m_TargetTransform->GetState(CTransform::STATE_LOOK);

		_vector vCamPos = vPlrPos + (vPlrLook * 1.6f) + _vector{ 0.f, 1.3f, 0.f, 0.f };
		m_pTransformCom->SetState(CTransform::STATE_POSITION, vCamPos);
		_vector vLookPlrPos = vPlrPos + _vector{ 0.f, 0.75f, 0.f, 0.f };
		m_pTransformCom->LookAt(vLookPlrPos);

		m_fEventAccTime = 0.f;

	}
	else if (TEXT("SuperSkill1") == _strEventKey) {

		_vector vPlrPos = m_TargetTransform->GetState(CTransform::STATE_POSITION);
		_vector vPlrLook = m_TargetTransform->GetState(CTransform::STATE_LOOK);
		_vector vPlrRight = m_TargetTransform->GetState(CTransform::STATE_RIGHT);
		

		_vector vCamPos = vPlrPos + _vector{ 0.f, 0.8f, 0.f, 0.f } + ((vPlrRight + vPlrLook) * 1.6f);
		m_pTransformCom->SetState(CTransform::STATE_POSITION, vCamPos);
		_vector vLookPlrPos = vPlrPos + _vector{ 0.f, 0.7f, 0.f, 0.f };

		m_pTransformCom->LookAt(vLookPlrPos);

		_vector vLookVec = m_pTransformCom->GetState(CTransform::STATE_LOOK);
		_vector vUpVec = m_pTransformCom->GetState(CTransform::STATE_UP);

		//_float fZRadian = 5.f;
		//_float fYRadian = -50.f;

		//m_pTransformCom->Rotation({0.f, 0.f, 1.f, 0.f}, XMConvertToRadians(fZRadian));
		//m_pTransformCom->Rotation({ 0.f, 1.f, 0.f, 0.f }, XMConvertToRadians(fYRadian));

		m_fEventAccTime = 0.f;
	}


}



void CThirdPersonCam::SwitchingCamMode(ECAMSTATE _eCamState)
{
	/*초기화*/
	m_fFovy = XMConvertToRadians(60.f);
	m_bShaking = false;
	m_fEventAccTime = 0.f;

	m_eCamState = _eCamState;
}

void CThirdPersonCam::SetCameraShaking(_float _fShakingPow, _float _fShakingTime, _bool m_bXDir)
{

	m_bShaking = true;

	m_bYShaking = m_bXDir;
	m_fTotalShakingTime = _fShakingTime;
	m_fShakingPow = _fShakingPow;

	if (m_eCamState == ECAM_FOCUSING) {
		m_vInitPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
	}

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

void CThirdPersonCam::StaticRotate(_float _fAzimuth, _float _fElevation)
{
	m_fAzimuth = _fAzimuth;
	m_fElevation = _fElevation;

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

void CThirdPersonCam::CalcLookVectors(_fvector _vCamPos, _fvector _vPlrPos)
{
	_vector vLook = _vPlrPos - _vCamPos;
	vLook = XMVector4Normalize(XMVectorSetY(XMVectorSetW(vLook, 0.f),0.f));
	XMStoreFloat4(&m_vLookPlr, vLook);
}

void CThirdPersonCam::LockOn()
{
}

void CThirdPersonCam::CameraEffect()
{
}

void CThirdPersonCam::MouseFix()
{
	POINT	ptMouse{ Client::g_iWinSizeX >> 1, Client::g_iWinSizeY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}
