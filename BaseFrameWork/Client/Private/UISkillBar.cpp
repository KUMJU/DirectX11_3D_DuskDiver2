#include "pch.h"
#include "UISkillBar.h"

CUISkillBar::CUISkillBar()
{
}

HRESULT CUISkillBar::Initialize()
{
    return S_OK;
}

void CUISkillBar::PriorityTick(_float _fTimeDelta)
{
}

void CUISkillBar::Tick(_float _fTimeDelta)
{
}

void CUISkillBar::LateTick(_float _fTimeDelta)
{
}

HRESULT CUISkillBar::Render()
{
    return S_OK;
}

shared_ptr<CUISkillBar> CUISkillBar::Create()
{
    shared_ptr<CUISkillBar> pInstance = make_shared<CUISkillBar>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CUISkillBar");

    return pInstance;
}
