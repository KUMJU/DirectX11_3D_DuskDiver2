#pragma once

#include "GameObject.h"

BEGIN(Client)

class CSpecialBossPattern : public CGameObject
{
public:
	CSpecialBossPattern();
	~CSpecialBossPattern() = default;


public:
	virtual HRESULT Initialize(shared_ptr<class CFinalBoss> _pBoss);

	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	void PatternStart();
	void StartEvent();

private:

	_int m_iPressNum = 0;
	_int m_iSuccessNum = 0;

	_bool m_bStartDone = false;
	_bool m_bCamFocusDone = false;
	
	_float m_fAccTime = 0.f;


private:
	void KeyInput();
	void SuccessPattern();

private:
	weak_ptr<class CFinalBoss> m_pFinalBoss;
	shared_ptr<class CUI_SequenceTex> m_pGaugeUI = nullptr;
	shared_ptr<class CPlayer> m_pPlayer = nullptr;

private:
	_bool m_bFirstEventStart = false;
	_bool m_bSecondEvenetStart = false;

public:

	static shared_ptr<CSpecialBossPattern> Create(shared_ptr<class CFinalBoss> _pBoss);



};


END