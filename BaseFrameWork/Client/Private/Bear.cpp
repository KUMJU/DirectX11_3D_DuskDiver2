#include "pch.h"
#include "Bear.h"

#include "Shader.h"
#include "Model.h"
#include "Collider.h"

#include "GameInstance.h"


CBear::CBear()
{
}

HRESULT CBear::Initialize()
{
    CTransform::TRANSFORM_DESC TransformDesc = {};
    TransformDesc.fSpeedPerSet = 0.f;
    TransformDesc.fRotationPerSet = 2.f;

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


    return S_OK;
}

void CBear::PriorityTick(_float _fTimeDelta)
{
}

void CBear::Tick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;
    
    if (m_bShaking) {
        m_fShakingAccTime += _fTimeDelta;
        
        if (m_fShakingAccTime >= 0.5f && !m_bChangeDir) {

            m_bChangeDir = true;

        }
        else if (m_fShakingAccTime >= 1.f) {
            m_fShakingAccTime = 0.f;
        }

        Shaking(_fTimeDelta);
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


}

void CBear::SetSpawnState()
{
}

void CBear::Shaking(_float _fTimeDelta)
{
    _float fDistance = m_fShakingSpeed * _fTimeDelta * m_iDir;

    _vector vPos= m_pTransformCom->GetState(CTransform::STATE_POSITION);
    _vector vRight = m_pTransformCom->GetState(CTransform::STATE_RIGHT);

    m_fXOffset += fDistance;

    XMVectorSetX(vRight, vPos.m128_f32[0] + fDistance);

}

void CBear::MovingFront(_vector _vDstPos)
{

    _vector vCurrentPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
    m_vMoveDistancePerTick = ((_vDstPos - vCurrentPos) / 0.2f);
    m_bMove = true;

    m_vDstPos = _vDstPos;
    m_fMovingTime = 0.f;

}

shared_ptr<CBear> CBear::Create()
{
    shared_ptr<CBear> pInstance = make_shared<CBear>();

    if (FAILED(pInstance->Initialize())) {
        MSG_BOX("Failed to Create : CBear");
    }

    return pInstance;
}
