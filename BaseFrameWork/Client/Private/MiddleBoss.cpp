#include "pch.h"
#include "MiddleBoss.h"

#include "Model.h"
#include "GameInstance.h"

#include "Shader.h"

#include "MapLoader.h"
#include "Collider.h"
#include "Navigation.h"
#include "MonsterSkillSet.h"

CMiddleBoss::CMiddleBoss()
{
}

HRESULT CMiddleBoss::Initialize()
{
    CTransform::TRANSFORM_DESC TransformDesc = {};
    TransformDesc.fSpeedPerSet = 4.f;
    TransformDesc.fRotationPerSet = 6.f;

    if (FAILED(__super::Initialize(&TransformDesc)))
        return E_FAIL;

    m_pShader = CGameInstance::GetInstance()->GetShader(TEXT("Shader_VtxAnim"));

    m_pModelCom = m_pGameInstance->GetModel(TEXT("MiddleBoss"));
    m_Components.emplace(TEXT("Com_Model"), m_pModelCom);

    AttackDistance = 3.f;
    m_eMonsterType = EMONSTER_TYPE::MONSTER_BOSS;

    m_iTotalAtkNum = 4;
    m_fTotalCoolTime = 2.f;

    m_bLoop = false;
    m_pModelCom->ChangeAnimation(43);

    m_pNavigation = CMapLoader::GetInstance()->GetCurrentNavi(0);
    m_Components.emplace(TEXT("Com_Navigation"), m_pNavigation);

    CCollider::COLLIDER_DESC colDesc = {};
    colDesc.vCenter = { 0.f, 2.f, 0.f };
    colDesc.fRadius = 2.f;

    m_iHP = 500;

    m_pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_SPHERE, colDesc);
    m_pCollider->SetOwner(shared_from_this());
    m_Components.emplace(TEXT("Com_Collider"), m_pCollider);

    m_IsAtkCool = true;

    m_pSkillSet = CMonsterSkillSet::Create(2, m_pModelCom);
    m_pSkillSet->SetTransform(m_pTransformCom);

	return S_OK;
}

void CMiddleBoss::PriorityTick(_float _fTimeDelta)
{
}

void CMiddleBoss::Tick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;

    if (m_eCurrentState == EMONSTER_STATE::STATE_IDLE) {

        if (47 == m_iAnimNum) {
            _bool jump = false;
            m_pTransformCom->GoStraight(_fTimeDelta, m_pNavigation, m_bJump);
        }
    }

    //공격 쿨타임일때
    if (m_IsAtkCool) {
        m_IsNearPlr = false;
    }

    //공격하는 중이 아니고 공격 쿨타임이 아닐때(행동 결정)
    if (EMONSTER_STATE::STATE_ATTACK != m_eCurrentState && !m_IsAtkCool) {

        ChasePlayer();

        if (!m_IsNearPlr) {

            ChangeAnim(47, true);
            m_eCurrentState =  EMONSTER_STATE::STATE_WALK;
            m_pTransformCom->GoStraight(_fTimeDelta, m_pNavigation, m_bJump);
        }
    }


    if (m_pModelCom->PlayAnimation(_fTimeDelta, m_bLoop, &m_vCurrentAnimPos)) {
        
        m_vPrevAnimPos = { 0.f, 0.f, 0.f };
        m_vCurrentAnimPos = { 0.f, 0.f, 0.f };
        CheckReserveAnimList();
    }

    CalcAnimationDistance();

    m_pSkillSet->Tick(_fTimeDelta);

    m_pCollider->Tick(m_pTransformCom->GetWorldMatrix());
    CGameInstance::GetInstance()->AddCollider(CCollisionMgr::COL_MONSTER, m_pCollider);


}

void CMiddleBoss::LateTick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;

    //계속 탐지하는건 아무래도 이상한 것ㄱ 같음 

    if (m_eCurrentState != EMONSTER_STATE::STATE_ATTACK) {
        CalcPlayerDistance();
    }

    if (m_IsAtkCool) {

        m_bAttackCoolTime += _fTimeDelta;

        if (m_bAttackCoolTime >= m_fTotalCoolTime) {
            m_IsAtkCool = false;
            m_bAttackCoolTime = 0.f;
        }


    }

    if (FAILED(m_pGameInstance->AddRenderGroup(CRenderer::RENDER_NONBLEND, shared_from_this())))
        return;
}

