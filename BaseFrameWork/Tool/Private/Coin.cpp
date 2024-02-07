#include "pch.h"
#include "Coin.h"

#include "Collider.h"
#include "GameInstance.h"

CCoin::CCoin()
{
}

HRESULT CCoin::Initialize()
{
    CTransform::TRANSFORM_DESC desc = {};
    desc.fRotationPerSet = 1.5f;
    desc.fSpeedPerSet = 0.f;

    __super::Initialize(TEXT("GoldA"), &desc);


    CCollider::COLLIDER_DESC CoinColDesc = {};

    CoinColDesc.vCenter = _float3(0.f, 0.4f, 0.f);
    CoinColDesc.fRadius = 0.6f;

    m_pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_SPHERE, CoinColDesc);
    m_pCollider->SetOwner(shared_from_this());


    return S_OK;
}

void CCoin::PriorityTick(_float _fTimeDelta)
{
    m_pTransformCom->Turn({ 0.f, 1.f, 0.f, 0.f }, _fTimeDelta);

}

void CCoin::Tick(_float _fTimeDelta)
{
    m_pCollider->Tick(m_pTransformCom->GetWorldMatrix());
    CGameInstance::GetInstance()->AddCollider(CCollisionMgr::COL_MONSTER, m_pCollider);

}

void CCoin::LateTick(_float _fTimeDelta)
{
    __super::LateTick(_fTimeDelta);

}

HRESULT CCoin::Render()
{
    __super::Render();

    return S_OK;
}

void CCoin::OnCollide(EObjType _eObjType, shared_ptr<CCollider> _pCollider)
{
    if (CGameObject::EObjType::OBJ_PLAYER == _eObjType) {

        //ÄÚÀÎ¸Ô±â 
    }

}

shared_ptr<CCoin> CCoin::Create()
{
    shared_ptr<CCoin> pInstance = make_shared<CCoin>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CCoin");

    return pInstance;
}
