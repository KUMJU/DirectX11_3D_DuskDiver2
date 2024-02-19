#include "pch.h"
#include "BossSkill4.h"

#include "Collider.h"
#include "GameInstance.h"
#include "EffectMgr.h"

CBossSkill4::CBossSkill4()
{
}

HRESULT CBossSkill4::Initialize()
{

    CCollider::COLLIDER_DESC normalAtkDesc = {};
    SKILLINFO skillDesc = {};

    m_eSkillOwner = EOWNER_TYPE::OWNER_MONSTER;

    m_iDamage = 15.f;

//   m_pEffectPreset = CEffectMgr::GetInstance()->FindEffect(TEXT("BossAtk4"));

    normalAtkDesc.vRadians = {XMConvertToRadians(7.f), 0.f, 0.f};
    normalAtkDesc.vExtents = { 2.f, 2.f, 50.f };
    normalAtkDesc.fRadius = 10.f;
    normalAtkDesc.vCenter = { 0.f, 5.f, -0.6f };

    skillDesc.bKnockUp = false;
    skillDesc.bDownAtk = true;
    skillDesc.fWeight = 1.f;

    skillDesc.fKnockUpDistance = 0.f;
    skillDesc.fDropSpeed = 0.f;


    skillDesc.iStartTrackPosition = 5.0;
    skillDesc.iEndTrackPosition = 20.0;

    shared_ptr<CCollider> pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_OBB, normalAtkDesc);
    pCollider->SetOwner(shared_from_this());
    m_bCancle = true;

    m_Collider = pCollider;
    m_Infos.push_back(skillDesc);

    return S_OK;
}

void CBossSkill4::PriorityTick(_float _fTimeDelta)
{
}

void CBossSkill4::Tick(_float _fTimeDelta)
{
    __super::Tick(_fTimeDelta);

}

void CBossSkill4::LateTick(_float _fTimeDelta)
{
    __super::LateTick(_fTimeDelta);

}

HRESULT CBossSkill4::Render()
{
    __super::Render();
    return S_OK;
}

shared_ptr<CBossSkill4> CBossSkill4::Create()
{
    shared_ptr<CBossSkill4> pInstance = make_shared<CBossSkill4>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CBossSkill4");

    return pInstance;
}
