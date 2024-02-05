#pragma once

#include "UI.h"

BEGIN(Client)

class CWorldHPBar final: public CUI
{
public:
	CWorldHPBar();
	virtual ~CWorldHPBar() = default;

public:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void SetHP(_int _iHP);
	void SetMaxHP(_int _iMaxHP) { 
		m_iMaxHP = _iMaxHP; 
		m_iHP = _iMaxHP;
	}

public:
	//월드 좌표를 스크린좌표로 변환
	void CalcScreenPos(_fvector _vWorldPos);


private:
	_int m_iHP = 0;
	_int m_iMaxHP = 0;

	_float m_fCurrentHPRatio = 1.f;
	_float m_fPrevHPRatio = 0.f;

	_float2 m_vOriginSize;

public:
	static shared_ptr<CWorldHPBar> Create();

};

END