#include "pch.h"
#include "Skill.h"

CSkill::CSkill()
{
}

HRESULT CSkill::Initialize(CTransform::TRANSFORM_DESC* _pDesc)
{
	return S_OK;
}

void CSkill::PriorityTick(_float _fTimeDelta)
{
}

void CSkill::Tick(_float _fTimeDelta)
{
	m_fSkillActiveTime += _fTimeDelta;

	if (m_fSkillActiveTime >= m_fSkillDuration) {
		m_IsEnabled = false;
	}


}

void CSkill::LateTick(_float _fTimeDelta)
{

	//collision °Ë»ç

}

HRESULT CSkill::Render()
{
	return S_OK;
}

void CSkill::ActiveSkill()
{
}
