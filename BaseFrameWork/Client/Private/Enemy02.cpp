#include "pch.h"
#include "Enemy02.h"

#include "GameInstance.h"
#include "Model.h"
#include "Shader.h"
#include "Collider.h"
#include "MapLoader.h"

#include "MonsterSkillSet.h"
#include "Navigation.h"
#include "WorldHPBar.h"

#include "Texture.h"

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

    m_iHP = 100;

    m_pHPBar = CWorldHPBar::Create();
    m_pHPBar->SetMaxHP(m_iHP);
    CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_UI"), m_pHPBar);


    m_pModelCom->SetAnimNum(20);
    m_pNavigation = CMapLoader::GetInstance()->GetCurrentNavi(0);
    m_Components.emplace(TEXT("Com_Navigation"), m_pNavigation);

        /*********Collider*************/

    CCollider::COLLIDER_DESC ColliderDesc = {};
    ColliderDesc.fRadius = 0.9f;
    ColliderDesc.vCenter = _float3(0.f, 0.6f, 0.f);
     
    m_pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_SPHERE, ColliderDesc);
    m_Components.emplace(TEXT("Com_Collider"), m_pCollider);
    m_pCollider->SetOwner(shared_from_this());

    //**************SkillSet*****************//

    m_pSkillSet = CMonsterSkillSet::Create(1, m_pModelCom);
    m_pSkillSet->SetTransform(m_pTransformCom);

    return S_OK;
}

void CEnemy02::PriorityTick(_float _fTimeDelta)
{
}

void CEnemy02::Tick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;

    __super::Tick(_fTimeDelta);


    if (m_bDie) {

        m_fDissolveAccTime += _fTimeDelta;

    }

    //공격 쿨타임일때
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

                //중복기상 때문에 제거 처리 해놨는데 혹시 나중에 문제가 생긴다면 여기 확인해볼것 
                m_NextAnimIndex.clear();
                ChangeAnim(12, false);
                m_iLastHitIndex = 100;
                m_iCurrentSkillOrderIndex = 100;
                ResetState();
                m_bDown = false;
            }
         

        }


    }

    if (m_bKnockUp) {
        m_bJump = true;
        OnHitKnockUp(_fTimeDelta);
    }


    if (m_bDrop) {
        m_bJump = true;
        OnHitDrop(_fTimeDelta);
    }

    ////플레이어가 가까워지면 방어모드 on 
    //if ((EMONSTER_STATE::STATE_IDLE == m_eCurrentState ||
    //    EMONSTER_STATE::STATE_WALK == m_eCurrentState)&&
    //    m_bDefenceAble && CalcPlayerDistanceReturn() < 3.f) {

    //    m_bDefenceMode = true;
    //    m_bDefenceAble = false;

    //    ChangeAnim(10, true);
    //}



    //방어 브레이크 이후 스턴 상태 
    if (EMONSTER_STATE::STATE_STUN == m_eCurrentState && !m_bDie) {

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
        EMONSTER_STATE::STATE_STUN != m_eCurrentState && !m_bHit && !m_bDie && 
        EMONSTER_STATE::STATE_SPAWN != m_eCurrentState) {

        ChasePlayer();

       if (!m_IsNearPlr) {

           if(!m_bWait)
           {
               m_eCurrentState = EMONSTER_STATE::STATE_WALK;
               ChangeAnim(62, true);
               m_pTransformCom->GoStraight(_fTimeDelta, m_pNavigation, m_bJump);
           }
       }
    }


    if (m_bHit) {
        m_pModelCom->SetLinearTime(0.02f);
    }

    if (m_iAnimNum == 16 || m_iAnimNum == 17) {
        m_pModelCom->SetLinearTime(0.01f);
    }
    else {
        m_pModelCom->SetLinearTime(0.05f);
    }

    if (m_pModelCom->PlayAnimation(_fTimeDelta, m_bLoop, &m_vCurrentAnimPos)) {
        
        if (m_iAnimNum == 9) {
            m_pHPBar->SetEnable(false);
            m_IsEnabled = false;
            return;
        }

        CheckReserveAnimList();
    }

   CalcAnimationDistance();


   m_pSkillSet->Tick(_fTimeDelta);


   m_pCollider->Tick(m_pTransformCom->GetWorldMatrix());
   CGameInstance::GetInstance()->AddCollider(CCollisionMgr::COL_MONSTER, m_pCollider);

   _vector vPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
   vPos.m128_f32[1] += 2.5f;

   m_pHPBar->CalcScreenPos(vPos);

}

void CEnemy02::LateTick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;

    if (!m_bHit && !m_bDie && EMONSTER_STATE::STATE_SPAWN != m_eCurrentState) {

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

#ifdef _DEBUG
    CGameInstance::GetInstance()->AddDebugComponent(m_pCollider);
#endif // _DEBUG


}

