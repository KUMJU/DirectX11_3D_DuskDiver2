#include "pch.h"
#include "Barrier.h"

CBarrier::CBarrier()
{
}

HRESULT CBarrier::Initialize()
{
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

shared_ptr<CBarrier> CBarrier::Create()
{
    shared_ptr<CBarrier> pInstance = make_shared<CBarrier>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CBarrier");

    return pInstance;
}
