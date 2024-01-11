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
	CModel(const CModel& _rhs);
	virtual ~CModel() = default;

public:
	HRESULT Initialize(TYPE eModelType, const _char* pModelFilePath, _fmatrix PivotMatrix);
	HRESULT InitializeClone();
	HRESULT Render(_uint iMeshIndex);

public:
	TYPE GetModelType() { return m_eModelType; }

public:
	_uint GetNumMeshes() const {
		return m_iNumMeshes;
	}


	_int GetBoneIndex(const _char* _pBoneName) const;

public:
	HRESULT BindMaterialShaderResource(shared_ptr<class CShader> _pShader, _uint _iMeshIndex, aiTextureType _eMaterialType, const _char* _pConstantName);
	HRESULT BindBoneMatrices(shared_ptr<class CShader> _pShader, const _char* _pConstName, _uint _iMeshIndex);
	_bool PlayAnimation(_float _fTimeDelta, _bool _isLoop, _float3* _vRootPos);

public:
	//초기 세팅용 , 근데 이건 init에서 받아도 될거 같기도? 일단 냅두기
	void SetAnimNum(_uint _iAnimNum) { _iAnimNum = _iAnimNum; }
	void ChangeAnimation(_uint _iAnimNum);


private:
	const aiScene* m_pAIScene = {};
	Assimp::Importer m_Importer;

private:
	_float4x4 m_PivotMatrix;
	CModel::TYPE m_eModelType = CModel::TYPE::TYPE_END;

private:
	_uint m_iNumMeshes = { 0 }; 
	vector<shared_ptr<class CMesh>> m_Meshes;

	_uint m_iNumMaterials = { 0 };
	vector<shared_ptr<class CMaterial>> m_Materials;

	vector<shared_ptr<class CBone>> m_Bones;

	_uint m_iNumAnimations = { 0 };
	_uint m_iCurrentAnimation = { 0 };
	vector<shared_ptr<class CAnimation>> m_Animations;

//For Anim Linear
private:

	_bool m_IsLinearState = false;
	_float m_fLinearTime = 0.f;
	_float m_fLinearTotalTime = 0.08f;

	shared_ptr<class CAnimation> m_CurrentAnim = nullptr;
	shared_ptr<class CAnimation> m_NextAnim = nullptr;

	_uint m_iNextAnimation = { 0 };
	_uint m_RootBoneIdx = { 0 };

private:
	HRESULT ReadyMeshes();
	HRESULT ReadyMaterials(const _char* _pModelFilePath);
	HRESULT ReadyBones(aiNode* _pNode, _int _iParentBoneIndex);
	HRESULT ReadyAnimations();

public:
	static shared_ptr<CModel> Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, TYPE eModelType, const _char* pModelFilePath, _fmatrix PivotMatrix = XMMatrixIdentity());
	static shared_ptr<CModel> Clone(shared_ptr<CModel> _rhs);

};

END