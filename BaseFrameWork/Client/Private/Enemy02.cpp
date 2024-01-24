#include "pch.h"
#include "Enemy02.h"

#include "GameInstance.h"
#include "Model.h"
#include "Shader.h"
#include "Collider.h"
#include "MapLoader.h"

CEnemy02::CEnemy02()
{
}

HRESULT CEnemy02::Initialize()
{
    CTransform::TRANSFORM_DESC TransformDesc = {};
    TransformDesc.fSpeedPerSet = 1.f;
    TransformDesc.fRotationPerSet = 10.f;

    if (FAILED(__super::Initialize(&TransformDesc)))
        return E_FAIL;

    m_pShader = CGameInstance::GetInstance()->GetShader(TEXT("Shader_VtxAnim"));

    m_pModelCom = m_pGameInstance->GetModel(TEXT("Enemy02"));
    m_Components.emplace(TEXT("Com_Model"), m_pModelCom);

    AttackDistance = 2.f;
    m_eMonsterType = EMONSTER_TYPE::MONSTER_NORMAL;

    m_iTotalAtkNum = 3;
    m_fTotalCoolTime = 2.f;

    m_pModelCom->SetAnimNum(20);
    m_pNavigation = CMapLoader::GetInstance()->GetCurrentNavi(0);

        /*********Collider*************/

    CCollider::COLLIDER_DESC ColliderDesc = {};
    ColliderDesc.fRadius = 0.9f;
    ColliderDesc.vCenter = _float3(0.f, 0.6f, 0.f);

    m_pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_SPHERE, ColliderDesc);
    m_Components.emplace(TEXT("Com_Collider"), m_pCollider);
    m_pCollider->SetOwner(shared_from_this());

    return S_OK;
}

void CEnemy02::PriorityTick(_float _fTimeDelta)
{
}

void CEnemy02::Tick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;

    //공격 쿨타임일때
    if (m_IsAtkCool) {

        m_IsNearPlr = false;

    }

    if (m_bKnockUp) {
        m_bJump = true;
        OnHitKnockUp(_fTimeDelta);
    }

    ////플레이어가 가까워지면 방어모드 on 
    if ((EMONSTER_STATE::STATE_IDLE == m_eCurrentState ||
        EMONSTER_STATE::STATE_WALK == m_eCurrentState)&&
        m_bDefenceAble && CalcPlayerDistanceReturn() < 3.f) {

        m_bDefenceMode = true;
        m_bDefenceAble = false;

        ChangeAnim(10, true);
    }

    _vector posCheck2 = m_pTransformCom->GetState(CTransform::STATE_POSITION);


    //방어 브레이크 이후 스턴 상태 
    if (EMONSTER_STATE::STATE_STUN == m_eCurrentState) {

        m_fStunTime += _fTimeDelta;

        if (m_fStunTime >= 3.f) {

            ChangeAnim(48, false);
            m_eCurrentState = EMONSTER_STATE::STATE_IDLE;
            m_fStunTime = 0.f;
            m_IsAtkCool = true;
        }
    }

    //공격하는 중이 아니고 공격 쿨타임이 아닐때(행동 결정)
    if(EMONSTER_STATE::STATE_ATTACK != m_eCurrentState && !m_IsAtkCool&& !m_bDefenceMode &&
        EMONSTER_STATE::STATE_STUN != m_eCurrentState && !m_bHit) {

        ChasePlayer();

       if (!m_IsNearPlr) {

           if(!m_bWait)
           {
               m_eCurrentState = EMONSTER_STATE::STATE_WALK;
               ChangeAnim(62, true);
               _bool jump = false;

               m_pTransformCom->GoStraight(_fTimeDelta, m_pNavigation, jump);
           }
       }
    }

    _vector posCheck3 = m_pTransformCom->GetState(CTransform::STATE_POSITION);


    if (m_bHit) {
        m_pModelCom->SetLinearTime(0.02f);
    }

    if (m_pModelCom->PlayAnimation(_fTimeDelta, m_bLoop, &m_vCurrentAnimPos)) {
        CheckReserveAnimList();
    }

   CalcAnimationDistance();

   _vector posCheck4 = m_pTransformCom->GetState(CTransform::STATE_POSITION);

   m_pCollider->Tick(m_pTransformCom->GetWorldMatrix());
   CGameInstance::GetInstance()->AddCollider(CCollisionMgr::COL_MONSTER, m_pCollider);

}

