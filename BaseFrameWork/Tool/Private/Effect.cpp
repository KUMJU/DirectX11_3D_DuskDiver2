#include "pch.h"
#include "Effect.h"

CEffect::CEffect()
{
}

HRESULT CEffect::Initialize(CTransform::TRANSFORM_DESC* _pDesc)
{
    __super::Initialize(_pDesc);
    return S_OK;
}

void CEffect::PriorityTick(_float _fTimeDelta)
{
}

void CEffect::Tick(_float _fTimeDelta)
{
}

void CEffect::LateTick(_float _fTimeDelta)
{
}

HRESULT CEffect::Render()
{
    return S_OK;
}
