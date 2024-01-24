#pragma once

#include "Skill.h" 

class CSkillE : public CSkill
{
public:
	CSkillE();
	virtual ~CSkillE() = default;

public:
	virtual HRESULT Initialize();

	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:

	static shared_ptr<CSkillE> Create();

};

