#pragma once

#include "Effect.h"
#include "VIBufferInstancing.h"

BEGIN(Tool)

class CEffectParticle : public CEffect
{
public:
	CEffectParticle();
	virtual ~CEffectParticle() = default;


public:
	virtual HRESULT Initialize(_uint _iInstanceNum, const wstring& _strTextureKey, CVIBufferInstancing::INSTANCE_DESC* _desc, char* _strName);
	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

private:
	shared_ptr<CVIBufferPoint> m_pVIBufferCom = nullptr;


public:
	virtual void ParsingData(Json::Value& _root) override;


public:
	virtual void ResetEffect();

private:
	CVIBufferInstancing::INSTANCE_DESC m_InstanceDesc = {};
	_uint m_iInstanceNum = 0;

	wstring m_TextureKey = TEXT("");

public:
	static shared_ptr<CEffectParticle> Create(_uint _iInstanceNum, const wstring& _strTextureKey, CVIBufferInstancing::INSTANCE_DESC* _desc , char* _strName);

};

END