#include "pch.h"
#include "MiddleBoss.h"

#include "Model.h"
#include "GameInstance.h"

#include "Shader.h"

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

    AttackDistance = 5.f;
    m_eMonsterType = EMONSTER_TYPE::MONSTER_BOSS;

    m_iTotalAtkNum = 3;
    m_fTotalCoolTime = 2.f;

    m_bLoop = false;
    m_pModelCom->ChangeAnimation(43);

	return S_OK;
}

void CMiddleBoss::PriorityTick(_float _fTimeDelta)
{
}

void CMiddleBoss::Tick(_float _fTimeDelta)
{

    //공격 쿨타임일때
    if (m_IsAtkCool) {
        m_IsNearPlr = false;
    }

    //공격하는 중이 아니고 공격 쿨타임이 아닐때(행동 결정)
    if (EMONSTER_STATE::STATE_ATTACK != m_eCurrentState && !m_IsAtkCool) {

      //  CalcPlayerDistance();

        ChasePlayer();

        if (!m_IsNearPlr) {
           // ChangeAnim(47, true);
            EMONSTER_STATE::STATE_WALK;
            m_pTransformCom->GoStraight(_fTimeDelta);
        }
    }


    if (m_pModelCom->PlayAnimation(_fTimeDelta, m_bLoop, &m_vCurrentAnimPos)) {
        
        m_vPrevAnimPos = { 0.f, 0.f, 0.f };
        m_vCurrentAnimPos = { 0.f, 0.f, 0.f };
        CheckReserveAnimList();
    }

    CalcAnimationDistance();

}

void CMiddleBoss::LateTick(_float _fTimeDelta)
{

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

HRESULT CMiddleBoss::Render()
{
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
        m_iAtkPattern = _iAtkNum;
        break;

    case 1:
        ChangeAnim(31, m_bLoop);
        m_iAtkPattern = _iAtkNum;
        break;

    case 2:
        ChangeAnim(32, m_bLoop);
        m_iAtkPattern = _iAtkNum;
        break;

    case 3:
        ChangeAnim(35, m_bLoop);
        m_iAtkPattern = _iAtkNum;
        break;
    default:
        break;
    }


    m_eCurrentState = EMONSTER_STATE::STATE_ATTACK;
    m_fTotalCoolTime = 6.f + rand() % 3;

}

void CMiddleBoss::IfEmptyAnimList()
{
    if (EMONSTER_STATE::STATE_ATTACK == m_eCurrentState) {

        WalkPattern();
        m_IsAtkCool = true;
    }


    ChangeAnim(40, true);
    m_eCurrentState = EMONSTER_STATE::STATE_IDLE;

}

void CMiddleBoss::CalcDistanceOption()
{
    if (47 == m_iAnimNum) {
        m_vPrevAnimPos = { 0.f, 0.f, 0.f };
        m_vCurrentAnimPos = { 0.f, 0.f, 0.f };
    }


}

void CMiddleBoss::WalkPattern()
{
    m_iAtkPattern = rand() % 4;
    _float CurrentLenth = CalcPlayerDistanceReturn();
    m_eCurrentState = EMONSTER_STATE::STATE_IDLE;

    if (2 == m_iAtkPattern || 3 == m_iAtkPattern) {

        if (CurrentLenth > AttackDistance) {
            ChangeAnim(47, true);
        }
    }
    else {
        if (CurrentLenth > AttackDistance) {
            ChangeAnim(40, true);
        }
    }

}

shared_ptr<CMiddleBoss> CMiddleBoss::Create()
{
    shared_ptr<CMiddleBoss> pInstance = make_shared<CMiddleBoss>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CMiddleBoss");

    return pInstance;
}
