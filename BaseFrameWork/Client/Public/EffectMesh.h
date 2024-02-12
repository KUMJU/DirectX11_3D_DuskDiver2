#pragma once

#include "Effect.h"


BEGIN(Engine)
class CModel;
class CTexture;
END


BEGIN(Client)

class CEffectMesh : public CEffect
{
public:

	struct MESH_DESC {
		_float3 vCenter;
		_float3 vScale;
	};



public:
	CEffectMesh();
	virtual ~CEffectMesh() = default;


public:
	virtual HRESULT Initialize(const wstring& _strTextureKey, const wstring& _strModelKey, MESH_DESC* _MeshDesc, char* _strName);
	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta, _matrix _ParentMat = XMMatrixIdentity());
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();


public:

	shared_ptr<CModel> m_pModel = nullptr;
	shared_ptr<CTexture> m_pMaskTexture = nullptr;
	vector<shared_ptr<CTexture>> m_pNoiseTexture;


public:

	static shared_ptr<CEffectMesh> Create(const wstring& _strTextureKey, const wstring& _strModelKey, MESH_DESC* _MeshDesc, char* _strName);



};

END