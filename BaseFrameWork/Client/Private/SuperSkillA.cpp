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
    m_pParticlePreset->PlayEffect();
}

shared_ptr<CSuperSkillA> CSuperSkillA::Create()
{
    shared_ptr<CSuperSkillA> pInstance = make_shared<CSuperSkillA>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CSuperSkillA");

    return pInstance;
}
