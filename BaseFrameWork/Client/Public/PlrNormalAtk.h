#pragma once

#include "Client_Defines.h"
#include "Skill.h"

BEGIN(Client)

class CPlrNormalAtk final : public CSkill
{
public:
	CPlrNormalAtk();
	virtual ~CPlrNormalAtk() = default;
	
public:
	virtual HRESULT Initialize(_uint _iComboNum);

	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:

	static shared_ptr<CPlrNormalAtk> Create(_uint _iComboNum);


};

END