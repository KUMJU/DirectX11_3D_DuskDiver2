#include "pch.h"
#include "BossSkill5.h"
#include "Collider.h"
#include "GameInstance.h"

CBossSkill5::CBossSkill5()
{
}

HRESULT CBossSkill5::Initialize()
{

    CCollider::COLLIDER_DESC normalAtkDesc = {};
    SKILLINFO skillDesc = {};

    m_eSkillOwner = EOWNER_TYPE::OWNER_MONSTER;

    m_iDamage = 15.f;

    normalAtkDesc.fRadius = 10.f;
    normalAtkDesc.vCenter = { 0.f, 0.f, 0.f };

    skillDesc.bKnockUp = false;
    skillDesc.bDownAtk = true;
    skillDesc.fWeight = 1.f;

    skillDesc.fKnockUpDistance = 0.f;
    skillDesc.fDropSpeed = 0.f;


    skillDesc.iStartTrackPosition = 30.0;
    skillDesc.iEndTrackPosition = 45.0;

    shared_ptr<CCollider> pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_SPHERE, normalAtkDesc);
    pCollider->SetOwner(shared_from_this());
    m_bCancle = true;

    m_Collider = pCollider;
    m_Infos.push_back(skillDesc);

    return S_OK;
}

void CBossSkill5::PriorityTick(_float _fTimeDelta)
{
}

void CBossSkill5::Tick(_float _fTimeDelta)
{
    m_fAccTime += _fTimeDelta;

    if (m_fAccTime >= 0.7f && !bSkillSeDone) {

        bSkillSeDone = true;

        CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_MON_SE);
        CGameInstance::GetInstance()->PlayAudio(TEXT("se_EN0301_attack3_1.wav"), CSoundMgr::CHANNELID::CH_MON_SE, 1.f);


    }


    __super::Tick(_fTimeDelta);

}

void CBossSkill5::LateTick(_float _fTimeDelta)
{
    __super::LateTick(_fTimeDelta);

}

HRESULT CBossSkill5::Render()
{
    __super::Render();
    return S_OK;
}

shared_ptr<CBossSkill5> CBossSkill5::Create()
{
    shared_ptr<CBossSkill5> pInstance = make_shared<CBossSkill5>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CBossSkill5");

    return pInstance;
}
