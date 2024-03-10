#include "pch.h"
#include "Skill_BurstR.h"

#include "Collider.h"
#include "GameInstance.h"

#include "CameraMgr.h"
#include "GroundCrack.h"
#include "EffectPreset.h"

CSkill_BurstR::CSkill_BurstR()
{
}

HRESULT CSkill_BurstR::Initialize()
{
    m_eSkillOwner = EOWNER_TYPE::OWNER_PLAYER;

    m_bMultiAtk = false;
    m_pEffectPreset = CEffectMgr::GetInstance()->FindEffect(TEXT("BurstR"));

    m_pParticlePreset = CEffectMgr::GetInstance()->FindAndCloneEffect(TEXT("UpParticleBase"));
    m_pParticlePreset->SetEnable(false);
    m_pParticlePreset->SetParticleGlowColor({1.f, 1.f, 0.1f, 1.f});

    if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_Effect"), m_pParticlePreset)))
        return E_FAIL;

    m_iDamage = 20;

    CCollider::COLLIDER_DESC normalAtkDesc = {};
    normalAtkDesc.fRadius = 1.f;
    normalAtkDesc.vCenter = { 0.f, 0.6f, 0.f };

    SKILLINFO info = {};

    info.bKnockUp = true;
    info.fKnockUpDistance = 16.f;
    info.fWeight = 1.5f;
    info.iStartTrackPosition = 20.f;
    info.iEndTrackPosition = 30.f;
    info.CancleAbleRatio = 1.0;


    m_bMultiAtk = false;

    SKILLINFO info2 = {};

    info2.bKnockUp = false;
    info2.fKnockUpDistance = 0.f;
    info2.bDownAtk = false;
    info2.bDropAtk = true;
    info2.fWeight = 1.f;
    info2.iStartTrackPosition = 10.f;
    info2.iEndTrackPosition = 20.f;
    info2.CancleAbleRatio = 1.0;
    info2.strHitSound = TEXT("se_HE01_Skill04_4.wav");
    info2.fHitVolume = 1.7f;


    shared_ptr<CCollider> pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_SPHERE, normalAtkDesc);
    pCollider->SetOwner(shared_from_this());
    m_bCancle = false;

    m_Collider = pCollider;

    m_Infos.push_back(info);
    m_Infos.push_back(info2);

    m_pGroundCrack = CGroundCrack::Create();
    m_pGroundCrack->SetEnable(false);

    if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_Effect"), m_pGroundCrack)))
        return E_FAIL;

    return S_OK;
}

void CSkill_BurstR::PriorityTick(_float _fTimeDelta)
{
}

void CSkill_BurstR::Tick(_float _fTimeDelta)
{

    m_fAccTime += _fTimeDelta;

    if (m_fAccTime >= 1.f && !bSkillSeDone) {

        bSkillSeDone = true;

        CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_PLR_VO);
        CGameInstance::GetInstance()->PlayAudio(TEXT("Hero01_ba_42.wav"), CSoundMgr::CHANNELID::CH_PLR_VO, 1.f);


        CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_PLR_FX);
        CGameInstance::GetInstance()->PlayAudio(TEXT("se_HE01_Skill06_1.wav"), CSoundMgr::CHANNELID::CH_PLR_FX, 1.f);

    }

    if (m_fAccTime >= 2.3f && !bVoiceDone) {

        bVoiceDone = true;
        CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_PLR_FX);
        CGameInstance::GetInstance()->PlayAudio(TEXT("se_HE01_Skill06_2.wav"), CSoundMgr::CHANNELID::CH_PLR_FX, 1.f);

        CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_PLR_VO);
        CGameInstance::GetInstance()->PlayAudio(TEXT("Hero01_ba_47.wav"), CSoundMgr::CHANNELID::CH_PLR_VO, 1.f);

    }


    if (m_fAccTime >= 2.55f && !m_bSetTimer) {

        CCameraMgr::GetInstance()->SetBattleZoom(0.2f, 20.f);

        m_pGroundCrack->SetPosition(m_pOwnerTransform->GetState(CTransform::STATE_POSITION));
        m_pGroundCrack->ActiveSkill();

        m_pParticlePreset->SetEffectPosition(m_pOwnerTransform->GetState(CTransform::STATE_POSITION) - _vector({ 0.f, -1.f, 0.f, 0.f }));
        m_pParticlePreset->PlayEffect();

        m_bSetTimer = true;


    }

    //if (m_fAccTime >= 2.6f && !m_bFinTimer) {
    //    CGameInstance::GetInstance()->SetTimerOffset(TEXT("Timer_60"), 1.f);
    //    m_bFinTimer = true;
    //}



    __super::Tick(_fTimeDelta);
}

void CSkill_BurstR::LateTick(_float _fTimeDelta)
{
    __super::LateTick(_fTimeDelta);

}

HRESULT CSkill_BurstR::Render()
{
    __super::Render();

    return S_OK;
}

shared_ptr<CSkill_BurstR> CSkill_BurstR::Create()
{
    shared_ptr<CSkill_BurstR> pInstance = make_shared<CSkill_BurstR>();

    if (FAILED(pInstance->Initialize())) {
        MSG_BOX("Failed to Create : CSkill_BurstR");
    }

    return pInstance;
}
