#pragma once

#include "MapObject.h"

BEGIN(Engine)
class CCollider;
END

BEGIN(Client)

class CCoin : public CMapObject
{
public:
	CCoin();
	virtual ~CCoin() = default;

public:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void OnCollide(EObjType _eObjType, shared_ptr<CCollider> _pCollider) override;

private:
	_float m_fLerpTime = 0.f;
	_float m_fScaleLerpTotalTime = 0.3f;


	_bool m_bCollided = false;


	_float3 m_vInitScale = _float3();
	_float m_fInitYPos = float();
private:
	void ScaleLerp();

private:
	shared_ptr<CCollider> m_pCollider = nullptr;
	
private:
	//Effect
	shared_ptr<class CEffectPreset> m_pGlowPreset= nullptr;
	shared_ptr<class CEffectPreset> m_pGetCoinEffect= nullptr;

public:
	static shared_ptr<CCoin> Create();

};

END