void CEnemy02::LateTick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;

    if (!m_bHit) {

        CalcPlayerDistance();


        if (m_bWait) {
            m_fWaitTime += _fTimeDelta;
            if (m_fWaitTime > 2.f) {
                m_bWait = false;
                m_fWaitTime = 0.f;
                IdlePattern(0);
            }

        }

        if (m_bDefenceMode) {
            m_fDefenceCoolTime += _fTimeDelta;

            if (0 < m_iDefenceGage && m_fDefenceCoolTime > 4.f) {

                m_bDefenceMode = false;
                m_eCurrentState = EMONSTER_STATE::STATE_STUN;
                ChangeAnim(46, true);
                m_fDefenceCoolTime = 0.f;

                /* m_bDefenceMode = false;
                 m_eCurrentState = EMONSTER_STATE::STATE_IDLE;
                 ChangeAnim(40, false);
                 m_fDefenceCoolTime = 0.f;
                 m_IsAtkCool = true;*/

            }
            else if (0 >= m_iDefenceGage || m_fDefenceCoolTime > 4.f) {

                m_bDefenceMode = false;
                m_eCurrentState = EMONSTER_STATE::STATE_STUN;
                ChangeAnim(46, true);
                m_fDefenceCoolTime = 0.f;
            }
        }

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


}

HRESULT CEnemy02::Render()
{
    if (!m_IsEnabled)
        return S_OK;

    m_pCollider->Render();


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
    m_NextAnimIndex.clear();
    m_bLoop = false;

     switch (_iAtkNum)
    {
    case 0:
        
        ChangeAnim(0, m_bLoop);
        m_iAtkPattern = _iAtkNum;
        break;

    case 1:
        ChangeAnim(2, m_bLoop);
        m_iAtkPattern = _iAtkNum;
        break;

    case 2:
        ChangeAnim(4, m_bLoop);
        m_iAtkPattern = _iAtkNum;
        break;
    default:
        break;
    }

    m_bWait = false;
    m_fWaitTime = 0.f;
    

    m_eCurrentState = EMONSTER_STATE::STATE_ATTACK;
    m_fTotalCoolTime = 6.f + rand() % 3;

}

void CEnemy02::IdlePattern(_uint _iAtkNum)
{
    //방어중일때는 현상유지 
    if (!m_bDefenceMode) {
        //m_iNextPattern = rand() % 2;
    
        switch (_iAtkNum)
        {
        case 0:
            ChangeAnim(22, false);
            m_NextAnimIndex.push_back({ 62, true });
            m_bWait = false;
            break;

        case 1:
            ChangeAnim(32, true);
            m_bWait = true;
            break;

        case 2:
            ChangeAnim(22, false);
            m_NextAnimIndex.push_back({ 62, true });
            m_bWait = true;
            break;

        default:
            break;
        }
    }

    m_eCurrentState = EMONSTER_STATE::STATE_IDLE;

}

void CEnemy02::WalkPattern(_uint _iWalkNum)
{
}

void CEnemy02::IfEmptyAnimList()
{

    m_vPrevAnimPos = { 0.f, 0.f, 0.f };
    m_vCurrentAnimPos = { 0.f, 0.f, 0.f };

    if (m_bHit) {

        //녹업종료 
        if (!m_bKnockUp && (50 == m_iAnimNum)) {
            ChangeAnim(12, false);
        }


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
        IdlePattern(rand()%3);
    }

}

_bool CEnemy02::CalcDistanceOption()
{
    if (48 == m_iAnimNum || 20 == m_iAnimNum || 22 == m_iAnimNum || 24 == m_iAnimNum || 26 == m_iAnimNum ||
        62 == m_iAnimNum || 32 == m_iAnimNum || 50 == m_iAnimNum || 16 == m_iAnimNum || 17 == m_iAnimNum) {
        m_vPrevAnimPos = { 0.f, 0.f, 0.f };
        m_vCurrentAnimPos = { 0.f, 0.f, 0.f };
        return false;
    }

    return true;
}

void CEnemy02::ResetState()
{
    m_fWaitTime = 0.f;
    IdlePattern(0);
}


void CEnemy02::OnHit()
{
    if (m_bDefenceMode) {

        return;
    }


    m_eCurrentState = EMONSTER_STATE::STATE_HIT;
    m_bHit = true;

    if (m_bDownAttack) {
        m_NextAnimIndex.clear();
        ChangeAnim(54, false);
        m_NextAnimIndex.push_back({ 50, false });
        m_NextAnimIndex.push_back({ 12, false });

    }else if (m_bKnockUp) {
        if (16 == m_iCurrentAtkNum) {
            m_NextAnimIndex.clear();
            ChangeAnim(17, false);
        }
        else {
            m_NextAnimIndex.clear();
            ChangeAnim(16, false);
        }

        m_NextAnimIndex.push_back({ 50, true });
        m_bJump = true;
    }
    else {

        if (18 == m_iAnimNum) {
            ChangeAnim(19, false);
        }
        else {
            ChangeAnim(18, false);
        }
    }

}

shared_ptr<CEnemy02> CEnemy02::Create()
{
	shared_ptr<CEnemy02> pInstance = make_shared<CEnemy02>();

	if(FAILED(pInstance->Initialize()))
		MSG_BOX("Failed to Create : CEnemy02");

	return pInstance;
}