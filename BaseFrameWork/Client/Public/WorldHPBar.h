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

	void HPBarReset();


private:
	//이전 체력과 현재 체력 차이 계산해서 쭈우욱 천천히 깎아줌 
	void CalcHPDiff(_float _fTimeDelta);


public:
	//월드 좌표를 스크린좌표로 변환
	void CalcScreenPos(_fvector _vWorldPos);


private:
	_int m_iHP = 0;
	_int m_iMaxHP = 0;

	_float m_fCurrentHPRatio = 1.f;
	_float m_fPrevHPRatio = 1.f;

	//현재 얼마씩 깎아주는지 
	_float m_fCurrentDiff = 0.f;

	_float2 m_vOriginSize;

	//HP바 체력깎여서 러프중인 상태 
	_bool m_IsHPLerp = false;


private:
	shared_ptr<CTexture> m_pBackgroundTexture = nullptr;
	shared_ptr<CTransform> m_pBackGroundTransform = nullptr;
	shared_ptr<CVIBuffer_UI> m_pBackGroundBuffer = nullptr;

public:
	static shared_ptr<CWorldHPBar> Create();

};

END