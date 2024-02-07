#include "pch.h"
#include "Enemy1Skill.h"
#include "Collider.h"
#include "GameInstance.h"

CEnemy1Skill::CEnemy1Skill()
{
}

HRESULT CEnemy1Skill::Initialize(_uint _iSkillNum)
{
    CCollider::COLLIDER_DESC normalAtkDesc = {};
    normalAtkDesc.fRadius = 1.8f;
    normalAtkDesc.vCenter = { 0.f, 0.6f, 1.f };

    m_eSkillOwner = EOWNER_TYPE::OWNER_MONSTER;

    SKILLINFO skillDesc = {};

    m_iDamage = 15.f;

    if (1 == _iSkillNum) {


        skillDesc.bKnockUp = false;
        skillDesc.bDownAtk = false;
        skillDesc.fWeight = 1.f;

        skillDesc.fKnockUpDistance = 4.f;
        skillDesc.fDropSpeed = 4.f;


        skillDesc.iStartTrackPosition = 20.0;
        skillDesc.iEndTrackPosition = 30.0;

    }
    else if (2 == _iSkillNum) {

        skillDesc.bKnockUp = false;
        skillDesc.bDownAtk = false;
        skillDesc.fWeight = 1.f;

        skillDesc.fKnockUpDistance = 0.f;
        skillDesc.fDropSpeed = 4.f;


        skillDesc.iStartTrackPosition = 14.0;
        skillDesc.iEndTrackPosition = 30.0;


    }
    else if(3 == _iSkillNum) {

        skillDesc.bKnockUp = true;
        skillDesc.bDownAtk = false;
        skillDesc.fWeight = 1.f;

        skillDesc.fKnockUpDistance = 4.f;
        skillDesc.fDropSpeed = 4.f;


        skillDesc.iStartTrackPosition = 10.0;
        skillDesc.iEndTrackPosition = 30.0;

    }
    else if (4 == _iSkillNum) {

        skillDesc.bKnockUp = true;
        skillDesc.bDownAtk = true;
        skillDesc.fWeight = 1.f;

        skillDesc.fKnockUpDistance = 4.f;
        skillDesc.fDropSpeed = 4.f;


        skillDesc.iStartTrackPosition = 20.0;
        skillDesc.iEndTrackPosition = 30.0;

    }

    shared_ptr<CCollider> pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_SPHERE, normalAtkDesc);
    pCollider->SetOwner(shared_from_this());
    m_bCancle = true;

    m_Collider = pCollider;
    m_Infos.push_back(skillDesc);
    return S_OK;
}

void CEnemy1Skill::PriorityTick(_float _fTimeDelta)
{
}

void CEnemy1Skill::Tick(_float _fTimeDelta)
{
    __super::Tick(_fTimeDelta);

}

void CEnemy1Skill::LateTick(_float _fTimeDelta)
{
}

HRESULT CEnemy1Skill::Render()
{
    __super::Render();

    return S_OK;
}

shared_ptr<CEnemy1Skill> CEnemy1Skill::Create(_uint _iSkillNum)
{
    shared_ptr<CEnemy1Skill> pInstance = make_shared<CEnemy1Skill>();

    if(FAILED(pInstance->Initialize(_iSkillNum)))
        MSG_BOX("Failed to Create : CEnemy1Skill");

    return pInstance;
}
