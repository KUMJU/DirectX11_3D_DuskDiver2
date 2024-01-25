#include "pch.h"
#include "SkillE.h"

#include "GameInstance.h"
#include "Collider.h"

CSkillE::CSkillE()
{
}

HRESULT CSkillE::Initialize()
{

    __super::Initialize();

    CCollider::COLLIDER_DESC normalAtkDesc = {};
    normalAtkDesc.fRadius = 1.f;
    normalAtkDesc.vCenter = { 0.f, 0.8f, 0.7f };


    SKILLINFO info = {};

    info.bKnockUp = true;
    info.bDownAtk = false;
    info.fKnockUpDistance = 4.f;
    info.fWeight = 2.f;
    info.iStartTrackPosition = 20.f;
    info.iEndTrackPosition = 25.f;

    SKILLINFO info1 = {};

    info1.bKnockUp = true;
    info1.bDownAtk = false;
    info1.fKnockUpDistance = 4.5f;
    info1.fWeight = 2.f;
    info1.iStartTrackPosition = 29.f;
    info1.iEndTrackPosition = 34.f;





    SKILLINFO info2 = {};

    info2.bKnockUp = true;
    info2.bDownAtk = false;
    info2.fKnockUpDistance = 4.5f;
    info2.fWeight = 2.f;

    info2.iStartTrackPosition = 37.f;
    info2.iEndTrackPosition = 42.f;



    SKILLINFO info3 = {};

    info3.bKnockUp = true;
    info3.bDownAtk = false;
    info3.fKnockUpDistance = 12.f;
    info3.fWeight = 2.f;
    info3.iStartTrackPosition = 50.f;
    info3.iEndTrackPosition = 60.f;

    shared_ptr<CCollider> pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_SPHERE, normalAtkDesc);
    pCollider->SetOwner(shared_from_this());
    m_bCancle = false;

    m_Collider =pCollider;

    m_Infos.push_back(info);
    m_Infos.push_back(info1);
    m_Infos.push_back(info2);
    m_Infos.push_back(info3);

    return S_OK;
}

void CSkillE::PriorityTick(_float _fTimeDelta)
{
}

void CSkillE::Tick(_float _fTimeDelta)
{
    __super::Tick(_fTimeDelta);

}

void CSkillE::LateTick(_float _fTimeDelta)
{
}

HRESULT CSkillE::Render()
{
    __super::Render();

    return S_OK;
}

shared_ptr<CSkillE> CSkillE::Create()
{
    shared_ptr<CSkillE> pInstance = make_shared<CSkillE>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CSkillE");

    return pInstance;
}
