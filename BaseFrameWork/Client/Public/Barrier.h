#pragma once

#include "MapObject.h"

BEGIN(Engine)
class CCollider;
class CTexture;
class CVIBuffer_UI;
END

BEGIN(Client)

class CBarrier : public CGameObject
{
public:
	CBarrier();
	virtual ~CBarrier() = default;

public:
	virtual HRESULT Initialize(_vector _vPos, _float3 _vRotation);
	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void OnCollide(EObjType _eObjType, shared_ptr<CCollider> _pCollider) override;

public:
	void ComputeColor(_float _fDeltaTime);

private:

	shared_ptr<CVIBuffer_UI> m_pVIBuffer = nullptr;
	shared_ptr<CTexture> m_pTexture = nullptr;

	shared_ptr<CCollider> m_pCollider = nullptr;
	shared_ptr<CShader> m_pShader = nullptr;

private:

	_float m_fColorChangeTime = 0.f;


	_float4 m_vRGBColor = _float4();
	
	_vector m_vOriginColor = _vector({ 1.f, 1.f, 1.f, 1.f });
	_vector m_vChangeColor = _vector({ 1.f, 0.4f, 0.6f, 1.f });

	_float m_fSign = 1.f;

public:
	static shared_ptr<CBarrier> Create(_vector _vPos, _float3 _vRotation);

};

END
