#include "pch.h"
#include "AirAtk.h"

#include "Collider.h"
#include "GameInstance.h"

CAirAtk::CAirAtk()
{
}

HRESULT CAirAtk::Initialize(_uint _iComboNum)
{
    m_eSkillOwner = EOWNER_TYPE::OWNER_PLAYER;
    m_IsBasicCombat = true;
    m_iDamage = 10;

    CCollider::COLLIDER_DESC normalAtkDesc = {};
    normalAtkDesc.fRadius = 0.7f;
    normalAtkDesc.vCenter = { 0.f, 0.5f, 0.4f };

    m_bMultiAtk = false;

    SKILLINFO skillDesc = {};
    skillDesc.iStartTrackPosition = 1.0;
    skillDesc.iEndTrackPosition = 9.0;

    if (1 == _iComboNum) {
        skillDesc.bKnockUp = true;
        skillDesc.fWeight = 1.5f;
        skillDesc.fKnockUpDistance = 4.f;


    }
    else if (2 == _iComboNum) {

        skillDesc.bKnockUp = true;
        skillDesc.fWeight = 1.5f;
        skillDesc.fKnockUpDistance = 4.f;

    }
    else if (3 == _iComboNum) {

        skillDesc.iStartTrackPosition = 7.0;
        skillDesc.iEndTrackPosition = 15.0;

        skillDesc.bKnockUp = true;
        skillDesc.fWeight = 1.5f;
        skillDesc.fKnockUpDistance = 4.f;

        normalAtkDesc.fRadius = 0.8f;
        skillDesc.bDownAtk = true;

    }
    else if (4 == _iComboNum) {

        skillDesc.iStartTrackPosition = 7.0;
        skillDesc.iEndTrackPosition = 15.0;

        skillDesc.bKnockUp = true;
        skillDesc.fWeight = 1.5f;
        skillDesc.fKnockUpDistance = 4.f;

        normalAtkDesc.fRadius = 0.8f;
        skillDesc.bDownAtk = true;


    }

    shared_ptr<CCollider> pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_SPHERE, normalAtkDesc);
    pCollider->SetOwner(shared_from_this());
    m_bCancle = true;

    m_Collider = pCollider;

    m_Infos.push_back(skillDesc);
    return S_OK;
}

void CAirAtk::PriorityTick(_float _fTimeDelta)
{
}

void CAirAtk::Tick(_float _fTimeDelta)
{
    __super::Tick(_fTimeDelta);
}

void CAirAtk::LateTick(_float _fTimeDelta)
{
}

HRESULT CAirAtk::Render()
{
    __super::Render();
    return S_OK;
}

shared_ptr<CAirAtk> CAirAtk::Create(_uint _iComboNum)
{
    shared_ptr<CAirAtk> pInstance = make_shared<CAirAtk>();

    if (FAILED(pInstance->Initialize(_iComboNum)))
        MSG_BOX("Failed to Create : CAirAtk");

    return pInstance;
}
