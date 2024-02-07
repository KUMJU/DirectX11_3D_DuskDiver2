#pragma once

#include "Tool_Defines.h"

BEGIN(Engine)
class CModel;
class CAnimation;
class CTransform;
END


BEGIN(Tool)

class CMonsterSkillSet
{
public:
	enum ESKILLSTATE {
		MON_SKILL1, MON_SKILL2, MON_SKILL3, MON_SKILL4, MON_SKILLEND
	};

public:
	CMonsterSkillSet();
	~CMonsterSkillSet() = default;
public:
	void InitializeSkill(_uint _iMonsterIdx, shared_ptr<CModel> _pBaseModel);
	void Tick(_float _fDelatTime);
	void Render();

public:

	_bool SwitchingSkill(ESKILLSTATE _eChangeSkill);
	_bool ActiveSkill(ESKILLSTATE _eChangeSkill);
	void InActiveSkill(ESKILLSTATE _ePastSkill);

	void SkillCancle(); // 중간에 플레이어한테 맞았거나 뭐 그래서 취소됨
	void SetTransform(shared_ptr<CTransform> _pTransform);


//SkillSet Initialize(몬스터 종류에 맞게 맞는 스킬을 배치해준다)
private:

	HRESULT SettingEnemy1Skill(shared_ptr<CModel> _pModel);
	HRESULT SettingEnemy2Skill(shared_ptr<CModel> _pModel);
	HRESULT SettingMiddleBossSkill(shared_ptr<CModel> _pModel);
	HRESULT SettingFinalBossSkill(shared_ptr<CModel> _pModel);

private:

	ESKILLSTATE m_eCurrentSkill = MON_SKILLEND;
	vector<shared_ptr<class CSkill>> m_Skills;

public:

	static shared_ptr<CMonsterSkillSet> Create(_uint _iMonsterIdx, shared_ptr<CModel> _pBaseModel);

};

END