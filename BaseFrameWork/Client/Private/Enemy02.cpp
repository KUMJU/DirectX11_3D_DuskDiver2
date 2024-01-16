#include "pch.h"
#include "Enemy02.h"

#include "GameInstance.h"
#include "Model.h"
#include "Shader.h"

CEnemy02::CEnemy02()
{
}

HRESULT CEnemy02::Initialize()
{
    CTransform::TRANSFORM_DESC TransformDesc = {};
    TransformDesc.fSpeedPerSet = 5.f;
    TransformDesc.fRotationPerSet = 10.f;

    if (FAILED(__super::Initialize(&TransformDesc)))
        return E_FAIL;

    m_pShader = CGameInstance::GetInstance()->GetShader(TEXT("Shader_VtxAnim"));
    m_pTransformCom->SetState(CTransform::STATE_POSITION, { 5.f, 0.f, 5.f });


    m_pModelCom = m_pGameInstance->GetModel(TEXT("Enemy02"));
    m_Components.emplace(TEXT("Com_Model"), m_pModelCom);

    AttackDistance = 2.f;
    m_eMonsterType = EMONSTER_TYPE::MONSTER_NORMAL;

    m_iTotalAtkNum = 3;
    m_fTotalCoolTime = 0.5f;

    return S_OK;
}

void CEnemy02::PriorityTick(_float _fTimeDelta)
{
}

void CEnemy02::Tick(_float _fTimeDelta)
{
    
    //공격 쿨타임일때
    if (m_IsAtkCool) {
        m_IsNearPlr = false;

       /* if (20 == m_iAnimNum) {
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
        }*/
    }

    //공격하는 중이 아니고 공격 쿨타임이 아닐때(행동 결정)
    if (EMONSTER_STATE::STATE_ATTACK != m_eCurrentState && !m_IsAtkCool) {

        CalcPlayerDistance();
        ChasePlayer();

        if (!m_IsNearPlr) {

            ChangeAnim(0, true);
           m_pTransformCom->GoStraight(_fTimeDelta);
        }
    }


    if (m_pModelCom->PlayAnimation(_fTimeDelta, m_bLoop, &m_vCurrentAnimPos)) {
        //공격 종료
        CheckReserveAnimList();
    }

   CalcAnimationDistance();

}

void CEnemy02::LateTick(_float _fTimeDelta)
{
    //if (m_IsAtkCool) {
    //    m_bAttackCoolTime += _fTimeDelta;

    //    if (m_bAttackCoolTime >= m_fTotalCoolTime) {
    //        m_IsAtkCool = false;
    //        m_bAttackCoolTime = 0.f;
    //    }

    //}

    if (FAILED(m_pGameInstance->AddRenderGroup(CRenderer::RENDER_NONBLEND, shared_from_this())))
        return;


}

HRESULT CEnemy02::Render()
{
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

void CEnemy02::AttackPattern(_uint _iAtkNum)
{
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
        ChangeAnim(2, m_bLoop);
        m_iAtkPattern = _iAtkNum;
        break;
    default:
        break;
    }

}

void CEnemy02::IdlePattern(_uint _iAtkNum)
{
}

void CEnemy02::WalkPattern(_uint _iWalkNum)
{
}

void CEnemy02::IfEmptyAnimList()
{
}

void CEnemy02::CalcDistanceOption()
{
}


shared_ptr<CEnemy02> CEnemy02::Create()
{
	shared_ptr<CEnemy02> pInstance = make_shared<CEnemy02>();

	if(FAILED(pInstance->Initialize()))
		MSG_BOX("Failed to Create : CEnemy02");

	return pInstance;
}