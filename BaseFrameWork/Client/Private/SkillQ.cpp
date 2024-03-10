#include "pch.h"
#include "SkillQ.h"
#include "Collider.h"

#include "GameInstance.h"

#include "EffectPreset.h"

#include "CameraMgr.h"

CSkillQ::CSkillQ()
{
}

HRESULT CSkillQ::Initialize()
{
    __super::Initialize();
    m_bCancle = true;

    m_iDamage = 15;

    m_pEffectPreset = CEffectMgr::GetInstance()->FindEffect(TEXT("SkillQ"));
    //m_pEffectPreset->SetParentTransform(m_pOwnerTransform);
    m_pEffectPreset->SetDistortionOption(true);

    CCollider::COLLIDER_DESC normalAtkDesc = {};
    normalAtkDesc.fRadius = 0.5f;
    normalAtkDesc.vCenter = { 0.f, 0.8f, 0.7f };

    m_eSkillOwner = EOWNER_TYPE::OWNER_PLAYER;

    m_pParticlePreset = CEffectMgr::GetInstance()->FindEffect(TEXT("ParticleBase"));
    CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_Effect"), m_pParticlePreset);

    SKILLINFO info = {};

    info.bKnockUp = true;
    info.bDownAtk = false;
    info.fKnockUpDistance = 15.f;
    info.fWeight = 2.f;
    info.iStartTrackPosition = 16.f;
    info.iEndTrackPosition = 35.f;
    info.CancleAbleRatio = 0.2;
    info.strHitSound = TEXT("se_HE01_Skill01_2.wav");
    info.fHitVolume = 1.4f;

    shared_ptr<CCollider> pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_SPHERE, normalAtkDesc);
    pCollider->SetOwner(shared_from_this());
    m_bCancle = false;

    m_Collider = pCollider;
    m_Infos.push_back(info);

	return S_OK;
}

void CSkillQ::PriorityTick(_float _fTimeDelta)
{

}

void CSkillQ::Tick(_float _fTimeDelta)
{
    __super::Tick(_fTimeDelta);

    m_fAccTime += _fTimeDelta;

    if (m_fAccTime >= 0.5f && !bVoiceDone) {

        bVoiceDone = true;
        CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_PLR_VO);
        CGameInstance::GetInstance()->PlayAudio(TEXT("Hero01_ba_57.wav"), CSoundMgr::CHANNELID::CH_PLR_VO, 1.f);
        
        CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_PLR_FX);
        CGameInstance::GetInstance()->PlayAudio(TEXT("se_HE01_Skill01_4.wav"), CSoundMgr::CHANNELID::CH_PLR_FX, 1.f);

    }

   
    if (m_fAccTime >= 0.6f && !m_bSetTimer) {

        CGameInstance::GetInstance()->SetTimerOffset(TEXT("Timer_60"), 0.5f);
        m_bSetTimer = true;


    }

    if (m_fAccTime >= 0.67f && !m_bFinTimer) {
        CGameInstance::GetInstance()->SetTimerOffset(TEXT("Timer_60"), 1.f);
        CCameraMgr::GetInstance()->SetBattleZoom(0.15f, 90.f);

        _vector vLook = m_pOwnerTransform->GetState(CTransform::STATE_LOOK);
        _vector vParticlePos = m_pOwnerTransform->GetState(CTransform::STATE_POSITION) + vLook + _vector({ 0.f, 0.5f, 0.f });

        m_pParticlePreset->SetEffectPosition(vParticlePos);
        m_pParticlePreset->PlayEffect();
        m_bFinTimer = true;
    }


}

void CSkillQ::LateTick(_float _fTimeDelta)
{
    __super::LateTick(_fTimeDelta);

}

HRESULT CSkillQ::Render()
{
    __super::Render();

	return S_OK;
}

void CSkillQ::EndSkill()
{

}


shared_ptr<CSkillQ> CSkillQ::Create()
{
    shared_ptr<CSkillQ> pInstance = make_shared<CSkillQ>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CSkillQ");

    return pInstance;
}
