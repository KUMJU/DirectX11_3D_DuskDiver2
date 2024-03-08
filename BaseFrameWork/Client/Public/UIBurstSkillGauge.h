#pragma once

#include "UI.h"

BEGIN(Client)

class CUIBurstSkillGauge : public CUI
{
public:
	CUIBurstSkillGauge();
	virtual ~CUIBurstSkillGauge() = default;

public:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void SetBurstSkillGauge(_float _fGaugeAmount);
	void ComputeUIAngle();

	void ActiveBurstSkill();

private:
	shared_ptr<CTexture> m_pBackgroundTexture = nullptr;
	shared_ptr<CTransform> m_pBackGroundTransform = nullptr;
	shared_ptr<CVIBuffer_UI> m_pBackGroundBuffer = nullptr;

private:

	_float m_fCurrentAngle = 0.f;

	_float m_fMaxGauge = 100.f;
	_float m_fCurrentBurstGauge = 0.f;

	_float m_fCurrentRate = 0.f;

private:
	shared_ptr<class CUI_SequenceTex> m_pUIEffect = nullptr;

public:
	static shared_ptr<CUIBurstSkillGauge> Create();

};

END