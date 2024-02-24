#pragma once

#include "UI.h"

BEGIN(Client)


class CUIScreenEffect : public CUI
{
public:
	enum EFFECTTYPE {
		TYPE_BLOOD, TYPE_SPEEDLINE, TYPE_REDEDGE, TYPE_END
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
	shared_ptr<CTexture> m_pEffectTexture[3];

public:
	static shared_ptr<CUIScreenEffect> Create();

};

END