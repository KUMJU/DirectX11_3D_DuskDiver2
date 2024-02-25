#include "pch.h"
#include "Barrier.h"
#include "GameInstance.h"
#include "Model.h"
#include "Texture.h"

#include "Collider.h"
#include "CollisionMgr.h"

CBarrier::CBarrier()
{
}

HRESULT CBarrier::Initialize(_vector _vPos)
{
    __super::Initialize(TEXT("TowerA"), nullptr);


    CCollider::COLLIDER_DESC CollDesc = {};

    CollDesc.vCenter = _float3(0.f, 1.f, 0.f);
    CollDesc.vExtents = _float3(0.5f, 1.f, 0.5f);
    
    CTransform::TRANSFORM_DESC desc = {};
    desc.fRotationPerSet = 2.f;
    m_pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_AABB, CollDesc);
    m_pCollider->SetOwner(shared_from_this());


    return S_OK;
}

void CBarrier::PriorityTick(_float _fTimeDelta)
{
}

void CBarrier::Tick(_float _fTimeDelta)
{
}

void CBarrier::LateTick(_float _fTimeDelta)
{
}

HRESULT CBarrier::Render()
{
    return S_OK;
}

void CBarrier::OnCollide(EObjType _eObjType, shared_ptr<CCollider> _pCollider)
{
}

shared_ptr<CBarrier> CBarrier::Create(_vector _vPos)
{
    shared_ptr<CBarrier> pInstance = make_shared<CBarrier>();

    if (FAILED(pInstance->Initialize(_vPos)))
        MSG_BOX("Failed to Create : CBarrier");

    return pInstance;
}


