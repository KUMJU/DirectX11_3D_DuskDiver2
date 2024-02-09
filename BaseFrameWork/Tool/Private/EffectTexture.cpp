#include "pch.h"
#include "EffectTexture.h"

#include "GameInstance.h"
#include "Shader.h"
#include "Texture.h"

#include "VIBuffer_UI.h"

CEffectTexture::CEffectTexture()
{
}

HRESULT CEffectTexture::Initialize(const wstring& _strTextureKey, TEXEFFECT_DESC* _TextureDesc, char* _strName)
{

    m_eEffectType = EFFECT_TYPE::TYPE_TEXTURE;
    __super::Initialize(nullptr);

    m_pTransformCom->SetState(CTransform::STATE_POSITION, { _TextureDesc->vCenter.x, _TextureDesc->vCenter.y, _TextureDesc->vCenter.z, 1.f });
    m_pTransformCom->SetScaling(_TextureDesc->vScale.x, 1.f , _TextureDesc->vScale.y);

    m_pTransformCom->RotaitionRollYawPitch(
        XMConvertToRadians(_TextureDesc->vRotation.x), 
        XMConvertToRadians(_TextureDesc->vRotation.y), 
        XMConvertToRadians(_TextureDesc->vRotation.z));
    m_vColor = _TextureDesc->vColor;

    m_pShader = CGameInstance::GetInstance()->GetShader(TEXT("Shader_VtxPointTex"));
 
    m_pTexture = CGameInstance::GetInstance()->GetTexture(_strTextureKey);
    m_Components.emplace(TEXT("Com_Texture"), m_pTexture);


    m_pVIBuffer = CVIBuffer_UI::Create(CGameInstance::GetInstance()->GetDeviceInfo(),
        CGameInstance::GetInstance()->GetDeviceContextInfo());
    m_Components.emplace(TEXT("Com_VIBuffer"), m_pVIBuffer);

    return S_OK;
}

void CEffectTexture::PriorityTick(_float _fTimeDelta)
{
}

void CEffectTexture::Tick(_float _fTimeDelta)
{
    //Parent 객체가 있다면 그대로 따라간다


}

void CEffectTexture::LateTick(_float _fTimeDelta)
{
    if (FAILED(CGameInstance::GetInstance()->AddRenderGroup(CRenderer::RENDER_NONBLEND, shared_from_this())))
        return;

}

HRESULT CEffectTexture::Render()
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

    if (FAILED(m_pShader->BindRawValue("g_RGBColor", &m_vColor, sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pTexture->BindShaderResource(m_pShader, "g_Texture", 0)))
        return E_FAIL;

    if (FAILED(m_pShader->Begin(0)))
        return E_FAIL;

    if (FAILED(m_pVIBuffer->BindBuffers()))
        return E_FAIL;

    if (FAILED(m_pVIBuffer->Render()))
        return E_FAIL;


    return S_OK;
}

shared_ptr<CEffectTexture> CEffectTexture::Create(const wstring& _strTextureKey, TEXEFFECT_DESC* _TextureDesc, char* _strName)
{

    shared_ptr<CEffectTexture> pInstance = make_shared<CEffectTexture>();

    if (FAILED(pInstance->Initialize(_strTextureKey, _TextureDesc, _strName)))
        MSG_BOX("Failed to Create : CEffectTexture");

    return pInstance;
}
