#include "pch.h"
#include "Enemy01.h"

#include "GameInstance.h"
#include "Model.h"
#include "Shader.h"

#include "MonsterSkillSet.h"
#include "Collider.h"

#include "MapLoader.h"
#include "Navigation.h"

#include "WorldHPBar.h"


CEnemy01::CEnemy01()
{
}

HRESULT CEnemy01::Initialize()
{
    CTransform::TRANSFORM_DESC TransformDesc = {};
    TransformDesc.fSpeedPerSet = 5.f;
    TransformDesc.fRotationPerSet = 10.f;

    if (FAILED(__super::Initialize(&TransformDesc)))
        return E_FAIL;

    m_pShader = CGameInstance::GetInstance()->GetShader(TEXT("Shader_VtxAnim"));

    m_pTransformCom->SetScaling(0.5f, 0.5f, 0.5f);

    m_pModelCom = m_pGameInstance->GetModel(TEXT("Enemy01"));
    m_Components.emplace(TEXT("Com_Model"), m_pModelCom);

    AttackDistance = 1.6f;
    m_eMonsterType = EMONSTER_TYPE::MONSTER_NORMAL;
    m_fTotalCoolTime = 2.f;

    m_iTotalAtkNum = 3;
    m_iCurrentAtkNum = 0;

    m_pNavigation = CMapLoader::GetInstance()->GetCurrentNavi(0);
    m_Components.emplace(TEXT("Com_Navigation"), m_pNavigation);

    m_iHP = 100;

    m_pHPBar = CWorldHPBar::Create();
    m_pHPBar->SetMaxHP(m_iHP);

    CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_UI"), m_pHPBar);

    /*********Collider*************/

    CCollider::COLLIDER_DESC ColliderDesc = {};
    ColliderDesc.fRadius = 1.6f;
    ColliderDesc.vCenter = { 0.f, 1.5f, 0.f };

    m_pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_SPHERE, ColliderDesc);
    m_Components.emplace(TEXT("Com_Collider"), m_pCollider);
    m_pCollider->SetOwner(shared_from_this());

    m_pSkillSet = CMonsterSkillSet::Create(0, m_pModelCom);
    m_pSkillSet->SetTransform(m_pTransformCom);

    return S_OK;
}

void CEnemy01::PriorityTick(_float _fTimeDelta)
{
}

void CEnemy01::Tick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;

    __super::Tick(_fTimeDelta);


    if (m_IsAtkCool) {
        m_IsNearPlr = false;
    }

    if (m_bSuperArmor) {
        CalcSuperArmorTime(_fTimeDelta);
    }

    if (m_bDown) {
        m_fDownTime += _fTimeDelta;


        if (m_fDownTime > 2.f) {

            if (m_bDie) {
                m_pHPBar->SetEnable(false);
                m_IsEnabled = false;
            }
            else {

                m_bHit = false;
                m_eCurrentState = EMONSTER_STATE::STATE_IDLE;
                m_IsAtkCool = true;
                m_bAttackCoolTime = 0.f;

                m_NextAnimIndex.clear();
                ChangeAnim(9, false);
                m_iLastHitIndex = 100;
                m_iCurrentSkillOrderIndex = 100;
                ResetState();
                m_bDown = false;

            }
        }


    }

    if (m_bKnockUp) {
        m_bJump = true;
        m_fGravity = 26.f;
        m_fGweight = 2.f;
        OnHitKnockUp(_fTimeDelta);
    }



    //공격 쿨타임일때
    if (m_IsAtkCool && !m_bHit && !m_bDie && m_eCurrentState != EMONSTER_STATE::STATE_SPAWN) {
        m_IsNearPlr = false;

        if (20 == m_iAnimNum) {
            m_pTransformCom->SetSpeed(0.5f);
            m_pTransformCom->GoLeft(_fTimeDelta);
            CalcPlayerDistance();
        }
        else if (6 == m_iAnimNum) {
        
        }
        else if (21 == m_iAnimNum) {
            m_pTransformCom->SetSpeed(0.5f);
            m_pTransformCom->GoRight(_fTimeDelta, m_pNavigation, &m_bJump);

        }
        else if (29 == m_iAnimNum) {
            m_pTransformCom->SetSpeed(0.5f);
            m_pTransformCom->GoStraight(_fTimeDelta, m_pNavigation, m_bJump);
        }
    }

    //공격하는 중이 아니고 공격 쿨타임이 아닐때(행동 결정)
    if (EMONSTER_STATE::STATE_ATTACK != m_eCurrentState && !m_IsAtkCool && !m_bHit && !m_bDie
        && m_eCurrentState != EMONSTER_STATE::STATE_SPAWN) {

        ChasePlayer();

        if (!m_IsNearPlr){
            WalkPattern(m_iWalkPatternNum);
            m_pTransformCom->GoStraight(_fTimeDelta, m_pNavigation, m_bJump);
        }
    }
    

    if (m_bHit) {
        m_pModelCom->SetLinearTime(0.02f);
    }
    else {
        m_pModelCom->SetLinearTime(0.05f);
    }

    if (m_pModelCom->PlayAnimation(_fTimeDelta, m_bLoop, &m_vCurrentAnimPos)) {

        m_vPrevAnimPos = { 0.f, 0.f, 0.f };
        m_vCurrentAnimPos = { 0.f, 0.f, 0.f };

        if (m_iAnimNum == 8) {
            m_pHPBar->SetEnable(false);
            m_IsEnabled = false;
            return;
        }

        CheckReserveAnimList();

        if (EMONSTER_STATE::STATE_ATTACK == m_eCurrentState) {
            m_eCurrentState = EMONSTER_STATE::STATE_IDLE;
        }
    }

    CalcAnimationDistance();

    m_pSkillSet->Tick(_fTimeDelta);
    m_pCollider->Tick(m_pTransformCom->GetWorldMatrix());
    CGameInstance::GetInstance()->AddCollider(CCollisionMgr::COL_MONSTER, m_pCollider);

    _vector vPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
    vPos.m128_f32[1] += 2.f;

    m_pHPBar->CalcScreenPos(vPos);

}

