#pragma once

#include "Effect.h"

BEGIN(Client)

class CEffectParticle : public CEffect
{
public:
	CEffectParticle();
	virtual ~CEffectParticle() = default;


public:
	virtual HRESULT Initialize(CTransform::TRANSFORM_DESC* _pDesc);
	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

private:
	shared_ptr<CVIBufferPoint> m_pVIBufferCom = nullptr;

public:
	static shared_ptr<CEffectParticle> Create();

};

END