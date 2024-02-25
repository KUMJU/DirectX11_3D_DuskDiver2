#pragma once
#include "UI.h"

BEGIN(Client)

class CUICommandGame : public CUI
{
public:
	CUICommandGame();
	virtual ~CUICommandGame() = default;

public:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void SetCompleteNum(_int _fCompleteNum) {
		m_iCompleteNum = _fCompleteNum;
		ComputeRate();
	}

	void ComputeRate();

	void GameStart() { 
		m_bStart = true;
		m_iAlarmTexNum = 0;
	}
	void GameClear() { 
		m_bEnd = true; 
		m_iAlarmTexNum = 1;
	}

public:
	//게이지바
	//키 입력 설명 

	shared_ptr<CTexture> m_pGaugeTex = nullptr;
	shared_ptr<CTexture> m_pGaugeBackGround = nullptr;

	shared_ptr<CTransform> m_pGaugeTransform = nullptr;
	shared_ptr<CTransform> m_pGaugeBackGroundTransform = nullptr;

	shared_ptr<CTexture> m_pAlarmTexture[2];
	shared_ptr<CTransform> m_pAlarmTransform = nullptr;


private:

	_int m_iCompleteNum = 0;
	_int m_iClearMaxNum = 20;

	_float m_fSuccessRate = 0.f;

	_bool m_bStart = false;
	_bool m_bEnd = false;

	_float m_fAlarmAccTime = 0.f;
	_float m_fAccTotalTime = 3.f;

	_int m_iAlarmTexNum = 0;

public:
	static shared_ptr<CUICommandGame> Create();


};

END