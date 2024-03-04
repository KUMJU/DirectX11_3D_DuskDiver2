#include "pch.h"
#include "Effect.h"

#include "GameInstance.h"

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

void CEffect::Tick(_float _fTimeDelta, _matrix _ParentMat)
{
}

void CEffect::LateTick(_float _fTimeDelta)
{

}

HRESULT CEffect::Render()
{
    return S_OK;
}

void CEffect::AddDistortionRender()
{

   // if (FAILED(CGameInstance::GetInstance()->AddRenderGroup(CRenderer::render, shared_from_this())))
    //    return;


}

void CEffect::SetRotation(_float _fX, _float _fY, _float _fZ)
{

    m_pTransformCom->RotaitionRollYawPitch(XMConvertToRadians(_fX), XMConvertToRadians(_fY), XMConvertToRadians(_fZ));


}

shared_ptr<CEffect> CEffect::CloneEffect()
{
    shared_ptr<CEffect> pEffect;

    return pEffect;

}
