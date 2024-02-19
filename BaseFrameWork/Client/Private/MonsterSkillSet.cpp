#include "pch.h"
#include "MonsterSkillSet.h"

#include "Animation.h"
#include "Model.h"
#include "Skill.h"
#include "Transform.h"

#include "Enemy1Skill.h"
#include "Enemy2Skill.h"
#include "MiddleBossSkill.h"

#include "BossSkill1.h"
#include "BossSkill2.h"
#include "BossSkill3.h"
#include "BossSkill4.h"
#include "BossSkill5.h"

#include "GameInstance.h"

CMonsterSkillSet::CMonsterSkillSet()
{
}

void CMonsterSkillSet::InitializeSkill(_uint _iMonsterIdx, shared_ptr<CModel> _pBaseModel)
{

	vector<shared_ptr<CAnimation>> pAnimation = _pBaseModel->GetAnimations();


	switch (_iMonsterIdx)
	{
	case 0:
		SettingEnemy1Skill(_pBaseModel);
		break;

	case 1:
		SettingEnemy2Skill(_pBaseModel);
		break;

	case 2:
		SettingMiddleBossSkill(_pBaseModel);
		break;

	case 3:
		SettingFinalBossSkill(_pBaseModel);
		break;

	default:
		break;
	}


}

void CMonsterSkillSet::Tick(_float _fDelatTime)
{
	if (ESKILLSTATE::MON_SKILLEND != m_eCurrentSkill) {

		m_Skills[m_eCurrentSkill]->Tick(_fDelatTime);

		if (!m_Skills[m_eCurrentSkill]->IsEnabled())
			m_eCurrentSkill = ESKILLSTATE::MON_SKILLEND;
	}


}

void CMonsterSkillSet::LateTick(_float _fDeltaTime)
{

	if (ESKILLSTATE::MON_SKILLEND != m_eCurrentSkill) {
		m_Skills[m_eCurrentSkill]->LateTick(_fDeltaTime);
	}


}

void CMonsterSkillSet::Render()
{
	if (ESKILLSTATE::MON_SKILLEND != m_eCurrentSkill) {

		m_Skills[m_eCurrentSkill]->Render();
	}

}

_bool CMonsterSkillSet::SwitchingSkill(ESKILLSTATE _eChangeSkill)
{
	m_eCurrentSkill = _eChangeSkill;
	m_Skills[m_eCurrentSkill]->SetEnable(true);
	m_Skills[m_eCurrentSkill]->ActiveSkill();
	ActiveSkill(_eChangeSkill);

	return true;
}

_bool CMonsterSkillSet::ActiveSkill(ESKILLSTATE _eChangeSkill)
{
	return _bool();
}

void CMonsterSkillSet::InActiveSkill(ESKILLSTATE _ePastSkill)
{
}

void CMonsterSkillSet::SkillCancle()
{
	if (ESKILLSTATE::MON_SKILLEND == m_eCurrentSkill)
		return;

	m_Skills[m_eCurrentSkill]->SetEnable(false);
	m_Skills[m_eCurrentSkill]->SkillReset();
	m_eCurrentSkill = ESKILLSTATE::MON_SKILLEND;
}

void CMonsterSkillSet::SetTransform(shared_ptr<CTransform> _pTransform)
{
	for (auto& iter : m_Skills) {
		iter->SetOwnerTransform(_pTransform);
	}
}

HRESULT CMonsterSkillSet::SettingEnemy1Skill(shared_ptr<CModel> _pModel)
{
	shared_ptr<CSkill> pSkill;
	vector<shared_ptr<CAnimation>> pBaseAnimation = _pModel->GetAnimations();

	pSkill = CEnemy1Skill::Create(1);
	pSkill->SetAnimation(pBaseAnimation[0]);
	m_Skills.push_back(pSkill);

	pSkill = CEnemy1Skill::Create(2);
	pSkill->SetAnimation(pBaseAnimation[1]);
	m_Skills.push_back(pSkill);

	pSkill = CEnemy1Skill::Create(3);
	pSkill->SetAnimation(pBaseAnimation[5]);
	m_Skills.push_back(pSkill);

	pSkill = CEnemy1Skill::Create(4);
	pSkill->SetAnimation(pBaseAnimation[2]);
	m_Skills.push_back(pSkill);

	return S_OK;
}

HRESULT CMonsterSkillSet::SettingEnemy2Skill(shared_ptr<CModel> _pModel)
{
	shared_ptr<CSkill> pSkill;
	vector<shared_ptr<CAnimation>> pBaseAnimation = _pModel->GetAnimations();

	pSkill = CEnemy2Skill::Create(1);
	pSkill->SetAnimation(pBaseAnimation[0]);
	m_Skills.push_back(pSkill);

	pSkill = CEnemy2Skill::Create(2);
	pSkill->SetAnimation(pBaseAnimation[2]);
	m_Skills.push_back(pSkill);

	pSkill = CEnemy2Skill::Create(3);
	pSkill->SetAnimation(pBaseAnimation[4]);
	m_Skills.push_back(pSkill);

	return S_OK;
}

HRESULT CMonsterSkillSet::SettingMiddleBossSkill(shared_ptr<CModel> _pModel)
{
	shared_ptr<CSkill> pSkill;
	vector<shared_ptr<CAnimation>> pBaseAnimation = _pModel->GetAnimations();

	pSkill = CMiddleBossSkill::Create(1);
	pSkill->SetAnimation(pBaseAnimation[23]);
	m_Skills.push_back(pSkill);

	pSkill = CMiddleBossSkill::Create(2);
	pSkill->SetAnimation(pBaseAnimation[31]);
	m_Skills.push_back(pSkill);

	pSkill = CMiddleBossSkill::Create(3);
	pSkill->SetAnimation(pBaseAnimation[32]);
	m_Skills.push_back(pSkill);

	pSkill = CMiddleBossSkill::Create(4);
	pSkill->SetAnimation(pBaseAnimation[35]);
	m_Skills.push_back(pSkill);

	return S_OK;
}

HRESULT CMonsterSkillSet::SettingFinalBossSkill(shared_ptr<CModel> _pModel)
{

	shared_ptr<CSkill> pSkill;
	vector<shared_ptr<CAnimation>> pBaseAnimation = _pModel->GetAnimations();

	pSkill = CBossSkill1::Create();
	pSkill->SetAnimation(pBaseAnimation[0]);
	m_Skills.push_back(pSkill);

	pSkill = CBossSkill2::Create();
	pSkill->SetAnimation(pBaseAnimation[1]);
	m_Skills.push_back(pSkill);

	pSkill = CBossSkill3::Create();
	pSkill->SetAnimation(pBaseAnimation[2]);
	m_Skills.push_back(pSkill);

	pSkill = CBossSkill4::Create();
	pSkill->SetAnimation(pBaseAnimation[4]);
	m_Skills.push_back(pSkill);

	pSkill = CBossSkill5::Create();
	pSkill->SetAnimation(pBaseAnimation[8]);
	m_Skills.push_back(pSkill);

	return S_OK;
}

shared_ptr<CMonsterSkillSet> CMonsterSkillSet::Create(_uint _iMonsterIdx, shared_ptr<CModel> _pBaseModel)
{

	shared_ptr<CMonsterSkillSet> pMonsterSkillSet = make_shared<CMonsterSkillSet>();
	pMonsterSkillSet->InitializeSkill(_iMonsterIdx, _pBaseModel);

	return pMonsterSkillSet;
}
