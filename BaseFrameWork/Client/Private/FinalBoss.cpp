#include "pch.h"
#include "FinalBoss.h"
#include "GameInstance.h"
#include "Shader.h"

#include "Model.h"

#include "Collider.h"

#include "MonsterSkillSet.h"
#include "BossHPBar.h"
#include "SpecialBossPattern.h"

#include "EffectMgr.h"
#include "EffectPreset.h"

#include "GroundCrack.h"


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
    m_IsAtkCool = true;

    /********Body Collider 1 ************/
    CCollider::COLLIDER_DESC ColliderDesc = {};
    ColliderDesc.fRadius = 2.5f;
    ColliderDesc.vCenter = { -5.f, 0.6f, 7.f };

    m_pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_SPHERE, ColliderDesc);
    m_Components.emplace(TEXT("Com_Collider"), m_pCollider);
    m_pCollider->SetOwner(shared_from_this());


    /********Body Collider 1 ************/

    ColliderDesc.vCenter = { 5.f, 0.6f, 9.5f };

    m_pCollider2 = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_SPHERE, ColliderDesc);
    m_Components.emplace(TEXT("Com_Collider2"), m_pCollider2);
    m_pCollider2->SetOwner(shared_from_this());

    m_eObjType = EObjType::OBJ_BOSSMONSTER;


    m_pSpecialPattern = CSpecialBossPattern::Create(dynamic_pointer_cast<CFinalBoss>(shared_from_this()));
    CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_Event"), m_pSpecialPattern);

    m_pSkillSet = CMonsterSkillSet::Create(3, m_pModelCom);
    m_pSkillSet->SetTransform(m_pTransformCom);

    m_iMaxHP = 200;
    m_iHP = m_iMaxHP;

    m_pHPBar = CBossHPBar::Create();
    m_pHPBar->SetMaxHP(m_iHP);
    m_pHPBar->SetEnable(false);
    CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_UI"), m_pHPBar);


    /* */

    m_pLaserEffectPreset = CEffectMgr::GetInstance()->FindEffect(TEXT("BossAtk4Ready"));
    m_pLaserEffectPreset->SetParentTransform(m_pTransformCom);

    m_pLaserEffectPreset2 = CEffectMgr::GetInstance()->FindEffect(TEXT("BossAtk4"));
    m_pLaserEffectPreset2->SetParentTransform(m_pTransformCom);


    m_pSpawnEffect = CEffectMgr::GetInstance()->FindEffect(TEXT("BossSpawnWind"));
    m_pSpawnEffect->SetParentTransform(m_pTransformCom);
    m_pSpawnEffect->SetEnable(false);
    CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_Effect"), m_pSpawnEffect);

    m_pModelCom->SetLinearTime(1.2f);

    return S_OK;
}

void CFinalBoss::PriorityTick(_float _fTimeDelta)
{
}

