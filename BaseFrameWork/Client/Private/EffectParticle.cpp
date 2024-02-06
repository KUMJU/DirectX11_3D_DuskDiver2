#include "pch.h"
#include "EffectParticle.h"

#include "GameInstance.h"
#include "VIBufferPoint.h"

#include "Shader.h"
#include "Texture.h"

CEffectParticle::CEffectParticle()
{
}

HRESULT CEffectParticle::Initialize(CTransform::TRANSFORM_DESC* _pDesc)
{

    if (FAILED(__super::Initialize(nullptr)))
        return E_FAIL;

    m_pTexture = CGameInstance::GetInstance()->GetTexture(TEXT("Snow"));
    m_Components.emplace(TEXT("Com_Texture"), m_pTexture);

    m_pShader = CGameInstance::GetInstance()->GetShader(TEXT("Shader_VtxInstance"));

    CVIBufferPoint::INSTANCE_DESC			InstanceDesc{};
    InstanceDesc.vPivot = _float3(0.f, 9.f, 0.f);
    InstanceDesc.vCenter = _float3(0.0f, 50.f, 0.f);
    InstanceDesc.vRange = _float3(300.0f, 5.0f, 300.0f);
    InstanceDesc.vSize = _float2(0.2f, 0.7f);
    InstanceDesc.vSpeed = _float2(1.f, 3.f);
    InstanceDesc.vLifeTime = _float2(20.f, 50.f);
    InstanceDesc.isLoop = true;
    InstanceDesc.vColor = _float4(1.f, 1.f, 1.f, 1.f);

    m_pVIBufferCom = CVIBufferPoint::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), 1000, &InstanceDesc);
    m_Components.emplace(TEXT("Com_VIBuffer"), m_pVIBufferCom);

    return S_OK;
}

void CEffectParticle::PriorityTick(_float _fTimeDelta)
{
}

void CEffectParticle::Tick(_float _fTimeDelta)
{
    m_pVIBufferCom->TickDrop(_fTimeDelta);

}

void CEffectParticle::LateTick(_float _fTimeDelta)
{
    if (FAILED(CGameInstance::GetInstance()->AddRenderGroup(CRenderer::RENDER_NONBLEND, shared_from_this())))
        return;
}

HRESULT CEffectParticle::Render()
{
    if (FAILED(m_pTransformCom->BindShaderResource(m_pShader, "g_WorldMatrix")))
        return E_FAIL;

    _float4x4 ViewMat = CGameInstance::GetInstance()->GetTransformFloat4x4(CPipeLine::D3DTS_VIEW);

    if (FAILED(m_pShader->BindMatrix("g_ViewMatrix", &ViewMat)))
        return E_FAIL;

    _float4x4 ProjMat = CGameInstance::GetInstance()->GetTransformFloat4x4(CPipeLine::D3DTS_PROJ);

    if (FAILED(m_pShader->BindMatrix("g_ProjMatrix", &ProjMat)))
        return E_FAIL;

    _float4 CamPos = CGameInstance::GetInstance()->GetCamPosition();

    if (FAILED(m_pShader->BindRawValue("g_vCamPosition", &CamPos, sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pTexture->BindShaderResource(m_pShader, "g_Texture", 0)))
        return E_FAIL;

    if (FAILED(m_pShader->Begin(0)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    return S_OK;
}

shared_ptr<CEffectParticle> CEffectParticle::Create()
{
    shared_ptr<CEffectParticle> pInstance = make_shared<CEffectParticle>();

    if (FAILED(pInstance->Initialize(nullptr)))
        MSG_BOX("Failed to Create : CEffectParticle");

    return pInstance;
}
