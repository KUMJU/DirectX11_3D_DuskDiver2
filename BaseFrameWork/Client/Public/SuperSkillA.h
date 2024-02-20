#pragma once

#include "Skill.h"
#include "Client_Defines.h"

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

protected:
	virtual void EndSkill();


public:

	static shared_ptr<CSuperSkillA> Create();
};

