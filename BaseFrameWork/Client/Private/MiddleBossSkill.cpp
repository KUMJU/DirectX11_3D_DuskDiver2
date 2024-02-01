#include "pch.h"
#include "MiddleBossSkill.h"
#include "Collider.h"
#include "GameInstance.h"

CMiddleBossSkill::CMiddleBossSkill()
{
}

HRESULT CMiddleBossSkill::Initialize(_uint _iComboNum)
{
	CCollider::COLLIDER_DESC normalAtkDesc = {};
	
	SKILLINFO skillDesc = {};

	m_eSkillOwner = EOWNER_TYPE::OWNER_MONSTER;

    //두번 휘두르기
    if (1 == _iComboNum) {

        normalAtkDesc.fRadius = 1.5f;
        normalAtkDesc.vCenter = { 0.f, 1.5f, 2.f };

        skillDesc.bKnockUp = true;
        skillDesc.bDownAtk = true;
        skillDesc.fWeight = 1.f;

        skillDesc.fKnockUpDistance = 10.f;
        skillDesc.fDropSpeed = 7.f;

        skillDesc.iStartTrackPosition = 5.0;
        skillDesc.iEndTrackPosition = 80.0;


    }
    //앞으로 넘어지기
    else if (2 == _iComboNum) {
        normalAtkDesc.fRadius = 2.f;
        normalAtkDesc.vCenter = { 0.f, 2.f, 0.5f };

        skillDesc.bKnockUp = false;
        skillDesc.bDownAtk = true;
        skillDesc.fWeight = 1.f;

        skillDesc.iStartTrackPosition = 35.0;
        skillDesc.iEndTrackPosition = 110.0;

    }
    //위에서 떨어지기 
    else if (3 == _iComboNum) {
        normalAtkDesc.fRadius = 2.f;
        normalAtkDesc.vCenter = { 0.f, 2.f, 1.5f };

        skillDesc.bKnockUp = true;
        skillDesc.bDownAtk = true;
        skillDesc.fWeight = 1.f;

        skillDesc.iStartTrackPosition = 70.0;
        skillDesc.iEndTrackPosition = 110.0;

    }
    //빙글빙글 도는 공격 
    else if (4 == _iComboNum) {
        normalAtkDesc.fRadius = 4.f;
        normalAtkDesc.vCenter = { 0.f, 2.f, 0.f };

        skillDesc.bKnockUp = false;
        skillDesc.bDownAtk = true;
        skillDesc.fWeight = 1.f;

        skillDesc.iStartTrackPosition = 50.0;
        skillDesc.iEndTrackPosition = 110.0;

    }

    shared_ptr<CCollider> pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_SPHERE, normalAtkDesc);
    pCollider->SetOwner(shared_from_this());
    m_bCancle = true;

    m_Collider = pCollider;
    m_Infos.push_back(skillDesc);


	return S_OK;
}

void CMiddleBossSkill::PriorityTick(_float _fTimeDelta)
{
}

void CMiddleBossSkill::Tick(_float _fTimeDelta)
{
    __super::Tick(_fTimeDelta);
}

void CMiddleBossSkill::LateTick(_float _fTimeDelta)
{
}

HRESULT CMiddleBossSkill::Render()
{
    __super::Render();
	return S_OK;
}

shared_ptr<CMiddleBossSkill> CMiddleBossSkill::Create(_uint _iSkillNum)
{
    shared_ptr<CMiddleBossSkill> pInstance = make_shared<CMiddleBossSkill>();
    
    if (FAILED(pInstance->Initialize(_iSkillNum)))
        MSG_BOX("Failed to Create : CMiddleBossSkill");

	return pInstance;
}
