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

private:



	_bool m_IsGaugeFull[4];


public:
	static shared_ptr<CUISkillBar> Create();


};

END