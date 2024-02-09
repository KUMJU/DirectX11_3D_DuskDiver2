#pragma once

#include "Effect.h"

BEGIN(Engine)

class CVIBuffer_UI;

END


BEGIN(Tool)


class CEffectTexture : public CEffect
{
public:
	struct TEXEFFECT_DESC {
		_float2 vScale; //X, Z 
		_float3 vRotation;
		_float3 vCenter;
		_float4 vColor;
	};

public:
	CEffectTexture();
	virtual ~CEffectTexture() = default;

public:
	virtual HRESULT Initialize(const wstring& _strTextureKey, TEXEFFECT_DESC* _TextureDesc, char* _strName);
	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

private:

	_float4 m_vColor = { 1.f, 1.f, 1.f, 1.f };
	shared_ptr<CVIBuffer_UI> m_pVIBuffer = nullptr;

public:
	static shared_ptr<CEffectTexture> Create(const wstring& _strTextureKey, TEXEFFECT_DESC* _TextureDesc, char* _strName);


};

END