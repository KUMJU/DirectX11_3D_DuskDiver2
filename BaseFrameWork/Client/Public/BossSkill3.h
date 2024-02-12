#pragma once

#include "Client_Defines.h"
#include "Skill.h"

BEGIN(Client)

class CBossSkill3 : public CSkill
{
public:
	CBossSkill3();
	virtual ~CBossSkill3() = default;

public:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	static shared_ptr<CBossSkill3> Create();

};

END