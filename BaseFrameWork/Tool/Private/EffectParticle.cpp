#include "pch.h"
#include "EffectParticle.h"

#include "GameInstance.h"
#include "VIBufferPoint.h"

#include "Shader.h"
#include "Texture.h"

CEffectParticle::CEffectParticle()
{
}

HRESULT CEffectParticle::Initialize(_uint _iInstanceNum, const wstring& _strTextureKey, CVIBufferInstancing::INSTANCE_DESC* _desc, char* _strName)
{

    if (FAILED(__super::Initialize(nullptr)))
        return E_FAIL;

    m_pTexture = CGameInstance::GetInstance()->GetTexture(_strTextureKey);
    m_Components.emplace(TEXT("Com_Texture"), m_pTexture);

    m_pShader = CGameInstance::GetInstance()->GetShader(TEXT("Shader_VtxInstance"));

    m_pVIBufferCom = CVIBufferPoint::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), _iInstanceNum, _desc);
    m_Components.emplace(TEXT("Com_VIBuffer"), m_pVIBufferCom);

    m_strEffectName = _strName;

    return S_OK;
}

void CEffectParticle::PriorityTick(_float _fTimeDelta)
{
    if (m_IsEnabled == false)
        return;

}

void CEffectParticle::Tick(_float _fTimeDelta)
{
    if (m_IsEnabled == false)
        return;

    if (m_pVIBufferCom->TickInstance(_fTimeDelta))
        m_IsEnabled = false;


}

void CEffectParticle::LateTick(_float _fTimeDelta)
{
    if (m_IsEnabled == false)
        return;

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

void CEffectParticle::ParsingData(ofstream& _fp)
{
    //Json Parsing  




}

void CEffectParticle::ResetEffect()
{
    m_pVIBufferCom->ResetInstance();
}

shared_ptr<CEffectParticle> CEffectParticle::Create(_uint _iInstanceNum, const wstring& _strTextureKey, CVIBufferInstancing::INSTANCE_DESC* _desc, char* _strName)
{
    shared_ptr<CEffectParticle> pInstance = make_shared<CEffectParticle>();

    if (FAILED(pInstance->Initialize(_iInstanceNum, _strTextureKey, _desc , _strName)))
        MSG_BOX("Failed to Create : CEffectParticle");

    return pInstance;
}
