#pragma once

#include "Effect.h"

BEGIN(Engine)

class CVIBuffer_UI;
class CVIRect;

END


BEGIN(Tool)


class CEffectTexture : public CEffect
{
public:
	struct TEXEFFECT_DESC {
		_float2 vStartScale;
		_float2 vMiddleScale;
		_float2 vEndScale;
		_float3 vRotation;
		_float3 vCenter;
		_float4 vColor;
		_float2 vDuration;
		_float	fTurnSpeed;
		_float4 vTurnAxis;

		_float fScaleChangeTime;

		_bool bLoop;

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
public:
	virtual void ParsingData(Json::Value& _root) override;
	virtual void ResetEffect();

	TEXEFFECT_DESC GetDesc() { return m_TextureDesc; }

public:

	void EditDesc(const wstring& _strTextureKey, TEXEFFECT_DESC _desc);


private:
	void ComputeInitData();

	void ScaleLerp();


private:

	_float4 m_vColor = { 1.f, 1.f, 1.f, 1.f };
	shared_ptr<CVIBuffer_UI> m_pVIBuffer = nullptr;

	wstring m_TextureKey = TEXT("");


	TEXEFFECT_DESC m_TextureDesc = {};

	_float m_fDurationStart = 0.f;
	_float m_fDurationEnd = 0.f;


	_float m_fAccTime = 0.f;

	_vector m_vTurnAxis = _vector();
	_vector m_vCurrentScale = _vector();

	_float m_fTimeDelta = 0.f;

private:
	_float2 m_vStartScaleDiff = _float2();
	_float2 m_vEndScaleDiff = _float2();

	_float m_fMiddleTime = 0.f;

public:
	static shared_ptr<CEffectTexture> Create(const wstring& _strTextureKey, TEXEFFECT_DESC* _TextureDesc, char* _strName);


};

END