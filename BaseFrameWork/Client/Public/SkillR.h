#pragma once

#include "Client_Defines.h"
#include "Skill.h"

class CSkillR : public CSkill
{
public:
	CSkillR();
	virtual ~CSkillR() = default;

public:
	virtual HRESULT Initialize();

	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:

	static shared_ptr<CSkillR> Create();


};

