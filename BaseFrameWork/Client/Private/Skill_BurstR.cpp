#include "pch.h"
#include "Skill_BurstR.h"

CSkill_BurstR::CSkill_BurstR()
{
}

HRESULT CSkill_BurstR::Initialize()
{
    m_eSkillOwner = EOWNER_TYPE::OWNER_PLAYER;


    return S_OK;
}

void CSkill_BurstR::PriorityTick(_float _fTimeDelta)
{
}

void CSkill_BurstR::Tick(_float _fTimeDelta)
{
}

void CSkill_BurstR::LateTick(_float _fTimeDelta)
{
}

HRESULT CSkill_BurstR::Render()
{
    return S_OK;
}

shared_ptr<CSkill_BurstR> CSkill_BurstR::Create()
{
    return shared_ptr<CSkill_BurstR>();
}
