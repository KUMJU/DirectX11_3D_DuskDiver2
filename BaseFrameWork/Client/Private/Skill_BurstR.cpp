#include "pch.h"
#include "Skill_BurstR.h"

#include "Collider.h"
#include "GameInstance.h"

#include "CameraMgr.h"

CSkill_BurstR::CSkill_BurstR()
{
}

HRESULT CSkill_BurstR::Initialize()
{
    m_eSkillOwner = EOWNER_TYPE::OWNER_PLAYER;

    m_bMultiAtk = false;
    m_pEffectPreset = CEffectMgr::GetInstance()->FindEffect(TEXT("BurstR"));


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


    shared_ptr<CCollider> pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_SPHERE, normalAtkDesc);
    pCollider->SetOwner(shared_from_this());
    m_bCancle = false;

    m_Collider = pCollider;

    m_Infos.push_back(info);
    m_Infos.push_back(info2);



    return S_OK;
}

void CSkill_BurstR::PriorityTick(_float _fTimeDelta)
{
}

void CSkill_BurstR::Tick(_float _fTimeDelta)
{

    m_fAccTime += _fTimeDelta;
    if (!m_bShaking) {

        if (m_fAccTime >= 2.5f) {
            CCameraMgr::GetInstance()->SetShakingMode(15.f, 0.1f, true);
            m_bShaking = true;
        }

    }


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
