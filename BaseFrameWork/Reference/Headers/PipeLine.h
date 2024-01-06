#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class CPipeLine final
{
public:
	enum D3DTRANSFORMSTATE { D3DTS_VIEW, D3DTS_PROJ, D3DTS_END };

public:
	CPipeLine();
	~CPipeLine() = default;


public:
	void SetTransform(D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix) {
		XMStoreFloat4x4(&m_TransformMatrix[eState], TransformMatrix);
	}

	_float4x4 GetTransformFloat4x4(D3DTRANSFORMSTATE eState) {
		return m_TransformMatrix[eState];
	}

	_matrix GetTransformMatrix(D3DTRANSFORMSTATE eState) {
		return XMLoadFloat4x4(&m_TransformMatrix[eState]);
	}

	_float4x4 GetTransformFloat4x4Inverse(D3DTRANSFORMSTATE eState) {
		return m_TransformMatrix_Inverse[eState];
	}

	_matrix GetTransformMatrixInverse(D3DTRANSFORMSTATE eState) {
		return XMLoadFloat4x4(&m_TransformMatrix_Inverse[eState]);
	}

	_float4 GetCamPosition() {
		return m_vCamPosition;
	}

public:
	void Tick();

private:
	_float4x4			m_TransformMatrix[D3DTS_END] = {};
	_float4x4			m_TransformMatrix_Inverse[D3DTS_END] = {};
	_float4				m_vCamPosition = {};

public:

	static shared_ptr<CPipeLine> Create();

};

END