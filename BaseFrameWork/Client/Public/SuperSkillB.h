#pragma once

#include "Client_Defines.h"
#include "Skill.h"

class CSuperSkillB : public CSkill
{
public:
	CSuperSkillB();
	virtual ~CSuperSkillB() = default;


public:
	virtual HRESULT Initialize();

	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

private:
	_bool m_bTimerSlowMotion = false;
	_float m_fSkillBAccTime = 0.f;

public:

	static shared_ptr<CSuperSkillB> Create();

};

