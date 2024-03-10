#pragma once

#include "Client_Defines.h"
#include "Skill.h"

BEGIN(Client)


class CSkill_BurstR : public CSkill
{
public:
	CSkill_BurstR();
	virtual ~CSkill_BurstR() = default;

public:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

private:
	_bool m_bShaking = false;

	shared_ptr<class CGroundCrack> m_pGroundCrack = nullptr;

public:

	static shared_ptr<CSkill_BurstR> Create();



};

END

