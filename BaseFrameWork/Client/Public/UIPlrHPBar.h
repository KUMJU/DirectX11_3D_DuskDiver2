#pragma once

#include "UI.h"

BEGIN(Client)

class CUIPlrHPBar final: public CUI
{
public:
	CUIPlrHPBar();
	virtual ~CUIPlrHPBar() = default;

public:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void SetHP(_int _iHP);
	void SetMaxHP(_int _iMaxHP) { m_iMaxHP = _iMaxHP; }


private:
	_int m_iHP = 0;
	_int m_iMaxHP = 0;

	_float m_fUIStartPoint = 0.f;
	_float m_fUIEndPoint = 0.f;

	_float m_fCurrentHPRatio = 0.f;
	_float m_fPrevHPRatio = 0.f;

public:
	static shared_ptr<CUIPlrHPBar> Create();


};

END
