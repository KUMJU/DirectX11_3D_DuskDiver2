#pragma once

#include "Skill.h"
#include "Tool_Defines.h"

class CSuperSkillA : public CSkill
{
public:
	CSuperSkillA();
	virtual ~CSuperSkillA() = default;

public:
	virtual HRESULT Initialize();

	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:

	static shared_ptr<CSuperSkillA> Create();
};

