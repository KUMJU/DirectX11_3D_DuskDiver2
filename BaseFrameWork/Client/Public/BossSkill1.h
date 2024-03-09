#pragma once

#include "Client_Defines.h"
#include "Skill.h"

BEGIN(Client)

class CBossSkill1 : public CSkill
{
public:
	CBossSkill1();
	virtual ~CBossSkill1() = default;

public:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();


public:
	static shared_ptr<CBossSkill1> Create();

};

END