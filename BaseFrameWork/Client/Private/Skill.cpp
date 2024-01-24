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


	if (m_fSkillActiveTime >= m_fStartDelayTime ) {
		for (auto& iter : m_Colliders) {
			iter->Tick(m_pOwnerTransform->GetWorldMatrix());
			CGameInstance::GetInstance()->AddCollider(CCollisionMgr::COL_PLRPROJ, iter);
		}

		if (m_fSkillActiveTime >= m_fSkillDuration) {
			m_IsEnabled = false;
			SkillReset();
		}
	}
}

void CSkill::LateTick(_float _fTimeDelta)
{

	//collision °Ë»ç

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
}

void CSkill::OnCollide(CGameObject::EObjType _eObjType, shared_ptr<CCollider> _pCol)
{
}
