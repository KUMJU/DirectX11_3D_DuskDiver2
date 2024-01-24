#include "pch.h"
#include "SkillSet.h"

#include "PlrNormalAtk.h"
#include "SkillQ.h"
#include "SkillR.h"
#include "SkillE.h"
#include "Skill.h"

CSkillSet::CSkillSet()
{
}

void CSkillSet::InitializeSkill()
{
	shared_ptr<CSkill> pSkill;
	
	//***********일반공격************//
	pSkill = CPlrNormalAtk::Create(1);
	m_Skills.push_back(pSkill);

	pSkill = CPlrNormalAtk::Create(2);
	m_Skills.push_back(pSkill);

	pSkill = CPlrNormalAtk::Create(3);
	m_Skills.push_back(pSkill);

	pSkill = CPlrNormalAtk::Create(4);
	m_Skills.push_back(pSkill);

	pSkill = CPlrNormalAtk::Create(5);
	m_Skills.push_back(pSkill);
	//*******************************//

	pSkill = CPlrNormalAtk::Create(5);
	m_Skills.push_back(pSkill);

	//*************BattleMode Skill***************//

	pSkill = CSkillQ::Create();
	m_Skills.push_back(pSkill);

	pSkill = CSkillE::Create();
	m_Skills.push_back(pSkill);

	pSkill = CSkillR::Create();
	m_Skills.push_back(pSkill);


	//*******************************//

	for (size_t i = 0; i < m_Skills.size(); ++i) {
		
		m_Skills[i]->SetSkillIndex(i);
	
	}

}

void CSkillSet::Tick(_float _fDelatTime)
{
	if (ESKILLSTATE::SKILL_END != m_eCurrentSkill) {

		m_Skills[m_eCurrentSkill]->Tick(_fDelatTime);

		if (!m_Skills[m_eCurrentSkill]->IsEnabled())
			m_eCurrentSkill = ESKILLSTATE::SKILL_END;
		
	}

}

void CSkillSet::Render()
{
	if (ESKILLSTATE::SKILL_END != m_eCurrentSkill) {

		m_Skills[m_eCurrentSkill]->Render();
	}
}

_bool CSkillSet::SwitchingSkill(ESKILLSTATE _eChangeSkill)
{
	if (ESKILLSTATE::SKILL_END != m_eCurrentSkill) {

		if (m_Skills[m_eCurrentSkill]->GetCancleAble()) {
			//중간취소 가능

			m_eCurrentSkill = _eChangeSkill;
			ActiveSkill(_eChangeSkill);
		}
		else {
			return false;
		}
	}
	else {

		m_eCurrentSkill = _eChangeSkill;
		ActiveSkill(_eChangeSkill);
	}

	return true;
}

_bool CSkillSet::ActiveSkill(ESKILLSTATE _eChangeSkill)
{
	m_Skills[_eChangeSkill]->ActiveSkill();
	m_Skills[_eChangeSkill]->SetEnable(true);

	return true;
}

void CSkillSet::InActiveSkill(ESKILLSTATE _ePastSkill)
{
}

void CSkillSet::SetPlayerTransform(shared_ptr<CTransform> _pTransform)
{
	for (auto& iter : m_Skills) {
		iter->SetOwnerTransform(_pTransform);
	}
	

}

shared_ptr<CSkillSet> CSkillSet::Create()
{
	shared_ptr<CSkillSet> pInstance = make_shared<CSkillSet>();
	pInstance->InitializeSkill();

	return pInstance;
}
