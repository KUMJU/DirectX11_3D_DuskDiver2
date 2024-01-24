#include "pch.h"
#include "SkillE.h"

#include "GameInstance.h"
#include "Collider.h"

CSkillE::CSkillE()
{
}

HRESULT CSkillE::Initialize()
{
    CCollider::COLLIDER_DESC normalAtkDesc = {};
    normalAtkDesc.fRadius = 0.5f;
    normalAtkDesc.vCenter = { 0.f, 0.8f, 0.7f };


    SKILLINFO info = {};

    info.fDelayTime = 0.1f;
    info.bKnockUp = true;
    info.bDownAtk = false;
    info.fSkillDuration = 0.6f;
    info.fKnockUpDistance = 5.f;
    info.fWeight = 2.f;




    SKILLINFO info1 = {};

    info1.fDelayTime = 0.2f;
    info1.bKnockUp = true;
    info1.bDownAtk = false;
    info1.fSkillDuration = 0.6f;
    info1.fKnockUpDistance = 5.f;
    info1.fWeight = 2.f;




    SKILLINFO info2 = {};

    info2.fDelayTime = 0.2f;
    info2.bKnockUp = true;
    info2.bDownAtk = false;
    info2.fSkillDuration = 0.7f;
    info2.fKnockUpDistance = 5.f;
    info2.fWeight = 2.f;



    SKILLINFO info3 = {};

    info3.fDelayTime = 0.6f;
    info3.bKnockUp = true;
    info3.bDownAtk = false;
    info3.fSkillDuration = 1.3f;
    info3.fKnockUpDistance = 11.f;
    info3.fWeight = 2.f;

    shared_ptr<CCollider> pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_SPHERE, normalAtkDesc);
    pCollider->SetOwner(shared_from_this());
    m_bCancle = false;

    m_Colliders.push_back(pCollider);

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
}

void CSkillE::LateTick(_float _fTimeDelta)
{
}

HRESULT CSkillE::Render()
{
    return S_OK;
}

shared_ptr<CSkillE> CSkillE::Create()
{
    shared_ptr<CSkillE> pInstance = make_shared<CSkillE>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CSkillE");

    return pInstance;
}
