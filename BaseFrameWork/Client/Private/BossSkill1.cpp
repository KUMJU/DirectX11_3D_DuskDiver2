#include "pch.h"
#include "BossSkill1.h"

#include "Collider.h"
#include "GameInstance.h"

#include "EffectMgr.h"

CBossSkill1::CBossSkill1()
{
}

HRESULT CBossSkill1::Initialize()
{

    CCollider::COLLIDER_DESC normalAtkDesc = {};
    SKILLINFO skillDesc = {};

    m_eSkillOwner = EOWNER_TYPE::OWNER_MONSTER;
    m_pEffectPreset = CEffectMgr::GetInstance()->FindEffect(TEXT("BossAtk1"));

    m_iDamage = 15.f;

    normalAtkDesc.fRadius = 17.f;
    normalAtkDesc.vCenter = { 0.f, 0.f, 2.f };

    skillDesc.bKnockUp = false;
    skillDesc.bDownAtk = true;
    skillDesc.fWeight = 1.f;

    skillDesc.fKnockUpDistance = 0.f;
    skillDesc.fDropSpeed = 0.f;


    skillDesc.iStartTrackPosition = 40.0;
    skillDesc.iEndTrackPosition = 50.0;

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

    m_fAccTime += _fTimeDelta;

    if (m_fAccTime >= 1.3f && !bSkillSeDone) {

        bSkillSeDone = true;

        CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_MON_SE);
        CGameInstance::GetInstance()->PlayAudio(TEXT("se_EN0301_attack1_2.wav"), CSoundMgr::CHANNELID::CH_MON_SE, 1.f);
       
    }

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
