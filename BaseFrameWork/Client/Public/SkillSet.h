#pragma once

#include "Client_Defines.h"

BEGIN(Engine)
class CTransform;
class CModel;
END

BEGIN(Client)

class CSkillSet
{
public:
	enum ESKILLSTATE {
		SKILL_ATK1, SKILL_ATK2, SKILL_ATK3, SKILL_ATK4, SKILL_ATK5,SKILL_HEAVYATK,
		SKILL_AIRATK1, SKILL_AIRATK2, SKILL_AIRATK3, SKILL_AIRHEAVY,
		SKILL_Q, SKILL_E, SKILL_R, SKILL_BURST_Q, SKILL_BURST_E, SKILL_BURST_R,
		SKILL_SUPER1, SKILL_SUPER2, SKILL_SPECIAL, SKILL_END
	};


public:
	CSkillSet();
	~CSkillSet() = default;
public:
	void InitializeSkill(shared_ptr<CModel> _pBaseModel, shared_ptr<CModel> _pBurstModel);
	void Tick(_float _fDelatTime);
	void LateTick(_float _fDeltaTime);
	void Render();

public:
	_bool SwitchingSkill(ESKILLSTATE _eChangeSkill);
	_bool ActiveSkill(ESKILLSTATE _eChangeSkill);
	void InActiveSkill(ESKILLSTATE _ePastSkill);

	void SetPlayerTransform(shared_ptr<CTransform> _pTransform);

	void SetPlayerModel(shared_ptr<CModel> _pBaseModel) { m_pPlrBaseModel = _pBaseModel; }
	void SetPlayerBurstModel(shared_ptr<CModel> _pBaseModel) { m_pPlrBurstModel = _pBaseModel; }

	void SetBurstMode(_bool _isBurst);

	_bool CheckMoveEnable();

private:
	vector<shared_ptr<class CSkill>> m_Skills;
	ESKILLSTATE m_eCurrentSkill = SKILL_END;

	shared_ptr<CTransform> m_pPlrTransform = nullptr;

	shared_ptr<CModel> m_pPlrBaseModel = nullptr;
	shared_ptr<CModel> m_pPlrBurstModel = nullptr;

	_int m_iCurrentSkill = -1;
	_bool m_isBurstMode = false;

public:
	static shared_ptr<CSkillSet> Create(shared_ptr<CModel> _pBaseModel, shared_ptr<CModel> _pBurstModel);
};

END