void CFinalBoss::Tick(_float _fTimeDelta)
{

    if (!m_IsEnabled)
        return;

    __super::Tick(_fTimeDelta);


    if (m_bSuperArmor) {
        CalcSuperArmorTimeBoss(_fTimeDelta);
    }

    if (m_eCurrentState != EMONSTER_STATE::STATE_SPAWN && !m_IsAtkCool && m_eCurrentState != EMONSTER_STATE::STATE_ATTACK && !m_bDie&&
        m_eCurrentState != EMONSTER_STATE::STATE_STUN) {
        _uint iRand = m_iTestNum % 4;
        AttackPattern(iRand);
        m_eCurrentState = EMONSTER_STATE::STATE_ATTACK;
    }


    if (!m_bSetOriginLook && !m_bDie && m_eCurrentState != EMONSTER_STATE::STATE_STUN && m_eCurrentState != EMONSTER_STATE::STATE_SPAWN) {
        _vector vLook = m_pTransformCom->GetState(CTransform::STATE_LOOK);

        m_fTurnTime += _fTimeDelta;

        if (m_fTurnTime > 2.f) {

            m_bSetOriginLook = true;
            m_fTurnTime = 0.f;
 
            _vector vPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
            vPos = vPos + _vector({0.f, 0.f, 1.f, 0.f});
            m_pTransformCom->LookAtForLandObject(vPos);

        }
        else {
            vLook= XMVectorLerp(vLook, { 0.f, 0.f, 1.f ,0.f }, m_fTurnTime /2.f);

            _vector vPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
            vPos = vPos + vLook;
            m_pTransformCom->LookAtForLandObject(vPos);
        }

    }


    if (4 == m_iAnimNum && !m_bDie && m_eCurrentState != EMONSTER_STATE::STATE_STUN && m_eCurrentState != EMONSTER_STATE::STATE_SPAWN) {
       
        m_fPatternCheckTime += _fTimeDelta;

        //플레이어 조준 완료, 레이저 발사 
        if (!m_bLaserOn && m_fPatternCheckTime> 6.f) {

            if (!m_bLaserSeDone) {
                CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_MON_SE);
                CGameInstance::GetInstance()->PlayAudio(TEXT("se_EN0302_freeze.wav"), CSoundMgr::CHANNELID::CH_MON_SE, 1.f);

                m_bLaserSeDone = true;
            }

            m_pSkillSet->SwitchingSkill(CMonsterSkillSet::MON_SKILL4);
            m_pLaserEffectPreset2->PlayEffect();
            m_bLaserOn = true;

        }

        if (!m_bLaserOn && m_fPatternCheckTime < 4.5f) {


            if (!m_bLaserGaugeSeDone) {
                CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_MON_SE);
                CGameInstance::GetInstance()->PlayAudio(TEXT("se_EN0301_attack4_1.wav"), CSoundMgr::CHANNELID::CH_MON_SE, 1.f);

                m_bLaserGaugeSeDone = true;
            }
       

            m_pLaserEffectPreset->Tick(_fTimeDelta);
            CalcPlayerDistance();
        }
        
        if (m_bLaserOn) {
            m_pLaserEffectPreset2->Tick(_fTimeDelta);
        }

        if (m_fPatternCheckTime >= m_fPattern4DelayTime) {

            ChangeAnim(5, false);
            m_bLaserOn = false;
            m_fPatternCheckTime = 0;
      
           
        }
    }


    if (m_pModelCom->PlayAnimation(_fTimeDelta, m_bLoop, &m_vCurrentAnimPos)) {

        m_vPrevAnimPos = { 0.f, 0.f, 0.f };
        m_vCurrentAnimPos = { 0.f, 0.f, 0.f };
        CheckReserveAnimList();
    }



    if (m_eCurrentState != EMONSTER_STATE::STATE_ATTACK) {

        m_pCollider->Tick(m_pTransformCom->GetWorldMatrix());
        m_pCollider2->Tick(m_pTransformCom->GetWorldMatrix());

        CGameInstance::GetInstance()->AddCollider(CCollisionMgr::COL_MONSTER, m_pCollider);
        CGameInstance::GetInstance()->AddCollider(CCollisionMgr::COL_MONSTER, m_pCollider2);
    }

    m_pSkillSet->Tick(_fTimeDelta);

}

