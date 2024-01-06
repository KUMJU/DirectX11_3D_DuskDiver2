#pragma once

#include "GameObject.h"


BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{

public:
	struct CAMERA_DESC
	{
		_float4		vEye, vAt;
		_float		fFovy = { 0.0f };
		_float		fNear = { 0.0f };
		_float		fFar = { 0.0f };
		_float		fAspect = { 0.0f };
	};

public:
	CCamera();
	virtual ~CCamera() = default;

public:
	virtual HRESULT Initialize(CAMERA_DESC* _pCamDes, CTransform::TRANSFORM_DESC* _TransDesc = nullptr);
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	HRESULT SetUpTransformMatices();
	CAMERA_DESC* m_pCameraInfo = {};

	_float				m_fFovy = { 0.0f };
	_float				m_fNear = { 0.0f };
	_float				m_fFar = { 0.0f };
	_float				m_fAspect = { 0.0f };

};

END