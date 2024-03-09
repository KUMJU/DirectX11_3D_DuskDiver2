#pragma once

#include "Effect.h"
#include "VIBufferPoint.h"

BEGIN(Client)

class CEffectParticle : public CEffect
{
public:
	CEffectParticle();
	CEffectParticle(const CEffectParticle& _rhs);
	virtual ~CEffectParticle() = default;


public:
	virtual HRESULT Initialize(_uint _iInstanceNum, const wstring& _strTextureKey, CVIBufferInstancing::INSTANCE_DESC* _desc, char* _strName, _bool _bLoop);
	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta, _matrix _ParentMat = XMMatrixIdentity());
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT RenderGlow(shared_ptr<CShader> _pShader);


private:
	shared_ptr<CVIBufferPoint> m_pVIBufferCom = nullptr;

public:
	virtual void ResetEffect();
	void SetGlowColor(_float4 _vColor) {
		m_vGlowColor = _vColor;
	}

private:
	CVIBufferInstancing::INSTANCE_DESC m_InstanceDesc = {};
	_uint m_iInstanceNum = 0;

	wstring m_TextureKey = TEXT("");

	_bool m_bFollow = false;
	_bool m_bSetParentMat = false;

	_float4 m_vGlowColor = _float4();
	
private:
	_matrix m_ParentMat;

public:
	static shared_ptr<CEffectParticle> Create(_uint _iInstanceNum, const wstring& _strTextureKey, CVIBufferInstancing::INSTANCE_DESC* _desc, char* _strName, _bool _bLoop);
	shared_ptr<CEffect> CloneEffect();

};

END