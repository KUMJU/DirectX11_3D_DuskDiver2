#pragma once

#include "Client_Defines.h"

BEGIN(Engine)

class CTransform;

END

//이름만 이렇지 SkillState와 하는 일이 같다고 봐도 될듯...???? 

BEGIN(Client)

class CSkillSet
{
public:
	enum ESKILLSTATE {
		SKILL_ATK1, SKILL_ATK2, SKILL_ATK3, SKILL_ATK4, SKILL_ATK5,SKILL_HEAVYATK,
		SKILL_Q, SKILL_E, SKILL_R, SKILL_BURST_Q, SKILL_BURST_E, SKILL_BURST_R,
		SKILL_SUPER1, SKILL_SUPER2, SKILL_SPECIAL, SKILL_END
	};


public:
	CSkillSet();
	~CSkillSet() = default;
public:
	void InitializeSkill();
	void Tick(_float _fDelatTime);
	void Render();

public:
	_bool SwitchingSkill(ESKILLSTATE _eChangeSkill);
	_bool ActiveSkill(ESKILLSTATE _eChangeSkill);
	void InActiveSkill(ESKILLSTATE _ePastSkill);

	void SetPlayerTransform(shared_ptr<CTransform> _pTransform);
private:
	vector<shared_ptr<class CSkill>> m_Skills;
	ESKILLSTATE m_eCurrentSkill = SKILL_END;

	shared_ptr<CTransform> m_pPlrTransform = nullptr;

	_int m_iCurrentSkill = -1;


public:

	static shared_ptr<CSkillSet> Create();


};

END