#include "pch.h"
#include "Skill_BurstE.h"

#include "Collider.h"
#include "GameInstance.h"
#include "CameraMgr.h"

#include "EffectPreset.h"

CSkill_BurstE::CSkill_BurstE()
{
}

HRESULT CSkill_BurstE::Initialize()
{
    __super::Initialize();

    m_iDamage = 15;

    m_eSkillOwner = EOWNER_TYPE::OWNER_PLAYER;
    m_pEffectPreset = CEffectMgr::GetInstance()->FindEffect(TEXT("BurstE"));

    m_pParticlePreset = CEffectMgr::GetInstance()->FindAndCloneEffect(TEXT("UpParticleBase2"));
    m_pParticlePreset->SetEnable(false);
    m_pParticlePreset->SetParticleGlowColor({ 1.f, 1.f, 0.1f, 1.f });

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
    info.CancleAbleRatio = 1.0;

    m_bMultiAtk = false;

    SKILLINFO info2 = {};

    info2.bKnockUp = true;
    info2.fKnockUpDistance = 5.f;
    info2.fWeight = 2.f;
    info2.iStartTrackPosition = 20.f;
    info2.iEndTrackPosition = 24.f;
    info2.CancleAbleRatio = 1.0;


    SKILLINFO info3 = {};

    info3.bKnockUp = true;
    info3.fKnockUpDistance = 5.f;
    info3.fWeight = 2.f;
    info3.iStartTrackPosition = 26.f;
    info3.iEndTrackPosition = 30.f;
    info3.CancleAbleRatio = 1.0;

    SKILLINFO info4 = {};

    info4.bKnockUp = true;
    info4.fKnockUpDistance = 5.f;
    info4.fWeight = 2.f;
    info4.iStartTrackPosition = 35.f;
    info4.iEndTrackPosition = 38.f;
    info4.CancleAbleRatio = 1.0;

    SKILLINFO info5 = {};

    info5.bKnockUp = true;
    info5.fKnockUpDistance = 11.f;
    info5.fWeight = 2.f;
    info5.iStartTrackPosition = 40.f;
    info5.iEndTrackPosition = 45.f;
    info5.CancleAbleRatio = 1.0;


    SKILLINFO info6 = {};

    info6.bKnockUp = true;
    info6.bDownAtk = false;
    info6.fKnockUpDistance = 16.f;
    info6.fWeight = 2.f;
    info6.iStartTrackPosition = 48.f;
    info6.iEndTrackPosition = 51.f;
    info6.CancleAbleRatio = 0.8;
    info6.strHitSound = TEXT("se_HE01_Skill04_3.wav");
    info6.fHitVolume = 1.7f;

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
    m_fAccTime += _fTimeDelta;

    if (m_fAccTime >= 0.7f && !bSkillSeDone) {

        CCameraMgr::GetInstance()->SetBattleZoom(2.f, 80.f);
        bSkillSeDone = true;

        CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_PLR_FX);
        CGameInstance::GetInstance()->PlayAudio(TEXT("se_HE03_Super02_2.wav"), CSoundMgr::CHANNELID::CH_PLR_FX, 1.f);
    }

    if (m_fAccTime >= 0.9f && !m_bParticleDone) {

        m_bParticleDone = true;

        m_pParticlePreset->SetEffectPosition(m_pOwnerTransform->GetState(CTransform::STATE_POSITION));
        m_pParticlePreset->PlayEffect();
    }



    if (m_fAccTime >= 1.6f && !bVoiceDone) {

        bVoiceDone = true;
  /*      CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_PLR_FX);
        CGameInstance::GetInstance()->PlayAudio(TEXT("se_HE01_Skill06_2.wav"), CSoundMgr::CHANNELID::CH_PLR_FX, 1.f);*/

        CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_PLR_VO);
        CGameInstance::GetInstance()->PlayAudio(TEXT("Hero01_ba_27.wav"), CSoundMgr::CHANNELID::CH_PLR_VO, 1.f);

    }

    if (m_fAccTime >= 1.8f && !m_bCamDone) {

        CCameraMgr::GetInstance()->SetBattleZoom(0.4f, 85.f);

        m_bCamDone = true;
    
    }


    __super::Tick(_fTimeDelta);

}

void CSkill_BurstE::LateTick(_float _fTimeDelta)
{
    __super::LateTick(_fTimeDelta);
}

HRESULT CSkill_BurstE::Render()
{
    __super::Render();

    return S_OK;
}

void CSkill_BurstE::EndSkill()
{
    m_bCamDone = false;
    m_bParticleDone = false;
}

shared_ptr<CSkill_BurstE> CSkill_BurstE::Create()
{
    shared_ptr<CSkill_BurstE> pInstance = make_shared<CSkill_BurstE>();

    if (FAILED(pInstance->Initialize())) {
        MSG_BOX("Failed to Create : CSkill_BurstE");
    }

    return pInstance;
}
