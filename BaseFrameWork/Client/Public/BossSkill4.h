#pragma once

#include "Client_Defines.h"
#include "Skill.h"

BEGIN(Client)

class CBossSkill4 : public CSkill
{
public:
	CBossSkill4();
	virtual ~CBossSkill4() = default;

public:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	static shared_ptr<CBossSkill4> Create();

};

END