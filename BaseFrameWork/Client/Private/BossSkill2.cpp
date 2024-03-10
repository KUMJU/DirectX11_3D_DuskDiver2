#include "pch.h"
#include "BossSkill2.h"

#include "Collider.h"
#include "GameInstance.h"

#include "GroundCrack.h"

CBossSkill2::CBossSkill2()
{
}

HRESULT CBossSkill2::Initialize()
{

    CCollider::COLLIDER_DESC normalAtkDesc = {};
    SKILLINFO skillDesc = {};

    m_eSkillOwner = EOWNER_TYPE::OWNER_MONSTER;
    m_pEffectPreset = CEffectMgr::GetInstance()->FindEffect(TEXT("BossAtk2"));

    m_iDamage = 7.f;

    normalAtkDesc.fRadius = 4.f;
    normalAtkDesc.vCenter = { 0.f, 0.f, 9.f };

    skillDesc.bKnockUp = false;
    skillDesc.bDownAtk = true;
    skillDesc.fWeight = 1.f;

    skillDesc.fKnockUpDistance = 0.f;
    skillDesc.fDropSpeed = 0.f;


    skillDesc.iStartTrackPosition = 31.0;
    skillDesc.iEndTrackPosition = 40.0;

    shared_ptr<CCollider> pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_SPHERE, normalAtkDesc);
    pCollider->SetOwner(shared_from_this());
    m_bCancle = true;

    m_Collider = pCollider;
    m_Infos.push_back(skillDesc);

    m_pGroundCrack = CGroundCrack::Create();
    m_pGroundCrack->SetEnable(false);
    m_pGroundCrack->SetPosition({ 0.f, 39.5f, -410.f, 1.f });
    CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_GameObject"), m_pGroundCrack);

    return S_OK;
}

void CBossSkill2::PriorityTick(_float _fTimeDelta)
{
}

void CBossSkill2::Tick(_float _fTimeDelta)
{
    m_fAccTime += _fTimeDelta;

    if (m_fAccTime >= 1.5f && !bSkillSeDone) {

        bSkillSeDone = true;

        CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_MON_SE);
        CGameInstance::GetInstance()->PlayAudio(TEXT("se_EN0301_attack2_3.wav"), CSoundMgr::CHANNELID::CH_MON_SE, 1.f);

        m_pGroundCrack->ActiveSkill();

    }

    __super::Tick(_fTimeDelta);

}

void CBossSkill2::LateTick(_float _fTimeDelta)
{
    __super::LateTick(_fTimeDelta);

}

HRESULT CBossSkill2::Render()
{
    __super::Render();
    return S_OK;
}

shared_ptr<CBossSkill2> CBossSkill2::Create()
{
    shared_ptr<CBossSkill2> pInstance = make_shared<CBossSkill2>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CBossSkill2");

    return pInstance;
}
