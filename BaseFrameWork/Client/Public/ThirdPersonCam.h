#pragma once

#include "Camera.h"
#include "Client_Defines.h"

BEGIN(Client)


//구면좌표계를 사용한 3인칭 카메라
class CThirdPersonCam final : public CCamera
{
public:
	enum ECAMSTATE {
		ECAM_DEFAULT,
		ECAM_EVENT,
		ECAM_LERP,
		ECAM_END
	};


public:
	enum ESHAKINGTYPE {
		ST_FOV,
		ST_POS,
		ST_END
	};

public:
	enum EVENTTYPE {
		TYPE_FOCUS,
		TYPE_ZOOMIN,
		TYPE_ZOOMOUT,
		TYPE_ROTATION,
		TYPE_END
	};

	struct tagEventDesc {
		_bool bZoomin;
		_bool bZoomOut;
		_bool bRotation;
		_bool bMoving;
		_bool bFocus;
		_bool bShaking;

		_float fSpeed;
		_float fRadian;
		_float fYOffset;
		_float fDistanceOffset;
		_float fAccTime;
		_float3 vEndPosition;
		_float4 vAxis = {0.f, 0.f ,0.f , 0.f};
		_float fShakingTime;
	};

public:
	CThirdPersonCam();
	virtual ~CThirdPersonCam();

private:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	_float4 GetCamLookVector() { return m_vLookPlr; }


public:
	void SetCameraShaking(_float _fShakingPow, _float _fShakingTime, _bool m_bXDir);
	void ShakeCamera(_float _fTimeDelta); // RandomShakeCamera
	void ShakeCameraPos(_float _fTimeDelta);

private:
	_bool m_bShaking = false;

	_bool m_bYShaking = false;
	_float m_fShakingAccTime = 0.f;
	_float m_fTotalShakingTime = 0.f;
	_float m_fShakingPow = 0.f;

	_bool m_bSetInitShaking = false;
	_vector m_vInitPos = _vector();

	_float m_fYOffset = 0.f;
	_float m_fXOffset = 0.f;
	_int m_iShakingCount = 0;
private:

	//초기 플레이어 좌표값을 이용해 필요한 값을 계산해두는 함수
	void SphericalCoordinates(_fvector _vTargetPos);
	//구면좌표계를 이용한 회전
	void SphericalRotate(_float _fAzimuth , _float _fElevation);
	//좌표계변환
	_vector ToCartesian();

private:
	//구면좌표계를 이용해 회전하지만 기존 값에 더해주는 구조가 아닌 지정된 각도로 돌림 
	void StaticRotate(_float _fAzimuth, _float _fElevation);


private:
	ECAMSTATE m_eCamState = ECAMSTATE::ECAM_DEFAULT;


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

	_float m_fLastYPos = 0.f;

private:
	_float m_fEventAccTime = 0.f;
	_float m_fCurrentEventTime = 0.f;

	_float m_fOriginFovy = 60.f;
	_float m_fCurrentFovy = 60.f;

	_float m_fZoomSpeed = 15.f;

	_int m_iCurrentEventIdx = 0;

	_bool m_bComputeMoving = false;
	_vector m_vDistancePerTick = _vector();

private:
	_float3 m_vCameraAt;
	_float3 m_vCameraEye;

//for Camera Lerp
private:
	_float4 m_vPreCamPos = {};

//for Player
private:
	_float4 m_vLookPlr = {};

	void CalcLookVectors(_fvector _vCamPos, _fvector _vPlrPos);

private:
	_bool m_IsInitDone = false;

private:
	void LockOn();
	void CameraEffect();

private:
	void MouseFix();

/***************************/
//		Camera Event       //
/***************************/

public:
	void StartCameraEvent(const wstring& _strEventKey);
	void FocusPlayer(_vector _vCamPos, _float _fHeight);

	void ZoomIn(_float _fDeltaTime);
	void ZoomOut(_float _fDeltaTime);
	void RotateCamera(_float _fDeltaTime);
	void MovingCamera(_float _fDeltaTime);
	void FocusCamera(_float _fDeltaTime);

private:
	void CreateEventInfo();
	void SetInitializePosition(const wstring& _strEventKey);

private:
	map<wstring, vector<tagEventDesc>> m_EventInfos;
	vector<tagEventDesc>* m_CurrentCamEvent;


private:
	shared_ptr<class CPlayer> m_TargetPlayer = nullptr;
	shared_ptr<CGameObject> m_pLockOnTarget = nullptr;
	shared_ptr<CTransform> m_TargetTransform = nullptr;


public:
	static shared_ptr<CThirdPersonCam> Create();

};

END