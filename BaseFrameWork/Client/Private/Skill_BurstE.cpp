#include "pch.h"
#include "Skill_BurstE.h"

#include "Collider.h"
#include "GameInstance.h"

CSkill_BurstE::CSkill_BurstE()
{
}

HRESULT CSkill_BurstE::Initialize()
{

    //일반공격 1  + 넉업공격 2~3번?
    __super::Initialize();

    CCollider::COLLIDER_DESC normalAtkDesc = {};
    normalAtkDesc.fRadius = 0.8f;
    normalAtkDesc.vCenter = { 0.f, 0.8f, 0.8f };

    ////////////info 1 /////////
    SKILLINFO info = {};

    info.bKnockUp = false;
    info.fKnockUpDistance = 5.f;
    info.fWeight = 2.f;
    info.iStartTrackPosition = 21.f;
    info.iEndTrackPosition = 28.f;

    m_bMultiAtk = false;

    SKILLINFO info2 = {};

    info2.bKnockUp = true;
    info2.fKnockUpDistance = 10.f;
    info2.fWeight = 2.f;
    info2.iStartTrackPosition = 30.f;
    info2.iEndTrackPosition = 41.f;

    shared_ptr<CCollider> pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_SPHERE, normalAtkDesc);
    pCollider->SetOwner(shared_from_this());
    m_bCancle = false;

    m_Collider = pCollider;

    m_Infos.push_back(info);
    m_Infos.push_back(info2);

    return S_OK;
}

void CSkill_BurstE::PriorityTick(_float _fTimeDelta)
{
}

void CSkill_BurstE::Tick(_float _fTimeDelta)
{
}

void CSkill_BurstE::LateTick(_float _fTimeDelta)
{
}

HRESULT CSkill_BurstE::Render()
{
    return E_NOTIMPL;
}

shared_ptr<CSkill_BurstE> CSkill_BurstE::Create()
{
    return shared_ptr<CSkill_BurstE>();
}
