#pragma once

#include "Effect.h"


BEGIN(Engine)
class CModel;
class CTexture;
END


BEGIN(Tool)

class CEffectMesh : public CEffect
{
public:

	struct MESH_DESC {
		_float3 vCenter;
		_float4 vColor;
		_float4 vLerpColor;
		_bool bNoise;
		_bool bMask;
		_bool bDiffuse;
		_float fNoiseSpeed;
		_float fMaskSpeed;
		_float2 vNoiseDir;
		_float2 vMaskDir;
		_float3 vStartScale;
		_float3 vMiddleScale;
		_float3 vEndScale;
		_float2 vDuration;
		_bool bUVLoop;
		_bool bLoop;
		char* szNoiseTexKey;
		char* szMaskTexKey;

		_float fScaleChangeTime;
	};



public:
	CEffectMesh();
	virtual ~CEffectMesh() = default;


public:
	virtual HRESULT Initialize(const wstring& _strTextureKey, const wstring& _strModelKey, MESH_DESC* _MeshDesc, char* _strName);
	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	void EditEffect(const wstring& _strTextureKey, const wstring& _strModelKey, MESH_DESC* _MeshDesc);


private:
	void ComputeInitSetting();
	void ScaleLerp();


public:

	shared_ptr<CModel> m_pModel = nullptr;
	shared_ptr<CTexture> m_pMaskTexture = nullptr;
	shared_ptr<CTexture> m_pNoiseTexture = nullptr;
	//vector<shared_ptr<CTexture>> m_pNoiseTexture;


private:
	_float4 m_vColor = _float4();
	_float4 m_vColor2 = _float4();
	MESH_DESC m_MeshDesc = {};

	_float m_fCurrentSpeed = 0.f;
	_float m_fTimeDelta = 0.f;

	_float m_fAccTime = 0.f;
	_float m_fMiddleTime = 0.f;
	
	wstring m_MeshKey = TEXT("");

private:
	
	/******Scale******/
	_float3 m_vStartScaleDiff = _float3();
	_float3 m_vEndScaleDiff = _float3();

	_vector m_vCurrentScale = _vector();
	
	/******Alpha******/



public:
	virtual void ParsingData(Json::Value& _root) override;
	virtual void ResetEffect();


public:

	static shared_ptr<CEffectMesh> Create(const wstring& _strTextureKey, const wstring& _strModelKey, MESH_DESC* _MeshDesc, char* _strName);



};

END