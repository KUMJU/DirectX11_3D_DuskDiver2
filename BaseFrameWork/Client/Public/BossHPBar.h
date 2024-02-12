#pragma once

#include "UI.h"

BEGIN(Client)

class CBossHPBar : public CUI
{
public:
	CBossHPBar();
	virtual ~CBossHPBar() = default;


public:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;


public:
	void SetHP(_int _iHP);
	void SetMaxHP(_int _iMaxHP) { m_iMaxHP = _iMaxHP; }
	void CalcHPDiff(_float _fTimeDelta);

private:
	_int m_iHP = 100;
	_int m_iMaxHP = 100;

	_float m_fUIStartPoint = 0.f;
	_float m_fUIEndPoint = 0.f;

	_float m_fCurrentHPRatio = 0.f;
	_float m_fPrevHPRatio = 0.f;

	_float m_fCurrentDiff = 0.f;
	_bool m_IsHPLerp = false;


private:
	shared_ptr<CTexture> m_pBackgroundTexture = nullptr;
	shared_ptr<CTransform> m_pBackGroundTransform = nullptr;
	shared_ptr<CVIBuffer_UI> m_pBackGroundBuffer = nullptr;


public:
	static shared_ptr<CBossHPBar> Create();

};

END