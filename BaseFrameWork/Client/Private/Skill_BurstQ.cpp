#include "pch.h"
#include "Skill_BurstQ.h"
#include "Collider.h"
#include "GameInstance.h"

#include "CameraMgr.h"
#include "EffectPreset.h"

CSkill_BurstQ::CSkill_BurstQ()
{
}

HRESULT CSkill_BurstQ::Initialize()
{
    __super::Initialize();

    m_iDamage = 20;

    CCollider::COLLIDER_DESC normalAtkDesc = {};
    normalAtkDesc.fRadius = 0.6f;
    normalAtkDesc.vCenter = { 0.f, 0.8f, 0.7f };

    m_eSkillOwner = EOWNER_TYPE::OWNER_PLAYER;
    m_pEffectPreset = CEffectMgr::GetInstance()->FindEffect(TEXT("BurstQ1"));

    m_pParticlePreset = CEffectMgr::GetInstance()->FindAndCloneEffect(TEXT("ParticleBaseDirZ"));
    CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_Effect"), m_pParticlePreset);
    m_pParticlePreset->SetEnable(false);
    m_pParticlePreset->SetParticleGlowColor(_float4(1.f, 1.f, 0.3f, 1.f));

    SKILLINFO info = {};

    info.iStartTrackPosition = 7.0;
    info.iEndTrackPosition = 15.0;
    info.bKnockUp = false;
    info.bDownAtk = false;
    info.fKnockUpDistance = 0.f;
    info.fWeight = 1.f;
    info.CancleAbleRatio = 1.0;

    SKILLINFO info1 = {};

    info1.iStartTrackPosition = 6.0;
    info1.iEndTrackPosition = 10.0;
    info1.bKnockUp = false;
    info1.bDownAtk = false;
    info1.fKnockUpDistance = 0.f;
    info1.fWeight = 1.f;
    info1.CancleAbleRatio = 1.0;

    SKILLINFO info2 = {};

    info2.iStartTrackPosition = 22.0;
    info2.iEndTrackPosition = 50.0;
    info2.bKnockUp = false;
    info2.bDownAtk = true;
    info2.fWeight = 1.f;
    info2.CancleAbleRatio = 0.7f;



    shared_ptr<CCollider> pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_SPHERE, normalAtkDesc);
    pCollider->SetOwner(shared_from_this());
    m_bCancle = false;

    m_Collider = pCollider;

    m_Infos.push_back(info);
    m_Infos.push_back(info1);
     m_Infos.push_back(info2);

    return S_OK;
}

void CSkill_BurstQ::PriorityTick(_float _fTimeDelta)
{
}

void CSkill_BurstQ::Tick(_float _fTimeDelta)
{

    m_fAccTime += _fTimeDelta;

    if (m_fAccTime >= 0.65f && !bSkillSeDone) {

        bSkillSeDone = true;

        CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_PLR_FX);
        CGameInstance::GetInstance()->PlayAudio(TEXT("se_HE01_Skill07_2.wav"), CSoundMgr::CHANNELID::CH_PLR_FX, 1.5f);

    }

    if (m_fAccTime >= 2.f && !bVoiceDone) {

        bVoiceDone = true;
        CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_PLR_FX);
        CGameInstance::GetInstance()->PlayAudio(TEXT("se_HE01_Skill05_5.wav"), CSoundMgr::CHANNELID::CH_PLR_FX, 1.5f);

        CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_PLR_VO);
        CGameInstance::GetInstance()->PlayAudio(TEXT("Hero01_ba_43.wav"), CSoundMgr::CHANNELID::CH_PLR_VO, 1.5f);

    }


    if (m_fAccTime >= 2.4f && !m_bParticleDone) {


        CCameraMgr::GetInstance()->SetBattleZoom(0.3f, 30.f);


        m_bParticleDone = true;
  
        // _vector vLook = m_pOwnerTransform->GetState(CTransform::STATE_LOOK);
        //  _vector vParticlePos = m_pOwnerTransform->GetState(CTransform::STATE_POSITION);

        m_pParticlePreset->SetParentTransform(m_pOwnerTransform);

        //  m_pParticlePreset->SetEffectPosition(vParticlePos);
        m_pParticlePreset->PlayEffect();

    }

    __super::Tick(_fTimeDelta);

}

void CSkill_BurstQ::LateTick(_float _fTimeDelta)
{
    __super::LateTick(_fTimeDelta);

}

HRESULT CSkill_BurstQ::Render()
{
    __super::Render();

	return S_OK;
}

void CSkill_BurstQ::EndSkill()
{
    m_bParticleDone = false;

}

shared_ptr<CSkill_BurstQ> CSkill_BurstQ::Create()
{
    shared_ptr<CSkill_BurstQ> pInstance = make_shared<CSkill_BurstQ>();

    if (FAILED(pInstance->Initialize())) {
        MSG_BOX("Failed to Create : CSkill_BurstQ");
    }

	return pInstance;
}
