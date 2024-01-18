#include "pch.h"
#include "Enemy01.h"

#include "GameInstance.h"
#include "Model.h"
#include "Shader.h"

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

    AttackDistance = 2.f;
    m_eMonsterType = EMONSTER_TYPE::MONSTER_NORMAL;

    m_iTotalAtkNum = 4;
    m_iCurrentAtkNum = 0;

    return S_OK;
}

void CEnemy01::PriorityTick(_float _fTimeDelta)
{
}

void CEnemy01::Tick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;


    //공격 쿨타임일때
    if (m_IsAtkCool) {
        m_IsNearPlr = false;

        if (20 == m_iAnimNum) {
            m_pTransformCom->SetSpeed(0.5f);
            m_pTransformCom->GoLeft(_fTimeDelta);
            CalcPlayerDistance();
        }
        else if (32 == m_iAnimNum) {

        }
        else if (6 == m_iAnimNum) {
        
        }
        else if (21 == m_iAnimNum) {
            m_pTransformCom->SetSpeed(0.5f);
            m_pTransformCom->GoRight(_fTimeDelta);

        }
        else if (29 == m_iAnimNum) {
            m_pTransformCom->SetSpeed(0.5f);
            m_pTransformCom->GoStraight(_fTimeDelta);
        }
    }

    //공격하는 중이 아니고 공격 쿨타임이 아닐때(행동 결정)
    if (EMONSTER_STATE::STATE_ATTACK != m_eCurrentState && !m_IsAtkCool) {

        CalcPlayerDistance();
        ChasePlayer();

        if (!m_IsNearPlr) {
            WalkPattern(m_iWalkPatternNum);
            m_pTransformCom->GoStraight(_fTimeDelta);
        }
    }
    

    if (m_pModelCom->PlayAnimation(_fTimeDelta, m_bLoop, &m_vCurrentAnimPos)) {
        //공격 종료
        CheckReserveAnimList();
    }

    CalcAnimationDistance();

}

void CEnemy01::LateTick(_float _fTimeDelta)
{

    if (!m_IsEnabled)
        return;

    if (m_IsAtkCool) {
        m_bAttackCoolTime += _fTimeDelta;

        if (m_bAttackCoolTime >= m_fTotalCoolTime) {
            m_IsAtkCool = false;
            m_bAttackCoolTime = 0.f;
        }

        CalcPlayerDistance();

    }            

    if (FAILED(m_pGameInstance->AddRenderGroup(CRenderer::RENDER_NONBLEND, shared_from_this())))
        return;

}

HRESULT CEnemy01::Render()
{
    if (!m_IsEnabled)
        return S_OK;

    if (FAILED(BindShaderResources()))
        return E_FAIL;

    _uint iNumMeshes = m_pModelCom->GetNumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++) {

        if (FAILED(m_pModelCom->BindMaterialShaderResource(m_pShader, (_uint)i, aiTextureType::aiTextureType_DIFFUSE, "g_DiffuseTexture")))
            return E_FAIL;

        if (FAILED(m_pModelCom->BindBoneMatrices(m_pShader, "g_BoneMatrices", i)))
            return E_FAIL;

        if (FAILED(m_pShader->Begin(0)))
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
        m_iAtkPattern = _iAtkNum;
        break;
    case 1:
        ChangeAnim(1, m_bLoop);
        m_iAtkPattern = _iAtkNum;
        break;
    case 2:
        ChangeAnim(5, m_bLoop);
        m_iAtkPattern = _iAtkNum;
        break;
    case 3:
        ChangeAnim(2, m_bLoop);
        m_iAtkPattern = _iAtkNum;
        break;

    default:
        break;
    }
    m_eCurrentState = EMONSTER_STATE::STATE_ATTACK;
    m_fTotalCoolTime = 3.f + rand() % 3;
}

void CEnemy01::IdlePattern(_uint _iAtkNum)
{
    
    m_iWalkPatternNum = rand() % 2;


    switch (_iAtkNum)
    {
    case 0:
        ChangeAnim(20, true);
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
        ChangeAnim(21, true);
        m_eCurrentState = EMONSTER_STATE::STATE_WALK;

        break;
    case 4:
        ChangeAnim(13, true);
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
        m_pTransformCom->SetSpeed(7.f);
        m_eCurrentState = EMONSTER_STATE::STATE_WALK;
        break;

    default:
        break;
    }

}

void CEnemy01::IfEmptyAnimList()
{
    if (3 == m_iAnimNum && m_bAttackCoolTime) {
        m_IsAtkCool = false;
        m_bAttackCoolTime = 0.f;
    }


    if (EMONSTER_STATE::STATE_ATTACK == m_eCurrentState) {
        m_eCurrentState = EMONSTER_STATE::STATE_IDLE;
        m_IsAtkCool = true;
        IdlePattern(rand() % 5);
    }

}

void CEnemy01::CalcDistanceOption()
{

    m_vPrevAnimPos = { 0.f, 0.f, 0.f };
    m_vCurrentAnimPos = { 0.f, 0.f, 0.f };

}

shared_ptr<CEnemy01> CEnemy01::Create()
{
    shared_ptr<CEnemy01> pInstance = make_shared<CEnemy01>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CEnemy01");

    return pInstance;
}