void CFinalBoss::LateTick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;

    if (m_IsAtkCool && !m_bDie && m_eCurrentState != EMONSTER_STATE::STATE_STUN && m_eCurrentState != EMONSTER_STATE::STATE_SPAWN) {

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

    if (4 == m_iAnimNum && !m_bDie && m_eCurrentState != EMONSTER_STATE::STATE_STUN && m_eCurrentState != EMONSTER_STATE::STATE_SPAWN) {

        if (!m_bLaserOn) {
            m_pLaserEffectPreset->LateTick(_fTimeDelta);
        }

        if (m_bLaserOn) {
            m_pLaserEffectPreset2->LateTick(_fTimeDelta);
        }
    }

	if (FAILED(m_pGameInstance->AddRenderGroup(CRenderer::RENDER_NONBLEND, shared_from_this())))
		return;


#ifdef _DEBUG
    CGameInstance::GetInstance()->AddDebugComponent(m_pCollider);
    CGameInstance::GetInstance()->AddDebugComponent(m_pCollider2);

#endif
    m_pSkillSet->LateTick(_fTimeDelta);

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

        if (FAILED(m_pShader->Begin(1)))
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
        m_pSkillSet->SwitchingSkill(CMonsterSkillSet::MON_SKILL1);

        break;

    case 1:
        ChangeAnim(1, false);
        m_pSkillSet->SwitchingSkill(CMonsterSkillSet::MON_SKILL2);

        break;

    case 2:


        CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_MON);
        CGameInstance::GetInstance()->PlayAudio(TEXT("se_EN0301_attack5_1.wav"), CSoundMgr::CHANNELID::CH_MON, 1.f);


        CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_MON_SE);
        CGameInstance::GetInstance()->PlayAudio(TEXT("se_EN0301_attack5_2.wav"), CSoundMgr::CHANNELID::CH_MON_SE, 1.f);


        ChangeAnim(2, false);
        m_pSkillSet->SwitchingSkill(CMonsterSkillSet::MON_SKILL3);


        break;

    case 3:

        m_bLaserSeDone = false;
        m_bLaserGaugeSeDone = false;

        ChangeAnim(3, false);
        m_NextAnimIndex.push_back({ 4 , true });
        m_vOriginLookVec = m_pTransformCom->GetState(CTransform::STATE_LOOK);
        m_pLaserEffectPreset->PlayEffect();
        break;

    case 4:
        ChangeAnim(8, false);
        m_pSkillSet->SwitchingSkill(CMonsterSkillSet::MON_SKILL5);
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

    if (8 == m_iAnimNum && EMONSTER_STATE::STATE_SPAWN == m_eCurrentState) {
        m_eCurrentState = EMONSTER_STATE::STATE_IDLE;
        m_pHPBar->SetEnable(true);
    }

    ChangeAnim(13, true);

}


void CFinalBoss::CalcSuperArmorTimeBoss(_float _fTimeDelta)
{
    m_bSuperArmorCoolTime += _fTimeDelta;

    if (m_bSuperArmorCoolTime > 0.13f) {
        m_bSuperArmorCoolTime = 0.f;
        m_bSuperArmor = false;
    }
}


_bool CFinalBoss::CalcDistanceOption()
{
    return true;
}

void CFinalBoss::StartSpecialPattern()
{
    m_bSpecialPatternStart = true;
    m_eCurrentState = EMONSTER_STATE::STATE_STUN;

    m_vRimColor = { 1.f , 0.8f, 0.1f };

    ChangeAnim(16, false);
    m_NextAnimIndex.push_back({ 15, true });
    
    m_bSuperArmor = false;
    m_pSpecialPattern->PatternStart();
    
}

void CFinalBoss::EndSpecialPattern()
{
    //m_eCurrentState = EMONSTER_STATE::STATE_IDLE;
    //ChangeAnim(14, false);

}

void CFinalBoss::SetSpawnState()
{
    m_pSpawnEffect->PlayEffect();
    m_eCurrentState = EMONSTER_STATE::STATE_SPAWN;
    ChangeAnim(8, false);
    m_iHP = 200;

}

void CFinalBoss::OnHit()
{
    if (m_eCurrentState == EMONSTER_STATE::STATE_STUN)
        return;

    if (m_bDie) {
        m_bDie = false;
        StartSpecialPattern();
    }


    m_pHPBar->SetHP(m_iHP);

    //if (m_iHP < 50 && !m_bSpecialPatternStart) {
    //    StartSpecialPattern();
    //}

}

shared_ptr<CFinalBoss> CFinalBoss::Create()
{
	shared_ptr<CFinalBoss> pInstance = make_shared<CFinalBoss>();

	if (FAILED(pInstance->Initialize()))
		MSG_BOX("Failed to Create : CFinalBoss");

	return pInstance;
}
