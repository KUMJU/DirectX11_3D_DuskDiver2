#pragma once

#include "Client_Defines.h"
#include "Skill.h"

BEGIN(Client)

class CBossSkill2 : public CSkill
{
public:
	CBossSkill2();
	virtual ~CBossSkill2() = default;

public:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

private:
	shared_ptr<class CGroundCrack> m_pGroundCrack = nullptr;


public:
	static shared_ptr<CBossSkill2> Create();

};

END