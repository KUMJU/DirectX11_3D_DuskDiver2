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

    m_pCollider->Render();

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

void CBear::OnHit()
{

    int a = 45;

}

void CBear::SetSpawnState()
{
}

shared_ptr<CBear> CBear::Create()
{
    shared_ptr<CBear> pInstance = make_shared<CBear>();

    if (FAILED(pInstance->Initialize())) {
        MSG_BOX("Failed to Create : CBear");
    }

    return pInstance;
}
