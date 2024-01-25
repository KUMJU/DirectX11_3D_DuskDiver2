#include "pch.h"
#include "Skill.h"

#include "Collider.h"
#include "GameInstance.h"
#include "Animation.h"

CSkill::CSkill()
{
}

HRESULT CSkill::Initialize()
{
	
	m_pMainAnimation = m_pAnimation;
	
	return S_OK;
}

void CSkill::PriorityTick(_float _fTimeDelta)
{
}

void CSkill::Tick(_float _fTimeDelta)
{
	if (!m_IsEnabled)
		return;

	if ((*m_pMainAnims).size() != 1) {

		if ((*m_pMainAnims)[m_iCurrentSkillOrder]->GetCurrentTrackPosition() >= m_Infos[m_iCurrentSkillOrder].iStartTrackPosition) {
			if (m_Infos[m_iCurrentSkillOrder].iEndTrackPosition <= (*m_pMainAnims)[m_iCurrentSkillOrder]->GetCurrentTrackPosition()) {
				++m_iCurrentSkillOrder;


				if (m_iCurrentSkillOrder == m_Infos.size()) {
					m_IsEnabled = false;
					SkillReset();
				}
				else {

					m_bDownAtk = m_Infos[m_iCurrentSkillOrder].bDownAtk;
					m_fDelay = m_Infos[m_iCurrentSkillOrder].fDelayTime;
					m_bKnokUp = m_Infos[m_iCurrentSkillOrder].bKnockUp;
				}



			}
			m_Collider->Tick(m_pOwnerTransform->GetWorldMatrix());
			CGameInstance::GetInstance()->AddCollider(CCollisionMgr::COL_PLRPROJ, m_Collider);
		}

	}
	else {

		if (m_pMainAnims->front()->GetCurrentTrackPosition() >= m_Infos[m_iCurrentSkillOrder].iStartTrackPosition) {
			if (m_Infos[m_iCurrentSkillOrder].iEndTrackPosition <= m_pMainAnims->front()->GetCurrentTrackPosition()) {
				++m_iCurrentSkillOrder;

				//���� ��ų �ε����� size ������ ������ ��ȸ�� �����ٴ� �ǹ�. ��ų ����� ������
				if (m_iCurrentSkillOrder == m_Infos.size()) {
					m_IsEnabled = false;
					SkillReset();
					return;
				}
				else
				{
					m_bDownAtk = m_Infos[m_iCurrentSkillOrder].bDownAtk;
					m_fDelay = m_Infos[m_iCurrentSkillOrder].fDelayTime;
					m_bKnokUp = m_Infos[m_iCurrentSkillOrder].bKnockUp;

				}
			}


			m_Collider->Tick(m_pOwnerTransform->GetWorldMatrix());
			CGameInstance::GetInstance()->AddCollider(CCollisionMgr::COL_PLRPROJ, m_Collider);
		}


	}




	

}

void CSkill::LateTick(_float _fTimeDelta)
{

	//collision �˻�

}

HRESULT CSkill::Render()
{
if (!m_IsEnabled)
	return S_OK;

#ifdef _DEBUG

	m_Collider->Render();
	
#endif // DEBUG

	return S_OK;
}

void CSkill::ActiveSkill()
{
}

void CSkill::SkillReset()
{
	m_fSkillActiveTime = 0.f;
	m_iCurrentSkillOrder = 0;
}

void CSkill::OnCollide(CGameObject::EObjType _eObjType, shared_ptr<CCollider> _pCol)
{
}
