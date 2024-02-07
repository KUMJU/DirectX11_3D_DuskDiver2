#pragma once

#include "Tool_Defines.h"
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

public:

	static shared_ptr<CSuperSkillB> Create();

};

