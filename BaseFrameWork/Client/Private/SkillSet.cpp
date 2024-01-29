#include "pch.h"
#include "SkillSet.h"

#include "PlrNormalAtk.h"
#include "SkillQ.h"
#include "SkillR.h"
#include "SkillE.h"
#include "Skill.h"
#include "Skill_BurstQ.h"
#include "Skill_BurstE.h"
#include "SuperSkillA.h"
#include "SuperSkillB.h"


#include "AirAtk.h"

#include "Model.h"
#include "Animation.h"

CSkillSet::CSkillSet()
{
}

void CSkillSet::InitializeSkill(shared_ptr<CModel> _pBaseModel, shared_ptr<CModel> _pBurstModel)
{
	m_pPlrBaseModel = _pBaseModel;
	m_pPlrBurstModel = _pBurstModel;

	vector<shared_ptr<CAnimation>> pBaseAnimation = m_pPlrBaseModel->GetAnimations();
	vector<shared_ptr<CAnimation>> pBurstAnimation = m_pPlrBurstModel->GetAnimations();

	shared_ptr<CSkill> pSkill;
	
	//***********일반공격************//
	pSkill = CPlrNormalAtk::Create(1);
	pSkill->SetAnimation(pBaseAnimation[10]);
	pSkill->SetBurstAnimation(pBurstAnimation[10]);
	m_Skills.push_back(pSkill);

	pSkill = CPlrNormalAtk::Create(2);
	pSkill->SetAnimation(pBaseAnimation[12]);
	pSkill->SetBurstAnimation(pBurstAnimation[12]);

	m_Skills.push_back(pSkill);

	pSkill = CPlrNormalAtk::Create(3);	
	pSkill->SetAnimation(pBaseAnimation[14]);
	pSkill->SetBurstAnimation(pBurstAnimation[14]);

	m_Skills.push_back(pSkill);

	pSkill = CPlrNormalAtk::Create(4);
	pSkill->SetAnimation(pBaseAnimation[15]);
	pSkill->SetBurstAnimation(pBurstAnimation[15]);

	m_Skills.push_back(pSkill);

	pSkill = CPlrNormalAtk::Create(5);
	pSkill->SetAnimation(pBaseAnimation[16]);
	pSkill->SetBurstAnimation(pBurstAnimation[16]);

	m_Skills.push_back(pSkill);
	//*******************************//

	pSkill = CPlrNormalAtk::Create(6);
	pSkill->SetAnimation(pBaseAnimation[35]);
	pSkill->SetBurstAnimation(pBurstAnimation[35]);
	m_Skills.push_back(pSkill);

	//********************AirAttack************************//


	pSkill = CAirAtk::Create(1);
	pSkill->SetAnimation(pBaseAnimation[0]);
	pSkill->SetBurstAnimation(pBurstAnimation[0]);
	m_Skills.push_back(pSkill);

	pSkill = CAirAtk::Create(2);
	pSkill->SetAnimation(pBaseAnimation[1]);
	pSkill->SetBurstAnimation(pBurstAnimation[1]);
	m_Skills.push_back(pSkill);

	pSkill = CAirAtk::Create(3);
	pSkill->SetAnimation(pBaseAnimation[2]);
	pSkill->SetBurstAnimation(pBurstAnimation[2]);
	m_Skills.push_back(pSkill);

	pSkill = CAirAtk::Create(4);
	pSkill->SetAnimation(pBaseAnimation[3]);
	pSkill->SetBurstAnimation(pBurstAnimation[3]);
	m_Skills.push_back(pSkill);


	//*************BattleMode Skill***************//

	pSkill = CSkillQ::Create();
	pSkill->SetAnimation(pBaseAnimation[60]);
	m_Skills.push_back(pSkill);

	pSkill = CSkillE::Create();
	pSkill->SetAnimation(pBaseAnimation[61]);
	m_Skills.push_back(pSkill);

	pSkill = CSkillR::Create();
	pSkill->SetAnimation(pBaseAnimation[62]);
	m_Skills.push_back(pSkill);


	//***************Burst Skill****************//

	pSkill = CSkill_BurstQ::Create();
	pSkill->SetAnimation(pBaseAnimation[63]); // 23
	pSkill->SetBurstAnimation(pBurstAnimation[63]);
	pSkill->SetBurstAnimation(pBurstAnimation[64]);
	pSkill->SetBurstAnimation(pBurstAnimation[65]);

	m_Skills.push_back(pSkill);

	pSkill = CSkill_BurstE::Create();
	pSkill->SetAnimation(pBaseAnimation[71]); // 88
	pSkill->SetBurstAnimation(pBurstAnimation[71]);

	m_Skills.push_back(pSkill);

	pSkill = CSkill_BurstQ::Create();
	pSkill->SetAnimation(pBaseAnimation[133]); //41
	pSkill->SetBurstAnimation(pBurstAnimation[133]);

	m_Skills.push_back(pSkill);

	//R: 133  E : 71 Q :63

	//***************Super Skill****************//

	pSkill = CSuperSkillA::Create();
	pSkill->SetAnimation(pBaseAnimation[91]); // 135
	pSkill->SetBurstAnimation(pBurstAnimation[91]);
	m_Skills.push_back(pSkill);

	pSkill = CSuperSkillB::Create();
	pSkill->SetAnimation(pBaseAnimation[92]); 
	pSkill->SetBurstAnimation(pBurstAnimation[92]);
	m_Skills.push_back(pSkill);


	for (size_t i = 0; i < m_Skills.size(); ++i) {
		
		m_Skills[i]->SetSkillIndex((_uint)i);
	
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

			m_Skills[m_eCurrentSkill]->SetEnable(false);
			m_Skills[m_eCurrentSkill]->SkillReset();

			m_eCurrentSkill = _eChangeSkill;
			//여기쪽 다시 생각해봐야할듯 일단 보류 
			//m_Skills[m_eCurrentSkill]->SetSkillCancleAble(false);
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

void CSkillSet::SetBurstMode(_bool _isBurst)
{
	if (_isBurst) {

		m_isBurstMode = _isBurst; 
		for (auto& iter : m_Skills) {
			iter->SwitchingBurstMode();
		}
	}
	else {



	}


}

shared_ptr<CSkillSet> CSkillSet::Create(shared_ptr<CModel> _pBaseModel, shared_ptr<CModel> _pBurstModel)
{
	shared_ptr<CSkillSet> pInstance = make_shared<CSkillSet>();
	pInstance->InitializeSkill(_pBaseModel, _pBurstModel);

	return pInstance;
}
