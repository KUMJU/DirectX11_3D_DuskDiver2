#pragma once

#include "Client_Defines.h"
#include "Skill.h"


BEGIN(Client)

class CAirAtk final : public CSkill
{
public:
	CAirAtk();
	virtual ~CAirAtk() = default;

public:
	virtual HRESULT Initialize(_uint _iComboNum);
	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

protected:
	virtual void EndSkill();

private:
	shared_ptr<class CEffectPreset> m_pFinishPreset = nullptr;

	_int m_iComboNum = 0;

public:

	static shared_ptr<CAirAtk> Create(_uint _iComboNum);

};

END