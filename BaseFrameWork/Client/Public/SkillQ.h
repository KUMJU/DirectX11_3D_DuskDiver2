#pragma once

#include "Client_Defines.h"
#include "Skill.h"

BEGIN(Client)

class CSkillQ : public CSkill
{
public:
	CSkillQ();
	virtual ~CSkillQ() = default;

public:
	virtual HRESULT Initialize();

	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:

	static shared_ptr<CSkillQ> Create();


};

END