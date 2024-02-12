#pragma once

#include "Client_Defines.h"
#include "Skill.h"

BEGIN(Client)

class CBossSkill5 : public CSkill
{
public:
	CBossSkill5();
	virtual ~CBossSkill5() = default;

public:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	static shared_ptr<CBossSkill5> Create();

};

END