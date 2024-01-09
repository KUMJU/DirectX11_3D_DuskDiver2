#pragma once

#include "Camera.h"
#include "Client_Defines.h"

BEGIN(Client)


//구면좌표계를 사용한 3인칭 카메라
class CThirdPersonCam final : public CCamera
{
public:
	CThirdPersonCam();
	virtual ~CThirdPersonCam();

private:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:

	//초기 플레이어 좌표값을 이용해 필요한 값을 계산해두는 함수
	void SphericalCoordinates(_fvector _vTargetPos);
	//구면좌표계를 이용한 회전
	void SphericalRotate(_float _fAzimuth , _float _fElevation);
	//좌표계변환
	_vector ToCartesian();

private:

	_float m_fRadius = 0.f;
	_float m_fAzimuth = 0.f; //방위각x
	_float m_fElevation = 0.f; //앙각y

	//////////////////////////////범위제한/////////////////////////
	_float m_fMinRad = 0.f;
	_float m_fMaxRad = 0.f;

	_float m_fMinAzimuth = 0.f;
	_float m_fMaxAzimuth = 0.f;
	_float m_fMinAzimuthRad = 0.f;
	_float m_fMaxAzimuthRad = 0.f;

	_float m_fMinElevation = 0.f;
	_float m_fMaxElevation = 0.f;
	_float m_fMinElevationRad = 0.f;
	_float m_fMaxElevationRad = 0.f;
	//////////////////////////////////////////////////////////////

	//삼각비 사용을 위한 T
	_float m_fT = 0.f;

	//마우스 민감도
	_float m_fMouseSensor = 0.f;
	//플레이어 기준으로 얼만큼 더 위에 있을건지
	_float m_fHeight = 0.f;

private:
	_float3 m_vCameraAt;
	_float3 m_vCameraEye;

//for Camera Lerp
private:
	_float4 m_vPreCamPos = {};


private:
	_bool m_IsInitDone = false;

private:
	void LockOn();
	void CameraEffect();

private:
	void MouseFix();


private:
	shared_ptr<class CPlayer> m_TargetPlayer = nullptr;
	shared_ptr<CGameObject> m_pLockOnTarget = nullptr;


public:
	static shared_ptr<CThirdPersonCam> Create();

};

END