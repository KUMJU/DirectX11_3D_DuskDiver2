#pragma once

#include "UI.h"

BEGIN(Client)


class CUIScreenEffect : public CUI
{
public:
	enum EFFECTTYPE {
		TYPE_BLOOD, TYPE_SPEEDLINE, TYPE_REDEDGE, TYPE_FADE, TYPE_WHITEOUT, TYPE_SCENESCREEN, TYPE_END
	};


public:
	CUIScreenEffect();
	virtual ~CUIScreenEffect() = default;

public:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void SwitchScreenEffect(EFFECTTYPE _eEffectType);


private:
	_float m_fAccTime = 0.f;
	_float m_fTotalTime = 0.f;
	_float m_fAlpha = 0.f;
	_float m_TotalTimes[5] = { 1.f, 1.f, 2.f, 4.f, 3.f };


	EFFECTTYPE m_eEffectType = EFFECTTYPE::TYPE_END;

private:
	shared_ptr<CTexture> m_pEffectTexture[4];

public:
	static shared_ptr<CUIScreenEffect> Create();

};

END