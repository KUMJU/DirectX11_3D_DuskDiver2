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
	virtual HRESULT Initialize(CModel::TYPE _eType, const aiMesh* pAIMesh, shared_ptr<CModel> _pModel, _fmatrix PivotMatrix);
	
	_float4x4 GetRootOffset(_uint _iNum) { return m_OffsetMatrices[_iNum]; };

public:
	void SetUpBoneMatrices(_float4x4* _pBoneMatirces, vector<shared_ptr<class CBone>>& _Bones);

public:
	HRESULT ReadyVertexBufferNonAnim(const aiMesh* _pAIMesh, _fmatrix PivotMatrix);
	HRESULT ReadyVertexBufferAnim(const aiMesh* _pAIMesh, shared_ptr<CModel> _pModel);

private:
	_char m_szName[MAX_PATH] = "";
	_uint m_iMaterialIndex = { 0 };

	_uint m_iNumBones = { 0 };
	vector<_uint> m_BoneIndices;
	vector<_float4x4> m_OffsetMatrices;

public:
	static shared_ptr<CMesh> Create(CModel::TYPE _eType, wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, const aiMesh* pAIMesh, shared_ptr<CModel> _pModel, _fmatrix PivotMatrix);


};

END