void CEnemy01::LateTick(_float _fTimeDelta)
{

    if (!m_IsEnabled)
        return;

    if (!m_bHit && !m_bDie && m_eCurrentState != EMONSTER_STATE::STATE_SPAWN) {

        CalcPlayerDistance();

        if (m_IsAtkCool) {
            m_bAttackCoolTime += _fTimeDelta;

            if (m_bAttackCoolTime >= m_fTotalCoolTime) {
                m_IsAtkCool = false;
                m_bAttackCoolTime = 0.f;
            }
        }
    }

    if (FAILED(m_pGameInstance->AddRenderGroup(CRenderer::RENDER_NONBLEND, shared_from_this())))
        return;

#ifdef _DEBUG
    CGameInstance::GetInstance()->AddDebugComponent(m_pCollider);
#endif
}

HRESULT CEnemy01::Render()
{
    if (!m_IsEnabled)
        return S_OK;

   // m_pSkillSet->Render();

    if (FAILED(BindShaderResources()))
        return E_FAIL;

    _uint iNumMeshes = m_pModelCom->GetNumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++) {

        if (FAILED(m_pModelCom->BindMaterialShaderResource(m_pShader, (_uint)i, aiTextureType::aiTextureType_DIFFUSE, "g_DiffuseTexture")))
            return E_FAIL;

        if (FAILED(m_pModelCom->BindBoneMatrices(m_pShader, "g_BoneMatrices", (_uint)i)))
            return E_FAIL;

        if (FAILED(m_pShader->Begin(1)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Render((_uint)i)))
            return E_FAIL;
    }

    return S_OK;
}

void CEnemy01::AttackPattern(_uint _iAtkNum)
{
    if (4 == m_iAnimNum) {
        _iAtkNum = 2;
    }
   
    m_bLoop = false;

    switch (_iAtkNum)
    {
    case 0:
        ChangeAnim(0, m_bLoop);
        m_pSkillSet->SwitchingSkill(CMonsterSkillSet::MON_SKILL1);
        m_iAtkPattern = _iAtkNum;
        break;
    case 1:
        ChangeAnim(1, m_bLoop);
        m_pSkillSet->SwitchingSkill(CMonsterSkillSet::MON_SKILL2);
        m_iAtkPattern = _iAtkNum;
        break;
    case 2:
        ChangeAnim(5, m_bLoop);
        m_pSkillSet->SwitchingSkill(CMonsterSkillSet::MON_SKILL3);
        m_iAtkPattern = _iAtkNum;
        break;
    case 3:
        ChangeAnim(2, m_bLoop);
        m_pSkillSet->SwitchingSkill(CMonsterSkillSet::MON_SKILL4);
        m_iAtkPattern = _iAtkNum;
        break;

    default:
        break;
    }
    m_eCurrentState = EMONSTER_STATE::STATE_ATTACK;
    m_fTotalCoolTime = 7.f + rand() % 3;
}

void CEnemy01::IdlePattern(_uint _iAtkNum)
{
    
    m_iWalkPatternNum = rand() % 2;


    switch (_iAtkNum)
    {
    case 0:
        if (m_iAnimNum != 20) {
            ChangeAnim(20, true);
        }
        m_eCurrentState = EMONSTER_STATE::STATE_WALK;
        break;
    case 1:
        ChangeAnim(3, false);
        m_eCurrentState = EMONSTER_STATE::STATE_IDLE;
        m_iWalkPatternNum = 1;

        break;
    case 2:
        ChangeAnim(6, false);
        m_NextAnimIndex.push_back({ 13, true });
        m_eCurrentState = EMONSTER_STATE::STATE_IDLE;
        break;
    case 3:
        if (m_iAnimNum != 21) {
            ChangeAnim(21, true);
        }
        m_eCurrentState = EMONSTER_STATE::STATE_WALK;

        break;
    case 4:
        if (m_iAnimNum != 13) {
            ChangeAnim(13, true);
        }
        m_eCurrentState = EMONSTER_STATE::STATE_IDLE;
        break;
    default:
        break;
    }    

}