HRESULT CMiddleBoss::Render()
{

    if (!m_IsEnabled)
        return S_OK;

    m_pCollider->Render();
    m_pSkillSet->Render();

    if (FAILED(BindShaderResources()))
        return E_FAIL;

    _uint iNumMeshes = m_pModelCom->GetNumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++) {

        if (FAILED(m_pModelCom->BindMaterialShaderResource(m_pShader, (_uint)i, aiTextureType::aiTextureType_DIFFUSE, "g_DiffuseTexture")))
            return E_FAIL;

        if (FAILED(m_pModelCom->BindBoneMatrices(m_pShader, "g_BoneMatrices", (_uint)i)))
            return E_FAIL;

        if (FAILED(m_pShader->Begin(0)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Render((_uint)i)))
            return E_FAIL;
    }

    return S_OK;
}

void CMiddleBoss::AttackPattern(_uint _iAtkNum)
{
    m_NextAnimIndex.clear();
    m_bLoop = false;

    switch (_iAtkNum)
    {
    case 0:

        ChangeAnim(23, m_bLoop);
        m_pSkillSet->SwitchingSkill(CMonsterSkillSet::MON_SKILL1);
        m_iAtkPattern = _iAtkNum;
        break;

    case 1:
        ChangeAnim(31, m_bLoop);
        m_pSkillSet->SwitchingSkill(CMonsterSkillSet::MON_SKILL2);
        m_iAtkPattern = _iAtkNum;
        break;

    case 2:
        ChangeAnim(32, m_bLoop);
        m_pSkillSet->SwitchingSkill(CMonsterSkillSet::MON_SKILL3);
        m_iAtkPattern = _iAtkNum;
        break;

    case 3:
        ChangeAnim(35, m_bLoop);
        m_pSkillSet->SwitchingSkill(CMonsterSkillSet::MON_SKILL4);
        m_iAtkPattern = _iAtkNum;
        break;
    default:
        break;
    }


    m_eCurrentState = EMONSTER_STATE::STATE_ATTACK;
    m_fTotalCoolTime = 4.5f + rand() % 3;

}

void CMiddleBoss::IfEmptyAnimList()
{
    if (EMONSTER_STATE::STATE_ATTACK == m_eCurrentState) {

        WalkPattern();
        m_IsAtkCool = true;
    }


    if (16 == m_iAnimNum) {
        m_IsEnabled = false;
        return; 
    }

    ChangeAnim(40, true);
    m_eCurrentState = EMONSTER_STATE::STATE_IDLE;

}

_bool CMiddleBoss::CalcDistanceOption()
{
    if (47 == m_iAnimNum) {
        m_vPrevAnimPos = { 0.f, 0.f, 0.f };
        m_vCurrentAnimPos = { 0.f, 0.f, 0.f };
        return false;
    }

    return true;


}

void CMiddleBoss::WalkPattern()
{
    m_iAtkPattern = rand() % 4;
    _float CurrentLenth = CalcPlayerDistanceReturn();
    m_eCurrentState = EMONSTER_STATE::STATE_IDLE;

    if (2 == m_iAtkPattern || 3 == m_iAtkPattern) {

        if (CurrentLenth > AttackDistance) {
           // ChangeAnim(47, true);
            m_IsAtkCool = false;
        }
    }
    else {
        if (CurrentLenth > AttackDistance) {
            ChangeAnim(40, true);
        }
    }

}

void CMiddleBoss::TurnToPlayer()
{
}

void CMiddleBoss::OnHit()
{

    if (m_bDie) {
        ChangeAnim(16, false);
    }


}

void CMiddleBoss::SetSpawnState()
{
    __super::SetSpawnState();

    m_eCurrentState = EMONSTER_STATE::STATE_IDLE;
    ChangeAnim(14, false);
    m_iHP = 300;

}

shared_ptr<CMiddleBoss> CMiddleBoss::Create()
{
    shared_ptr<CMiddleBoss> pInstance = make_shared<CMiddleBoss>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CMiddleBoss");

    return pInstance;
}
