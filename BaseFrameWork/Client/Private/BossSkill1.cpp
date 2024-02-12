#include "pch.h"
#include "BossSkill1.h"

#include "Collider.h"
#include "GameInstance.h"

CBossSkill1::CBossSkill1()
{
}

HRESULT CBossSkill1::Initialize()
{

    CCollider::COLLIDER_DESC normalAtkDesc = {};
    SKILLINFO skillDesc = {};

    m_eSkillOwner = EOWNER_TYPE::OWNER_MONSTER;

    m_iDamage = 15.f;

    normalAtkDesc.fRadius = 10.f;
    normalAtkDesc.vCenter = { 0.f, 0.f, 2.f };

    skillDesc.bKnockUp = false;
    skillDesc.bDownAtk = true;
    skillDesc.fWeight = 1.f;

    skillDesc.fKnockUpDistance = 0.f;
    skillDesc.fDropSpeed = 0.f;


    skillDesc.iStartTrackPosition = 35.0;
    skillDesc.iEndTrackPosition = 55.0;

    shared_ptr<CCollider> pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_SPHERE, normalAtkDesc);
    pCollider->SetOwner(shared_from_this());
    m_bCancle = true;

    m_Collider = pCollider;
    m_Infos.push_back(skillDesc);

    return S_OK;
}

void CBossSkill1::PriorityTick(_float _fTimeDelta)
{
}

void CBossSkill1::Tick(_float _fTimeDelta)
{
    __super::Tick(_fTimeDelta);

}

void CBossSkill1::LateTick(_float _fTimeDelta)
{
    __super::LateTick(_fTimeDelta);
}

HRESULT CBossSkill1::Render()
{
    __super::Render();
    return S_OK;
}

shared_ptr<CBossSkill1> CBossSkill1::Create()
{
    shared_ptr<CBossSkill1> pInstance = make_shared<CBossSkill1>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CBossSkill1");

    return pInstance;
}
