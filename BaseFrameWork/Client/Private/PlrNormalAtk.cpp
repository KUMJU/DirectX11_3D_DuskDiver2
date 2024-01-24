#include "pch.h"
#include "PlrNormalAtk.h"
#include "Collider.h"

#include "GameInstance.h"

CPlrNormalAtk::CPlrNormalAtk()
{
}

HRESULT CPlrNormalAtk::Initialize(_uint _iComboNum)
{
    CCollider::COLLIDER_DESC normalAtkDesc = {};
    normalAtkDesc.fRadius = 0.4f;
    normalAtkDesc.vCenter = { 0.f, 0.7f, 0.5f };

    m_bKnokBack = true;
    m_fSkillDuration = 0.5f;
    m_fKnockBackDistance = 4.f;
    m_bDownAtk = false;

    if (3 == _iComboNum) {
        normalAtkDesc.fRadius = 0.6f;
        m_bKnokBack = true;
        m_fStartDelayTime = 0.3f;

    }
    if (4 == _iComboNum) {
        normalAtkDesc.fRadius = 0.6f;
        m_fStartDelayTime = 0.3f;
        m_bKnokUp = true;
        m_fKnockUpDistance = 3.f;

    }
    else if (5 == _iComboNum) {

        normalAtkDesc.fRadius = 0.6f;

        m_fKnockBackDistance = 1.5f;
        m_fSkillDuration = 1.4f;
        m_fStartDelayTime = 0.8f;

        m_bDownAtk = true;
    }

    shared_ptr<CCollider> pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_SPHERE, normalAtkDesc);
    pCollider->SetOwner(shared_from_this());
    m_bCancle = true;

    m_Colliders.push_back(pCollider);

    return S_OK;
}

void CPlrNormalAtk::PriorityTick(_float _fTimeDelta)
{
}

void CPlrNormalAtk::Tick(_float _fTimeDelta)
{
    __super::Tick(_fTimeDelta);

}

void CPlrNormalAtk::LateTick(_float _fTimeDelta)
{
}

HRESULT CPlrNormalAtk::Render()
{
    __super::Render();

    return S_OK;
}

shared_ptr<CPlrNormalAtk> CPlrNormalAtk::Create(_uint _iComboNum )
{
    shared_ptr<CPlrNormalAtk> pInstance = make_shared<CPlrNormalAtk>();

    if (FAILED(pInstance->Initialize(_iComboNum)))
        MSG_BOX("Failed to Create : CPlrNormalAtk");

    return pInstance;
}
