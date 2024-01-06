#pragma once

#include "Engine_Defines.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CModel : public CComponent
{
public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };

public:
	CModel(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);
	virtual ~CModel() = default;

public:
	HRESULT Initialize(TYPE eModelType, const _char* pModelFilePath, _fmatrix PivotMatrix);
	HRESULT Render(_uint iMeshIndex);

public:
	_uint GetNumMeshes() const {
		return m_iNumMeshes;
	}

public:
	HRESULT BindMaterialShaderResource(shared_ptr<class CShader> _pShader, _uint _iMeshIndex, aiTextureType _eMaterialType, const _char* _pConstantName);

private:
	const aiScene* m_pAIScene = {};
	Assimp::Importer m_Importer;

private:
	_float4x4 m_PivotMatrix;

private:
	_uint m_iNumMeshes = { 0 }; 
	vector<shared_ptr<class CMesh>> m_Meshes;

	_uint m_iNumMaterials = { 0 };
	vector<shared_ptr<class CMaterial>> m_Materials;

private:
	HRESULT ReadyMeshes();
	HRESULT ReadyMaterials(const _char* _pModelFilePath);

public:
	static shared_ptr<CModel> Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, TYPE eModelType, const _char* pModelFilePath, _fmatrix PivotMatrix = XMMatrixIdentity());

};

END