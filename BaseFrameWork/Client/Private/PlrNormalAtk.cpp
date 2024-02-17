#include "pch.h"
#include "PlrNormalAtk.h"
#include "Collider.h"

#include "GameInstance.h"

CPlrNormalAtk::CPlrNormalAtk()
{
}

HRESULT CPlrNormalAtk::Initialize(_uint _iComboNum)
{

    m_eSkillOwner = EOWNER_TYPE::OWNER_PLAYER;
    m_IsBasicCombat = true;

    CCollider::COLLIDER_DESC normalAtkDesc = {};
    normalAtkDesc.fRadius = 0.4f;
    normalAtkDesc.vCenter = { 0.f, 0.7f, 0.5f };

    m_iDamage = 10;

    m_bMultiAtk = false;

    SKILLINFO skillDesc = {};

    skillDesc.bDownAtk = false;
    skillDesc.fWeight = 1.f;

    skillDesc.iStartTrackPosition = 5.0;
    skillDesc.iEndTrackPosition = 25.0;
    skillDesc.CancleAbleRatio = 0.0;

    if (3 == _iComboNum) {
        normalAtkDesc.fRadius = 0.6f;
    }
    if (4 == _iComboNum) {
        normalAtkDesc.fRadius = 0.6f;
        skillDesc.bKnockUp = true;
        skillDesc.fWeight = 1.5f;
        skillDesc.fKnockUpDistance =15.f;


    }
    else if (5 == _iComboNum) {

        skillDesc.iStartTrackPosition = 28.0;
        skillDesc.iEndTrackPosition = 55.0;

        normalAtkDesc.fRadius = 0.6f;
        skillDesc.bDownAtk = true;

    }
    //Heavy Combo
    else if (6 == _iComboNum) {

        skillDesc.iStartTrackPosition = 35.0;
        skillDesc.iEndTrackPosition = 45.0;
        skillDesc.bKnockUp = true;
        skillDesc.fWeight = 1.5f;
        skillDesc.fKnockUpDistance = 15.f;

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

shared_ptr<CPlrNormalAtk> CPlrNormalAtk::Create(_uint _iComboNum)
{
    shared_ptr<CPlrNormalAtk> pInstance = make_shared<CPlrNormalAtk>();

    if (FAILED(pInstance->Initialize(_iComboNum)))
        MSG_BOX("Failed to Create : CPlrNormalAtk");

    return pInstance;
}
