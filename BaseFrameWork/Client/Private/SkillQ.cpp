#include "pch.h"
#include "SkillQ.h"
#include "Collider.h"

#include "GameInstance.h"

#include "EffectPreset.h"

CSkillQ::CSkillQ()
{
}

HRESULT CSkillQ::Initialize()
{
    __super::Initialize();
    m_bCancle = true;

    m_iDamage = 15;

    m_pEffectPreset = CEffectMgr::GetInstance()->FindEffect(TEXT("SkillStart"));
   // m_pEffectPreset->SetParentTransform(m_pOwnerTransform);

    CCollider::COLLIDER_DESC normalAtkDesc = {};
    normalAtkDesc.fRadius = 0.5f;
    normalAtkDesc.vCenter = { 0.f, 0.8f, 0.7f };

    m_eSkillOwner = EOWNER_TYPE::OWNER_PLAYER;


    SKILLINFO info = {};

    info.bKnockUp = true;
    info.bDownAtk = false;
    info.fKnockUpDistance = 15.f;
    info.fWeight = 2.f;
    info.iStartTrackPosition = 16.f;
    info.iEndTrackPosition = 35.f;
    info.CancleAbleRatio = 0.2;

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

shared_ptr<CSkillQ> CSkillQ::Create()
{
    shared_ptr<CSkillQ> pInstance = make_shared<CSkillQ>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CSkillQ");

    return pInstance;
}
