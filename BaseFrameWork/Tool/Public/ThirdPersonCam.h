#pragma once

#include "Camera.h"
#include "Client_Defines.h"

BEGIN(Client)


//������ǥ�踦 ����� 3��Ī ī�޶�
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

	//�ʱ� �÷��̾� ��ǥ���� �̿��� �ʿ��� ���� ����صδ� �Լ�
	void SphericalCoordinates(_fvector _vTargetPos);
	//������ǥ�踦 �̿��� ȸ��
	void SphericalRotate(_float _fAzimuth , _float _fElevation);
	//��ǥ�躯ȯ
	_vector ToCartesian();

private:

	_float m_fRadius = 0.f;
	_float m_fAzimuth = 0.f; //������x
	_float m_fElevation = 0.f; //�Ӱ�y

	//////////////////////////////��������/////////////////////////
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

	//�ﰢ�� ����� ���� T
	_float m_fT = 0.f;

	//���콺 �ΰ���
	_float m_fMouseSensor = 0.f;
	//�÷��̾� �������� ��ŭ �� ���� ��������
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