#pragma once

#include "UI.h"

BEGIN(Client)

class CUIBurstGaugeBar : public CUI
{
public:
	CUIBurstGaugeBar();
	virtual ~CUIBurstGaugeBar() = default;

public:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void SetCurrentBurstGauge(_float _fBurstEnergy);
	void AddBurstGauge();
	void DecreaseBurstGauge();
	void SetBurstMode(_bool _bBurst);

private:
	void ComputeRatio();

private:
	vector<shared_ptr<CTransform>> m_pTexTransforms;

private:
	_float m_fCurrentBurstGauge = 0.f;

	_bool m_IsGaugeFull[3] = { true, true, true };
	_float m_fGaugeRatio[3] = { 0.f, 0.f , 0.f };

private:
	_bool m_bBurstMode = false;

private:
	shared_ptr<class CUI_SequenceTex> m_pUIEffect = nullptr;

public:
	static shared_ptr<CUIBurstGaugeBar> Create();

};

END