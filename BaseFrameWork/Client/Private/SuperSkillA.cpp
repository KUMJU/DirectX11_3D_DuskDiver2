#include "pch.h"
#include "SuperSkillA.h"
#include "Collider.h"
#include "GameInstance.h"

#include "EffectMgr.h"
#include "EffectPreset.h"

CSuperSkillA::CSuperSkillA()
{
}

HRESULT CSuperSkillA::Initialize()
{
    __super::Initialize();

    m_eSkillOwner = EOWNER_TYPE::OWNER_PLAYER;

    m_iDamage = 5.f;

    m_bMultiAtk = true;

    CCollider::COLLIDER_DESC AtkDesc = {};
    AtkDesc.fRadius = 2.5f;
    AtkDesc.vCenter = { 0.f, 1.5f, 1.8f };


    SKILLINFO info = {};

    info.bKnockUp = true;
    info.bDownAtk = false;
    info.fKnockUpDistance = 6.f;
    info.fWeight = 2.f;
    info.iStartTrackPosition = 45.0;
    info.iEndTrackPosition = 90.0;

    m_pEffectPreset = CEffectMgr::GetInstance()->FindEffect(TEXT("SuperSkill1"));
    m_pParticlePreset = CEffectMgr::GetInstance()->FindEffect(TEXT("SuperParticle"));

    m_pEffectPreset->SetDistortionOption(true);

    CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_Effect"), m_pParticlePreset);

    shared_ptr<CCollider> pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_SPHERE, AtkDesc);
    pCollider->SetOwner(shared_from_this());
    m_bCancle = false;

    m_Collider = pCollider;

    m_Infos.push_back(info);
    

    return S_OK;
}

void CSuperSkillA::PriorityTick(_float _fTimeDelta)
{
}

void CSuperSkillA::Tick(_float _fTimeDelta)
{
    __super::Tick(_fTimeDelta);

    m_fAccTime += _fTimeDelta;

    if (m_fAccTime >= 1.53f && !bVoiceDone) {

        bVoiceDone = true;
        CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_PLR_FX);
        CGameInstance::GetInstance()->PlayAudio(TEXT("se_HE01_Super01_1.wav"), CSoundMgr::CHANNELID::CH_PLR_FX, 1.f);

    }

    if (m_fAccTime >= 2.3f && !bSkillSeDone) {

        bSkillSeDone = true;
        CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_PLR_VO);
        CGameInstance::GetInstance()->PlayAudio(TEXT("Hero01_ba_62.wav"), CSoundMgr::CHANNELID::CH_PLR_VO, 1.f);

        CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_PLR_FX);
        CGameInstance::GetInstance()->PlayAudio(TEXT("se_HE01_Super02_2.wav"), CSoundMgr::CHANNELID::CH_PLR_FX, 1.f);

    }

    if (m_fAccTime >= 3.6f && !m_IsSkillFinSe) {

        m_IsSkillFinSe = true;

        CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_PLR_FX);
        CGameInstance::GetInstance()->PlayAudio(TEXT("se_HE01_Super02_3.wav"), CSoundMgr::CHANNELID::CH_PLR_FX, 1.f);

    }

}

void CSuperSkillA::LateTick(_float _fTimeDelta)
{
    __super::LateTick(_fTimeDelta);

}

HRESULT CSuperSkillA::Render()
{
    __super::Render();
    return S_OK;
}

void CSuperSkillA::EndSkill()
{
    m_IsSkillFinSe = false;
    m_pParticlePreset->PlayEffect();
}

shared_ptr<CSuperSkillA> CSuperSkillA::Create()
{
    shared_ptr<CSuperSkillA> pInstance = make_shared<CSuperSkillA>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CSuperSkillA");

    return pInstance;
}
