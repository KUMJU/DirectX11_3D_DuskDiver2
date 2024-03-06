#include "pch.h"
#include "Skill_BurstQ.h"
#include "Collider.h"
#include "GameInstance.h"

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

    if (m_fAccTime >= 0.7f && !bSkillSeDone) {

        bSkillSeDone = true;

        CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_PLR_FX);
        CGameInstance::GetInstance()->PlayAudio(TEXT("se_HE03_Super02_2.wav"), CSoundMgr::CHANNELID::CH_PLR_FX, 1.f);

    }

    if (m_fAccTime >= 1.6f && !bVoiceDone) {

        bVoiceDone = true;
        CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_PLR_FX);
        CGameInstance::GetInstance()->PlayAudio(TEXT("se_HE01_Skill06_2.wav"), CSoundMgr::CHANNELID::CH_PLR_FX, 1.f);

        CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_PLR_VO);
        CGameInstance::GetInstance()->PlayAudio(TEXT("Hero01_ba_27.wav"), CSoundMgr::CHANNELID::CH_PLR_VO, 1.f);

    }



    //se_HE01_Skill05_5
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

shared_ptr<CSkill_BurstQ> CSkill_BurstQ::Create()
{
    shared_ptr<CSkill_BurstQ> pInstance = make_shared<CSkill_BurstQ>();

    if (FAILED(pInstance->Initialize())) {
        MSG_BOX("Failed to Create : CSkill_BurstQ");
    }

	return pInstance;
}
