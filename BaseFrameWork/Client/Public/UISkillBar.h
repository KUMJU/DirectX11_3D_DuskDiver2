#pragma once

#include "UI.h"

BEGIN(Client)

class CUISkillBar : public CUI
{
public:
	CUISkillBar();
	virtual ~CUISkillBar() = default;

public:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void SetSkillGauge(_int _iSkillGauge , _bool _isIncrease);
	void ComputeRatio(_float _fTimeRate);
	void SetCurrentGauging(_float _fRatio);

private:
	vector<shared_ptr<CTransform>> m_pTexTransforms;

private:
	_bool m_IsGaugeFull[4] = {true, true, true, true};
	_float m_fGaugeRatio[4] = {1.f, 1.f , 1.f, 1.f};

	_float m_fCurrentGaugingRatio = 0.f;
	_int m_iFirstEmptyIdx = 4;

public:
	static shared_ptr<CUISkillBar> Create();


};

END