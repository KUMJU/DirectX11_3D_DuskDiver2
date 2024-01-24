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


    SKILLINFO skillDesc = {};

    skillDesc.bDownAtk = false;
    skillDesc.fSkillDuration = 0.5f;
    skillDesc.bDownAtk = false;
    skillDesc.fWeight = 1.f;


    if (3 == _iComboNum) {
        normalAtkDesc.fRadius = 0.6f;
        skillDesc.fDelayTime = 0.3f;
    }
    if (4 == _iComboNum) {
        normalAtkDesc.fRadius = 0.6f;
        skillDesc.bKnockUp = true;
        skillDesc.fDelayTime = 0.3f;
        skillDesc.fWeight = 0.7f;
        skillDesc.fKnockUpDistance = 10.f;

    }
    else if (5 == _iComboNum) {

        normalAtkDesc.fRadius = 0.6f;
        skillDesc.fSkillDuration = 1.4f;
        skillDesc.fDelayTime = 0.8f;
        skillDesc.bDownAtk = true;

    }

    shared_ptr<CCollider> pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_SPHERE, normalAtkDesc);
    pCollider->SetOwner(shared_from_this());
    m_bCancle = true;

    m_Colliders.push_back(pCollider);

    m_Infos.push_back(skillDesc);
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