void CEnemy01::WalkPattern(_uint _iWalkNum)
{
    switch (_iWalkNum)
    {
    case 0:
        ChangeAnim(16, true);
        m_pTransformCom->SetSpeed(4.f);
        m_eCurrentState = EMONSTER_STATE::STATE_WALK;
        break;
    case 1:
        ChangeAnim(4, true);
        m_pTransformCom->SetSpeed(6.f);
        m_eCurrentState = EMONSTER_STATE::STATE_WALK;
        break;

    default:
        break;
    }

}

void CEnemy01::UIEnableOn()
{
    m_pHPBar->HPBarReset();
    m_pHPBar->SetEnable(true);
}

void CEnemy01::IfEmptyAnimList()
{
    if (m_eCurrentState == EMONSTER_STATE::STATE_SPAWN) {
        m_eCurrentState = EMONSTER_STATE::STATE_IDLE;
        ChangeAnim(13, true);
    }

    if (14 == m_iAnimNum && m_bDie) {
        m_pHPBar->SetEnable(false);
        m_IsEnabled = false;
        return;
    }

    if (8 == m_iAnimNum) {
        m_IsEnabled = true;
    }

    if (6 == m_iAnimNum) {

        ChangeAnim(13, true);
    }


    if (m_bHit && !m_bKnockUp && !m_bDown) {

        //녹업종료 
        m_bHit = false;
        m_eCurrentState = EMONSTER_STATE::STATE_IDLE;
        m_IsAtkCool = true;
        m_bAttackCoolTime = 0.f;

        ChangeAnim(20, false);
        m_iLastHitIndex = 100;
        m_iCurrentSkillOrderIndex = 100;
        ResetState();

    }


    if (EMONSTER_STATE::STATE_ATTACK == m_eCurrentState) {
        m_eCurrentState = EMONSTER_STATE::STATE_IDLE;
        m_IsAtkCool = true;
        IdlePattern(rand() % 4);
    }

    if (3 == m_iAnimNum && m_bAttackCoolTime) {
        m_IsAtkCool = false;
        m_bAttackCoolTime = 0.f;
    }
}

_bool CEnemy01::CalcDistanceOption()
{
    //0 1 5 2 
    if (27 == m_iAnimNum || 16 == m_iAnimNum || 14 == m_iAnimNum||
        20 == m_iAnimNum || 21 == m_iAnimNum || 29 == m_iAnimNum ||
        0 == m_iAnimNum|| 1 == m_iAnimNum || 5 == m_iAnimNum || 2 == m_iAnimNum||
        4 == m_iAnimNum ||  13 == m_iAnimNum) {
        m_vPrevAnimPos = { 0.f, 0.f, 0.f };
        m_vCurrentAnimPos = { 0.f, 0.f, 0.f };
        return false;
    }

    return true;
}

void CEnemy01::OnHit()
{
    m_eCurrentState = EMONSTER_STATE::STATE_HIT;
    m_bHit = true;

    m_pHPBar->SetHP(m_iHP);

    if (m_bDownAttack) {
        m_NextAnimIndex.clear();
        ChangeAnim(26, false);

        if (m_bDie) {
            m_NextAnimIndex.push_back({ 14, false });
        }
        else {
            m_NextAnimIndex.push_back({ 14, false });
            m_NextAnimIndex.push_back({ 9, false });
        }


    }
    else if (m_bKnockUp) {
       m_NextAnimIndex.clear();
       ChangeAnim(27, false);
       m_NextAnimIndex.push_back({ 14,true });
       m_bJump = true;
    }
    else {

        if (m_bDie) {
            ChangeAnim(8, false);
        }
        else {
            ChangeAnim(12, false);
        }
    }

}

void CEnemy01::SetSpawnState()
{
    __super::SetSpawnState();

    UIEnableOn();

    m_eCurrentState = EMONSTER_STATE::STATE_SPAWN;
    ChangeAnim(18, false);
    m_iHP = 100;


}

void CEnemy01::ResetState()
{
    //여기가 문제인듯 

    m_IsAtkCool = true;
    m_fTotalCoolTime = 2.f + rand() % 3;
    m_eCurrentState = EMONSTER_STATE::STATE_IDLE;
    IdlePattern(rand()%4);
}

shared_ptr<CEnemy01> CEnemy01::Create()
{
    shared_ptr<CEnemy01> pInstance = make_shared<CEnemy01>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CEnemy01");

    return pInstance;
}
