#pragma once

#include "Client_Defines.h"
#include "Skill.h"

BEGIN(Client)

class CEnemy2Skill : public CSkill
{
public:
	CEnemy2Skill();
	virtual ~CEnemy2Skill() = default;

public:

	virtual HRESULT Initialize(_uint _iComboNum);

	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	static shared_ptr<CEnemy2Skill> Create(_uint _iSkillNum);

};

END
