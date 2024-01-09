#pragma once

#include "VIBuffer.h"
#include "Model.h"

BEGIN(Engine)

class CMesh final : public CVIBuffer
{
public:
	CMesh(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);
	virtual ~CMesh() = default;

public:
	_uint GetMaterialIndex() const {
		return m_iMaterialIndex;
	}

public:
	virtual HRESULT Initialize(CModel::TYPE _eType, const aiMesh* pAIMesh, _fmatrix PivotMatrix);
	
public:
	HRESULT ReadyVertexBufferNonAnim(const aiMesh* _pAIMesh, _fmatrix PivotMatrix);
	HRESULT ReadyVertexBufferAnim(const aiMesh* _pAIMesh);

private:
	_uint m_iMaterialIndex = { 0 };

	_uint m_iNumBones = { 0 };
	vector<_uint> m_BoneIndices;

public:
	static shared_ptr<CMesh> Create(CModel::TYPE _eType, wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, const aiMesh* pAIMesh, _fmatrix PivotMatrix);


};

END