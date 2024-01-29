#include "pch.h"
#include "Skill_BurstE.h"

#include "Collider.h"
#include "GameInstance.h"

CSkill_BurstE::CSkill_BurstE()
{
}

HRESULT CSkill_BurstE::Initialize()
{
    __super::Initialize();

    m_eSkillOwner = EOWNER_TYPE::OWNER_PLAYER;


    CCollider::COLLIDER_DESC normalAtkDesc = {};
    normalAtkDesc.fRadius = 1.f;
    normalAtkDesc.vCenter = { 0.f, 0.8f, 0.f };

    ////////////info 1 /////////
    SKILLINFO info = {};

    info.bKnockUp = false;
    info.fKnockUpDistance = 0.f;
    info.fWeight = 1.f;
    info.iStartTrackPosition = 3.f;
    info.iEndTrackPosition = 15.f;

    m_bMultiAtk = false;

    SKILLINFO info2 = {};

    info2.bKnockUp = true;
    info2.fKnockUpDistance = 5.f;
    info2.fWeight = 2.f;
    info2.iStartTrackPosition = 20.f;
    info2.iEndTrackPosition = 24.f;

    SKILLINFO info3 = {};

    info3.bKnockUp = true;
    info3.fKnockUpDistance = 5.f;
    info3.fWeight = 2.f;
    info3.iStartTrackPosition = 26.f;
    info3.iEndTrackPosition = 30.f;

    SKILLINFO info4 = {};

    info4.bKnockUp = true;
    info4.fKnockUpDistance = 5.f;
    info4.fWeight = 2.f;
    info4.iStartTrackPosition = 35.f;
    info4.iEndTrackPosition = 38.f;

    SKILLINFO info5 = {};

    info5.bKnockUp = true;
    info5.fKnockUpDistance = 11.f;
    info5.fWeight = 2.f;
    info5.iStartTrackPosition = 40.f;
    info5.iEndTrackPosition = 45.f;


    SKILLINFO info6 = {};

    info6.bKnockUp = true;
    info6.bDownAtk = false;
    info6.fKnockUpDistance = 16.f;
    info6.fWeight = 2.f;
    info6.iStartTrackPosition = 48.f;
    info6.iEndTrackPosition = 53.f;


    shared_ptr<CCollider> pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_SPHERE, normalAtkDesc);
    pCollider->SetOwner(shared_from_this());
    m_bCancle = false;

    m_Collider = pCollider;

    m_Infos.push_back(info);
    m_Infos.push_back(info2);
    m_Infos.push_back(info3);
    m_Infos.push_back(info4);
    m_Infos.push_back(info5);
    m_Infos.push_back(info6);


    return S_OK;
}

void CSkill_BurstE::PriorityTick(_float _fTimeDelta)
{
}

void CSkill_BurstE::Tick(_float _fTimeDelta)
{
    __super::Tick(_fTimeDelta);

}

void CSkill_BurstE::LateTick(_float _fTimeDelta)
{
}

HRESULT CSkill_BurstE::Render()
{
    __super::Render();

    return S_OK;
}

shared_ptr<CSkill_BurstE> CSkill_BurstE::Create()
{
    shared_ptr<CSkill_BurstE> pInstance = make_shared<CSkill_BurstE>();

    if (FAILED(pInstance->Initialize())) {
        MSG_BOX("Failed to Create : CSkill_BurstE");
    }

    return pInstance;
}
