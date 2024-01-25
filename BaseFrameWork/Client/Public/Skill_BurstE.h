#pragma once
#include "Client_Defines.h"
#include "Skill.h"

class CSkill_BurstE : public CSkill
{
public:
	CSkill_BurstE();
	virtual ~CSkill_BurstE() =default;

public:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:

	static shared_ptr<CSkill_BurstE> Create();


};