HRESULT CEnemy02::Render()
{
    if (!m_IsEnabled)
        return S_OK;
   // m_pSkillSet->Render();


    if (FAILED(BindShaderResources()))
        return E_FAIL;


    _uint iNumMeshes = m_pModelCom->GetNumMeshes();

    _uint iPassIdx = 1;


    if (m_bDie) {
        iPassIdx = 4;

        _float fRate = m_fDissolveAccTime / m_fTotalDissolveTime;

        if (FAILED(m_pShader->BindRawValue("g_fDissolveRate", &fRate, sizeof(_float))))
            return E_FAIL;

        if (FAILED(m_pDissolveTexture->BindShaderResource(m_pShader, "g_DissolveTexture", 0)))
            return E_FAIL;

    }

    for (size_t i = 0; i < iNumMeshes; i++) {

        if (FAILED(m_pModelCom->BindMaterialShaderResource(m_pShader, (_uint)i, aiTextureType::aiTextureType_DIFFUSE, "g_DiffuseTexture")))
            return E_FAIL;

        if (FAILED(m_pModelCom->BindBoneMatrices(m_pShader, "g_BoneMatrices", (_uint)i)))
            return E_FAIL;

        if (FAILED(m_pShader->Begin(iPassIdx)))
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

        CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_MON2);
        CGameInstance::GetInstance()->PlayAudio(TEXT("se_EN0006_attack1.wav"), CSoundMgr::CHANNELID::CH_MON2, 1.f);

        m_pSkillSet->SwitchingSkill(CMonsterSkillSet::ESKILLSTATE::MON_SKILL1);
        m_iAtkPattern = _iAtkNum;
        break;

    case 1:
        ChangeAnim(2, m_bLoop);
        CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_MON2);
        CGameInstance::GetInstance()->PlayAudio(TEXT("se_EN0006_attack2_2.wav"), CSoundMgr::CHANNELID::CH_MON2, 1.f);

        m_pSkillSet->SwitchingSkill(CMonsterSkillSet::ESKILLSTATE::MON_SKILL2);
        m_iAtkPattern = _iAtkNum;
        break;

    case 2:
        ChangeAnim(4, m_bLoop);

        CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_MON2);
        CGameInstance::GetInstance()->PlayAudio(TEXT("se_EN0004_attack1_2.wav"), CSoundMgr::CHANNELID::CH_MON2, 1.f);
        m_pSkillSet->SwitchingSkill(CMonsterSkillSet::ESKILLSTATE::MON_SKILL3);
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
    
    if (50 == m_iAnimNum) {
        m_pHPBar->SetEnable(false);
        m_IsEnabled = false;
        return;
    }


    if (m_eCurrentState == EMONSTER_STATE::STATE_SPAWN) {
        m_eCurrentState = EMONSTER_STATE::STATE_IDLE;
        ChangeAnim(20, true);
    }


    if ((m_bHit && !m_bKnockUp && m_bDown) || 
        (m_bHit && !m_bKnockUp && !m_bDown)) {

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

    if (12 == m_iAnimNum) {
        IdlePattern(rand() % 3);
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
        62 == m_iAnimNum || 32 == m_iAnimNum || 50 == m_iAnimNum || 16 == m_iAnimNum || 17 == m_iAnimNum ||
        18 == m_iAnimNum || 19 == m_iAnimNum || 12 == m_iAnimNum) {
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

    m_pHPBar->SetHP(m_iHP);

    if (m_bDownAttack) {
         m_NextAnimIndex.clear();
         ChangeAnim(54, false);

         if (m_bDie) {
             m_NextAnimIndex.push_back({ 50, false });
         }
         else {
             m_NextAnimIndex.push_back({ 50, false });
             m_NextAnimIndex.push_back({ 12, false });
         }


    }else if (m_bKnockUp) {
        if (16 == m_iAnimNum) {
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
    else if (m_bDrop) {
        if (16 == m_iAnimNum) {
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
         
        if (m_bDie) {
            ChangeAnim(9, false);
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

}

void CEnemy02::SetSpawnState()
{
    __super::SetSpawnState();

    m_pHPBar->HPBarReset();
    m_pHPBar->SetEnable(true);

    m_eCurrentState = EMONSTER_STATE::STATE_SPAWN;
    ChangeAnim(37, false);
    m_iHP = 100;

}

shared_ptr<CEnemy02> CEnemy02::Create()
{
	shared_ptr<CEnemy02> pInstance = make_shared<CEnemy02>();

	if(FAILED(pInstance->Initialize()))
		MSG_BOX("Failed to Create : CEnemy02");

	return pInstance;
}