#pragma once

#include "VIBuffer.h"
#include "Model.h"

BEGIN(Engine)

class CModelMesh final : public CVIBuffer
{
public:
	CModelMesh(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);
	virtual ~CModelMesh();

public:
	_uint GetMaterialIndex() const {
		return m_iMaterialIndex;
	}

public:
	virtual HRESULT Initialize(CModel::TYPE _eType, HANDLE _handle, _fmatrix PivotMatrix);

	_float4x4 GetRootOffset(_uint _iNum) { return m_OffsetMatrices[_iNum]; };

public:
	void SetUpBoneMatrices(_float4x4* _pBoneMatirces, vector<shared_ptr<class CBone>>& _Bones);

public:
	HRESULT ReadyVertexBufferNonAnim(HANDLE _handle, _fmatrix PivotMatrix);
	HRESULT ReadyVertexBufferAnim(HANDLE _handle);

private:
	_char m_szName[MAX_PATH] = "";
	_uint m_iMaterialIndex = { 0 };

	_uint m_iNumBones = { 0 };
	vector<_uint> m_BoneIndices;
	vector<_float4x4> m_OffsetMatrices;

public:
	static shared_ptr<CModelMesh> Create(CModel::TYPE _eType, wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, HANDLE _handle, _fmatrix PivotMatrix);


};

END