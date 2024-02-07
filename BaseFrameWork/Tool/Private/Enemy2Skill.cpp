#include "pch.h"
#include "Enemy2Skill.h"
#include "Collider.h"
#include "GameInstance.h"

CEnemy2Skill::CEnemy2Skill()
{
}

HRESULT CEnemy2Skill::Initialize(_uint _iComboNum)
{

    CCollider::COLLIDER_DESC normalAtkDesc = {};
    SKILLINFO skillDesc = {};

    m_eSkillOwner = EOWNER_TYPE::OWNER_MONSTER;

    m_iDamage = 15.f;

    //돌면서 공격
    if (1 == _iComboNum) {

        normalAtkDesc.fRadius = 1.f;
        normalAtkDesc.vCenter = { 0.f, 0.5f, 0.f };

        skillDesc.bKnockUp = true;
        skillDesc.bDownAtk = true;
        skillDesc.fWeight = 1.f;

        skillDesc.fKnockUpDistance = 4.f;
        skillDesc.fDropSpeed = 4.f;


        skillDesc.iStartTrackPosition = 10.0;
        skillDesc.iEndTrackPosition = 40.0;


    }
    //위에서 아래로 쾅?? 
    else if (2 == _iComboNum) {
        normalAtkDesc.fRadius = 1.f;
        normalAtkDesc.vCenter = { 0.f, 0.5f, 0.5f };

        skillDesc.bKnockUp = false;
        skillDesc.bDownAtk = true;
        skillDesc.fWeight = 1.f;

        skillDesc.iStartTrackPosition = 30.0;
        skillDesc.iEndTrackPosition = 50.0;

    }
    //밀기 
    else if (3 == _iComboNum) {
        normalAtkDesc.fRadius = 1.f;
        normalAtkDesc.vCenter = { 0.f, 0.5f, 0.5f };

        skillDesc.bKnockUp = false;
        skillDesc.bDownAtk = false;
        skillDesc.fWeight = 1.f;

        skillDesc.iStartTrackPosition = 30.0;
        skillDesc.iEndTrackPosition = 50.0;

    }

    shared_ptr<CCollider> pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_SPHERE, normalAtkDesc);
    pCollider->SetOwner(shared_from_this());
    m_bCancle = true;

    m_Collider = pCollider;
    m_Infos.push_back(skillDesc);

    return S_OK;
}

void CEnemy2Skill::PriorityTick(_float _fTimeDelta)
{
}

void CEnemy2Skill::Tick(_float _fTimeDelta)
{
    __super::Tick(_fTimeDelta);

}

void CEnemy2Skill::LateTick(_float _fTimeDelta)
{
}

HRESULT CEnemy2Skill::Render()
{
    __super::Render();
    return S_OK;

}

shared_ptr<CEnemy2Skill> CEnemy2Skill::Create(_uint _iSkillNum)
{
    shared_ptr<CEnemy2Skill> pinstance = make_shared<CEnemy2Skill>();
    if(FAILED(pinstance->Initialize(_iSkillNum)))
        MSG_BOX("Failed to Create : CEnenmy2Skill");

    return pinstance;
}
