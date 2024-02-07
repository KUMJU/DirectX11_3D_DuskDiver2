#pragma once

#include "Skill.h"

class CSkill_BurstQ : public CSkill
{
public:
	CSkill_BurstQ();
	virtual ~CSkill_BurstQ() = default;

public:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:

	static shared_ptr<CSkill_BurstQ> Create();


};

