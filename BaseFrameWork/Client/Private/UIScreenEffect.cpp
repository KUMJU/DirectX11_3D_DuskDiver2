#include "pch.h"
#include "UIScreenEffect.h"

CUIScreenEffect::CUIScreenEffect()
{
}

HRESULT CUIScreenEffect::Initialize()
{
    return S_OK;
}

void CUIScreenEffect::PriorityTick(_float _fTimeDelta)
{
}

void CUIScreenEffect::Tick(_float _fTimeDelta)
{
}

void CUIScreenEffect::LateTick(_float _fTimeDelta)
{
}

HRESULT CUIScreenEffect::Render()
{
    return S_OK;
}

void CUIScreenEffect::SwitchScreenEffect(EFFECTTYPE _eEffectType)
{
}

shared_ptr<CUIScreenEffect> CUIScreenEffect::Create()
{
    return shared_ptr<CUIScreenEffect>();
}
