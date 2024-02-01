#pragma once

#include "Client_Defines.h"
#include "Skill.h"

BEGIN(Client)

class CMiddleBossSkill : public CSkill
{
public:
	CMiddleBossSkill();
	virtual ~CMiddleBossSkill() = default;


public:

	virtual HRESULT Initialize(_uint _iComboNum);

	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	static shared_ptr<CMiddleBossSkill> Create(_uint _iSkillNum);


};

END