#include "pch.h"
#include "FinalBoss.h"
#include "GameInstance.h"
#include "Shader.h"

#include "Model.h"

CFinalBoss::CFinalBoss()
{
}

HRESULT CFinalBoss::Initialize()
{
    CTransform::TRANSFORM_DESC TransformDesc = {};
    TransformDesc.fSpeedPerSet = 0.f;
    TransformDesc.fRotationPerSet = 0.f;

    if (FAILED(__super::Initialize(&TransformDesc)))
        return E_FAIL;

    m_pShader = CGameInstance::GetInstance()->GetShader(TEXT("Shader_VtxAnim"));

    m_pModelCom = m_pGameInstance->GetModel(TEXT("FinalBoss"));
    m_Components.emplace(TEXT("Com_Model"), m_pModelCom);

    AttackDistance = 5.f;
    m_eMonsterType = EMONSTER_TYPE::MONSTER_BOSS;

    m_iTotalAtkNum = 3;
    m_fTotalCoolTime = 3.f;

    m_bLoop = true;
    m_pModelCom->ChangeAnimation(13);
    m_IsAtkCool = true;

    return S_OK;
}

void CFinalBoss::PriorityTick(_float _fTimeDelta)
{
}

void CFinalBoss::Tick(_float _fTimeDelta)
{

    if (!m_IsEnabled)
        return;

    if (!m_bSetOriginLook) {
        _vector vLook = m_pTransformCom->GetState(CTransform::STATE_LOOK);

        m_fTurnTime += _fTimeDelta;

        if (m_fTurnTime > 2.f) {

            m_bSetOriginLook = true;
            m_fTurnTime = 0.f;
            vLook = XMVectorLerp(vLook, m_vOriginLookVec, 1.f);

            m_pTransformCom->SetState(CTransform::STATE_LOOK, vLook);
            

        }
        else {
            vLook= XMVectorLerp(vLook, m_vOriginLookVec, m_fTurnTime /2.f);
            m_pTransformCom->SetState(CTransform::STATE_LOOK, vLook);

        }

    }


    if (4 == m_iAnimNum) {
        CalcPlayerDistance();
        m_fPatternCheckTime += _fTimeDelta;

        if (m_fPatternCheckTime >= m_fPattern4DelayTime) {

            ChangeAnim(5, false);
            m_fPatternCheckTime = 0;

        }

    }



    if (m_pModelCom->PlayAnimation(_fTimeDelta, m_bLoop, &m_vCurrentAnimPos)) {

        m_vPrevAnimPos = { 0.f, 0.f, 0.f };
        m_vCurrentAnimPos = { 0.f, 0.f, 0.f };
        CheckReserveAnimList();
    }

}

void CFinalBoss::LateTick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;

    if (m_IsAtkCool) {

        m_bAttackCoolTime += _fTimeDelta;

        if (m_bAttackCoolTime >= m_fTotalCoolTime) {
            m_IsAtkCool = false;
            m_bAttackCoolTime = 0.f;

            ++m_iTestNum;
            _uint iRand = m_iTestNum % 5;
            AttackPattern(iRand);
            m_eCurrentState = EMONSTER_STATE::STATE_ATTACK;
        }

    }


	if (FAILED(m_pGameInstance->AddRenderGroup(CRenderer::RENDER_NONBLEND, shared_from_this())))
		return;
}

HRESULT CFinalBoss::Render()
{

    if (!m_IsEnabled)
        return S_OK;

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

void CFinalBoss::AttackPattern(_uint _iAtkNum)
{
    m_eCurrentState = EMONSTER_STATE::STATE_ATTACK;
    m_iCurrentAtkNum = _iAtkNum;

    switch (_iAtkNum)
    {
    case 0:
        ChangeAnim(0, false);

        break;

    case 1:
        ChangeAnim(1, false);

        break;

    case 2:
        ChangeAnim(2, false);

        break;

    case 3:
        ChangeAnim(3, false);
        m_NextAnimIndex.push_back({ 4 , true });
        m_vOriginLookVec = m_pTransformCom->GetState(CTransform::STATE_LOOK);
        break;

    case 4:
        ChangeAnim(8, false);

        break;

    default:
        break;
    }

}

void CFinalBoss::IfEmptyAnimList()
{
    if (EMONSTER_STATE::STATE_ATTACK == m_eCurrentState) {

        if( 5 == m_iAnimNum ){
            m_bSetOriginLook = false;
        }

        m_IsAtkCool = true;
        m_eCurrentState = EMONSTER_STATE::STATE_IDLE;
    }

    ChangeAnim(13, true);

}

void CFinalBoss::CalcDistanceOption()
{
}

shared_ptr<CFinalBoss> CFinalBoss::Create()
{
	shared_ptr<CFinalBoss> pInstance = make_shared<CFinalBoss>();

	if (FAILED(pInstance->Initialize()))
		MSG_BOX("Failed to Create : CFinalBoss");

	return pInstance;
}
