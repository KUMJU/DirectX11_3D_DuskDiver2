#include "pch.h"
#include "SkillR.h"
#include "Collider.h"
#include "GameInstance.h"

CSkillR::CSkillR()
{
}

HRESULT CSkillR::Initialize()
{
    __super::Initialize();

    m_eSkillOwner = EOWNER_TYPE::OWNER_PLAYER;

    m_bCancle = true;

    CCollider::COLLIDER_DESC normalAtkDesc = {};
    normalAtkDesc.fRadius = 0.8f;
    normalAtkDesc.vCenter = { 0.f, 0.8f, 0.8f };

    ////////////info 1 /////////
    SKILLINFO info = {};

    info.bKnockUp = true;
    info.fKnockUpDistance = 5.f;
    info.fWeight = 2.f;
    info.iStartTrackPosition = 21.f;
    info.iEndTrackPosition = 28.f;
    info.CancleAbleRatio = 1.0;



    m_bMultiAtk = false;


    SKILLINFO info2= {};

    info2.bKnockUp = true;
    info2.fKnockUpDistance = 10.f;
    info2.fWeight = 2.f;
    info2.iStartTrackPosition = 30.f;
    info2.iEndTrackPosition = 41.f;
    info2.CancleAbleRatio = 0.85;

    shared_ptr<CCollider> pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_SPHERE, normalAtkDesc);
    pCollider->SetOwner(shared_from_this());
    m_bCancle = false;
   
    m_Collider = pCollider;

    m_Infos.push_back(info);
    m_Infos.push_back(info2);

    return S_OK;
}

void CSkillR::PriorityTick(_float _fTimeDelta)
{
}
 
void CSkillR::Tick(_float _fTimeDelta)
{
    __super::Tick(_fTimeDelta);

}

void CSkillR::LateTick(_float _fTimeDelta)
{
}

HRESULT CSkillR::Render()
{
    __super::Render();

    return S_OK;
}

shared_ptr<CSkillR> CSkillR::Create()
{
    shared_ptr<CSkillR> pInstance = make_shared<CSkillR>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CSkillR");

    return pInstance;
}
