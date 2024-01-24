#include "pch.h"
#include "Skill.h"

#include "Collider.h"
#include "GameInstance.h"

CSkill::CSkill()
{
}

HRESULT CSkill::Initialize()
{
	return S_OK;
}

void CSkill::PriorityTick(_float _fTimeDelta)
{
}

void CSkill::Tick(_float _fTimeDelta)
{
	if (!m_IsEnabled)
		return;

	m_fSkillActiveTime += _fTimeDelta;

	if (m_fSkillActiveTime >= m_Infos[m_iCurrentSkillOrder].fDelayTime) {
		
		if (m_fSkillActiveTime >= m_Infos[m_iCurrentSkillOrder].fSkillDuration) {
			++m_iCurrentSkillOrder;

			//���� ��ų �ε����� size ������ ������ ��ȸ�� �����ٴ� �ǹ�. ��ų ����� ������
			if (m_iCurrentSkillOrder == m_Infos.size()) {
				m_IsEnabled = false;
				SkillReset();
				return;
			}
			else
			{
				m_fSkillActiveTime = 0.f;
				m_bDownAtk = m_Infos[m_iCurrentSkillOrder].bDownAtk;
				m_fDelay = m_Infos[m_iCurrentSkillOrder].fDelayTime;
				m_bKnokUp = m_Infos[m_iCurrentSkillOrder].bKnockUp;

			}
		}
		
		
		for (auto& iter : m_Colliders) {
			iter->Tick(m_pOwnerTransform->GetWorldMatrix());
			CGameInstance::GetInstance()->AddCollider(CCollisionMgr::COL_PLRPROJ, iter);
		}

	}
}

void CSkill::LateTick(_float _fTimeDelta)
{

	//collision �˻�

}

HRESULT CSkill::Render()
{
#ifdef _DEBUG

	for (auto& iter : m_Colliders) {
		iter->Render();
	}
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
