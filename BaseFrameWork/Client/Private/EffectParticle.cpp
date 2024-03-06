#include "pch.h"
#include "EffectParticle.h"

#include "GameInstance.h"
#include "VIBufferPoint.h"

#include "Shader.h"
#include "Texture.h"
CEffectParticle::CEffectParticle()
{
}

CEffectParticle::CEffectParticle(const CEffectParticle& _rhs)
    :m_InstanceDesc(_rhs.m_InstanceDesc),
    m_TextureKey(_rhs.m_TextureKey)
{
}

HRESULT CEffectParticle::Initialize(_uint _iInstanceNum, const wstring& _strTextureKey, CVIBufferInstancing::INSTANCE_DESC* _desc, char* _strName, _bool _bLoop)
{

    m_InstanceDesc = *_desc;

    if (FAILED(__super::Initialize(nullptr)))
        return E_FAIL;

    m_pTexture = CGameInstance::GetInstance()->GetTexture(_strTextureKey);
    m_Components.emplace(TEXT("Com_Texture"), m_pTexture);

    m_pShader = CGameInstance::GetInstance()->GetShader(TEXT("Shader_VtxInstance"));

    m_pVIBufferCom = CVIBufferPoint::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), _iInstanceNum, _desc);
    m_Components.emplace(TEXT("Com_VIBuffer"), m_pVIBufferCom);

    m_iInstanceNum = _iInstanceNum;
    m_strEffectName = _strName;

    m_TextureKey = _strTextureKey;

    m_eEffectType = EFFECT_TYPE::TYPE_PARTICLE;
    m_bLoop = _bLoop;

    return S_OK;
}

void CEffectParticle::PriorityTick(_float _fTimeDelta)
{
    if (m_IsEnabled == false)
        return;

}

void CEffectParticle::Tick(_float _fTimeDelta, _matrix _ParentMat)
{
    if (m_IsEnabled == false)
        return;


    if (m_pVIBufferCom->TickInstance(_fTimeDelta))
        m_IsEnabled = false;

    m_ParentMat = _ParentMat;
    


}

void CEffectParticle::LateTick(_float _fTimeDelta)
{
    if (m_IsEnabled == false)
        return;

    if (FAILED(CGameInstance::GetInstance()->AddRenderGroup(CRenderer::RENDER_NONLIGHT, shared_from_this())))
        return;

    if (FAILED(CGameInstance::GetInstance()->AddRenderGroup(CRenderer::RENDER_GLOW, shared_from_this())))
        return;
}

HRESULT CEffectParticle::Render()
{
    _matrix WorldOwnMatrix = m_pTransformCom->GetWorldMatrix();
    _float4x4 FloatWorld;

    WorldOwnMatrix.r[3] += m_vParentPos;
    WorldOwnMatrix = WorldOwnMatrix * m_ParentMat;

    XMStoreFloat4x4(&FloatWorld, WorldOwnMatrix);

    if (FAILED(m_pShader->BindMatrix("g_WorldMatrix", &FloatWorld)))
        return E_FAIL;


    //m_pTransformCom->SetState(CTransform::STATE_POSITION, m_ParentMat.r[3]);
   // XMStoreFloat4x4(&matWorld, m_ParentMat);

    /*if (FAILED(m_pShader->BindMatrix("g_WorldMatrix", &matWorld)))
        return E_FAIL;*/

    //if (FAILED(m_pTransformCom->BindShaderResource(m_pShader, "g_WorldMatrix")))
    //    return E_FAIL;

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

HRESULT CEffectParticle::RenderGlow(shared_ptr<CShader> _pShader)
{
    _matrix WorldOwnMatrix = m_pTransformCom->GetWorldMatrix();
    _float4x4 FloatWorld;

    WorldOwnMatrix.r[3] += m_vParentPos;
    WorldOwnMatrix = WorldOwnMatrix * m_ParentMat;

    XMStoreFloat4x4(&FloatWorld, WorldOwnMatrix);

    if (FAILED(m_pShader->BindMatrix("g_WorldMatrix", &FloatWorld)))
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

    _float4 vGlowColor = _float4(0.38, 1.0, 0.98, 1.f);
        
    if (FAILED(m_pShader->BindRawValue("g_vGlowColor", &vGlowColor, sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pTexture->BindShaderResource(m_pShader, "g_Texture", 0)))
        return E_FAIL;

    if (FAILED(m_pShader->Begin(1)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    return S_OK;
}

void CEffectParticle::ResetEffect()
{
    m_bSetParentMat = false;
    m_pVIBufferCom->ResetInstance();
}

shared_ptr<CEffectParticle> CEffectParticle::Create(_uint _iInstanceNum, const wstring& _strTextureKey, CVIBufferInstancing::INSTANCE_DESC* _desc, char* _strName, _bool _bLoop)
{
    shared_ptr<CEffectParticle> pInstance = make_shared<CEffectParticle>();

    if (FAILED(pInstance->Initialize(_iInstanceNum, _strTextureKey, _desc, _strName, _bLoop)))
        MSG_BOX("Failed to Create : CEffectParticle");

    return pInstance;
}

shared_ptr<CEffect> CEffectParticle::CloneEffect()
{
    shared_ptr<CEffectParticle> pInstance = make_shared<CEffectParticle>();

    if (FAILED(pInstance->Initialize(m_iInstanceNum, m_TextureKey, &m_InstanceDesc, m_strEffectName, m_bLoop))) {
        MSG_BOX("Failed to Clone : CModel");
    }

    return pInstance;
}
