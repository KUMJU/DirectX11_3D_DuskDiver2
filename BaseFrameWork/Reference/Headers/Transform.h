#pragma once

#include "Component.h"


BEGIN(Engine)

class ENGINE_DLL CTransform final: public CComponent
{
public:

	enum STATE {STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

	struct TRANSFORM_DESC {

		_float fSpeedPerSet = { 0.f };
		_float fRotationPerSet = { 0.f };

	};

public:
	CTransform(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);
	virtual ~CTransform();

public:
	_vector GetState(STATE _eState) {
		return XMLoadFloat4x4(&m_WorldMatrix).r[_eState];
	}

	_float3 GetScaled() {
		return _float3(
			XMVector3Length(GetState(STATE_RIGHT)).m128_f32[0],
			XMVector3Length(GetState(STATE_UP)).m128_f32[0],
			XMVector3Length(GetState(STATE_LOOK)).m128_f32[0]
		);
	}

	_float* GetWorldMatFloatArr() {
		return m_WorldMatrix.m[0];
	}

	_matrix GetWorldMatrix() {
		return XMLoadFloat4x4(&m_WorldMatrix);
	}

	_matrix GetWorldMatrixInverse() {
		return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix));
	}

	void SetScaling(_float _fX, _float _fY, _float _fZ);

	void SetState(STATE _eState, _fvector _vState) {
		_float4 vTmp;
		XMStoreFloat4(&vTmp, _vState);
		memcpy(&m_WorldMatrix.m[_eState], &vTmp, sizeof(_float4));
	}

	void SetWorldMatrix(_float4x4 _worldMat) {
		m_WorldMatrix = _worldMat;
	}

	void SetSpeed(_float _fSpeed) {
		m_fSpeedPerSec = _fSpeed;
	}

	//모든 이동에 가능여부 확인
	void CheckingMove(_fvector _vPosition, shared_ptr<class CNavigation> _pNavigation , _bool& _IsJump);
	void SetYOffset(_float _fOffset) { m_fYOffset = _fOffset; }

public:

	//이동 + 이동 제한 확인
	void GoStraight(_float _fTimeDelta, shared_ptr<class CNavigation> _pNavigation, _bool& _bJump);
	void GoBackward(_float _fTimeDelta);
	void GoLeft(_float _fTimeDelta);
	void GoRight(_float _fTimeDelta, shared_ptr<CNavigation> _pNavigation, _bool _bJump);
	void Turn(_fvector _vAxis, _float _fTimeDelta);

	void TurnPlayer(_vector _vAxis, _float _fAngle, _float _fTimeDelta);
		
	void Rotation(_fvector _vAxis, _float _fRadian);
	void MoveTo(_fvector _vPoint, _float _fLimit, _float _fTimeDelta);
	void LookAt(_fvector _vPoint);
	void LookAtForLandObject(_fvector _vPoint);

public:
	virtual HRESULT Initialize(TRANSFORM_DESC* _pDesc);

public:
	HRESULT BindShaderResource(shared_ptr<class CShader> _pShader, const _char* _pContantName);

private:
	_float4x4 m_WorldMatrix = {};
	_float m_fSpeedPerSec = { 0.f };
	_float m_fRotationPerSet = { 0.f };

	_float m_fYOffset = { 0.f };

public:

	static shared_ptr<CTransform> Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, TRANSFORM_DESC * _pDesc = nullptr);
};

END