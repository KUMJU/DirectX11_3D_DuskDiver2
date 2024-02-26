#include "pch.h"
#include "Bear.h"

#include "Shader.h"
#include "Model.h"
#include "Collider.h"

#include "UIMgr.h"
#include "GameInstance.h"
#include "SceneTriggerInsert.h"


CBear::CBear()
{
}

HRESULT CBear::Initialize()
{
    CTransform::TRANSFORM_DESC TransformDesc = {};
    TransformDesc.fSpeedPerSet = 0.f;
    TransformDesc.fRotationPerSet = 3.f;

    if (FAILED(__super::Initialize(&TransformDesc)))
        return E_FAIL;

    m_pTransformCom->SetScaling(6.f, 6.f, 6.f);

    m_pShader = CGameInstance::GetInstance()->GetShader(TEXT("Shader_VtxMesh"));

    m_pModelCom = m_pGameInstance->GetModel(TEXT("Mole_BearA"));
    m_Components.emplace(TEXT("Com_Model"), m_pModelCom);

    m_pHitModel = m_pGameInstance->GetModel(TEXT("Mole_BearAc"));
    m_Components.emplace(TEXT("Com_HitModel"), m_pHitModel);

    m_iHP = 99999;

    /*********Collider*************/

    CCollider::COLLIDER_DESC ColliderDesc = {};
    ColliderDesc.fRadius = 0.2f;
    ColliderDesc.vCenter = { 0.f, 0.1f, 0.f };

    m_pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_SPHERE, ColliderDesc);
    m_Components.emplace(TEXT("Com_Collider"), m_pCollider);
    m_pCollider->SetOwner(shared_from_this());


    m_eMonsterType = EMONSTER_TYPE::MONSTER_NORMAL;

    m_pTrigger = CSceneTriggerInsert::Create({ -3.f, 27.f, -139.f });

    return S_OK;
}

void CBear::PriorityTick(_float _fTimeDelta)
{
}

void CBear::Tick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;
    
    if (m_bTurn) {
        m_pTransformCom->Turn({ 0.f, 1.f, 0.f }, _fTimeDelta);
    }

    if (m_bShaking) {
        m_fShakingAccTime += _fTimeDelta;
        m_fShakingTotalAccTime += _fTimeDelta;

        if (!m_bFirstShaking&& m_fShakingAccTime >= 0.035f) {
            m_iDir *= -1.f;
            m_fShakingAccTime = 0.f;
            m_bFirstShaking = true;
        }
        else {

            if (m_fShakingAccTime >= 0.07f) {
                m_iDir *= -1.f;
                m_fShakingAccTime = 0.f;
            }

        }
        
        if (m_fShakingTotalAccTime >= m_fShakingTotalTime) {
            m_fShakingTotalAccTime = 0.f;
            m_fShakingAccTime = 0.f;
            m_pTransformCom->SetState(CTransform::STATE_POSITION, m_vBeforeShakingPos);
            m_bShaking = false;
            m_bFirstShaking = false;
        }
        else {
            Shaking(_fTimeDelta);
        }

    }


    if (m_bMove) {
        
        m_fMovingTime += _fTimeDelta;


        if (m_fMovingTime >= 0.2f) {

            m_bMove = false;
            m_pTransformCom->SetState(CTransform::STATE_POSITION, m_vDstPos);
            m_fMovingTime = 0.f;
            
        }
        else {
            _vector vCurrentPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
            vCurrentPos += m_vMoveDistancePerTick * _fTimeDelta;
            m_pTransformCom->SetState(CTransform::STATE_POSITION, XMVectorSetW(vCurrentPos, 1.f));
        }


    }

    if (m_bRunaway) {

        m_fDialogAccTime += _fTimeDelta;

        if (m_fDialogAccTime > 8.f && m_fDialogAccTime < 12.f) {
            _float fSpeedPerTick = 18.f * _fTimeDelta;
            _vector vPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);

            vPos.m128_f32[1] += fSpeedPerTick;

            m_pTransformCom->SetState(CTransform::STATE_POSITION, vPos);

        }
        else if (m_fDialogAccTime >= 12.f) {
            m_bRunaway = false;
            m_pTransformCom->Rotation({ 0.f,1.f, 0.f }, XMConvertToRadians(0.f));
            CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_Event"), m_pTrigger);
        }


    }


    m_pCollider->Tick(m_pTransformCom->GetWorldMatrix());
    CGameInstance::GetInstance()->AddCollider(CCollisionMgr::COL_MONSTER, m_pCollider);
}

void CBear::LateTick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;

    if (FAILED(CGameInstance::GetInstance()->AddRenderGroup(CRenderer::RENDER_NONBLEND, shared_from_this())))
        return;

}

HRESULT CBear::Render()
{
    if (!m_IsEnabled)
        return S_OK;

    if (FAILED(BindShaderResources()))
        return E_FAIL;

    _uint iNumMeshes = m_pModelCom->GetNumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++) {

        if (FAILED(m_pModelCom->BindMaterialShaderResource(m_pShader, (_uint)i, aiTextureType::aiTextureType_DIFFUSE, "g_DiffuseTexture")))
            return E_FAIL;

        if (FAILED(m_pShader->Begin(0)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Render((_uint)i)))
            return E_FAIL;
    }

    return S_OK;
}

void CBear::ChangeModel(_int _iModelNum)
{
    if (1 == _iModelNum) {
        m_pModelCom = CGameInstance::GetInstance()->GetModel(TEXT("Mole_BearAa"));

    }
    else if (2 == _iModelNum) {
        m_pModelCom = CGameInstance::GetInstance()->GetModel(TEXT("Mole_FakeBear"));
    }
    else if (3 == _iModelNum) {
        m_pModelCom = m_pHitModel;
    }

}

void CBear::OnHit()
{
    SetShaking(2.f, 0.5f);
    ++m_iHitNum;

    if (m_iHitNum >= 10) {

        CUIMgr::GetInstance()->StartDialog(TEXT("BearCoinQuest"));
        m_bRunaway = true;
        //Dialog
        //m_bRunaway
    }

}

void CBear::SetSpawnState()
{
}

void CBear::Shaking(_float _fTimeDelta)
{
    
    _vector vPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
    _vector vRight = m_pTransformCom->GetState(CTransform::STATE_RIGHT);

    _vector fDistance = m_fShakingSpeed * vRight * _fTimeDelta * m_iDir;


  //  m_fXOffset += fDistance;

    vPos = XMVectorSetW(vPos + fDistance, 1.f);
    m_pTransformCom->SetState(CTransform::STATE_POSITION, vPos);

}

void CBear::MovingFront(_vector _vDstPos)
{

    _vector vCurrentPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
    m_vMoveDistancePerTick = ((_vDstPos - vCurrentPos) / 0.2f);
    m_bMove = true;

    m_vDstPos = _vDstPos;
    m_fMovingTime = 0.f;

}

void CBear::SetShaking(_float _fShakeDistance, _float _fShakeTime)
{
    if (m_bShaking) {
        m_pTransformCom->SetState(CTransform::STATE_POSITION, m_vBeforeShakingPos);
    }

    m_fShakingSpeed = _fShakeDistance;
    m_fShakingTotalTime = _fShakeTime;
    m_bShaking = true;
    m_vBeforeShakingPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
}

shared_ptr<CBear> CBear::Create()
{
    shared_ptr<CBear> pInstance = make_shared<CBear>();

    if (FAILED(pInstance->Initialize())) {
        MSG_BOX("Failed to Create : CBear");
    }

    return pInstance;
}
