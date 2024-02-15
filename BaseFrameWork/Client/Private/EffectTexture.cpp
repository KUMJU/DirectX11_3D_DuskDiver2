#include "pch.h"
#include "EffectTexture.h"

#include "GameInstance.h"
#include "Shader.h"
#include "Texture.h"

#include "VIRect.h"

#include "VIBuffer_UI.h"

CEffectTexture::CEffectTexture()
{
}

HRESULT CEffectTexture::Initialize(const wstring& _strTextureKey, TEXEFFECT_DESC* _TextureDesc, char* _strName)
{

    m_eEffectType = EFFECT_TYPE::TYPE_TEXTURE;


    CTransform::TRANSFORM_DESC desc = {};

    m_TextureDesc = *_TextureDesc;
    desc.fRotationPerSet = _TextureDesc->fTurnSpeed;
    desc.fSpeedPerSet = 0.f;

    __super::Initialize(&desc);

    ComputeInitData();
    m_pTransformCom->SetState(CTransform::STATE_POSITION, { _TextureDesc->vCenter.x, _TextureDesc->vCenter.y, _TextureDesc->vCenter.z, 1.f });
    m_pTransformCom->SetScaling(_TextureDesc->vStartScale.x, _TextureDesc->vStartScale.y, 1.f);



    m_pTransformCom->RotaitionRollYawPitch(
        XMConvertToRadians(_TextureDesc->vRotation.x),
        XMConvertToRadians(_TextureDesc->vRotation.y),
        XMConvertToRadians(_TextureDesc->vRotation.z));

    m_vColor = _TextureDesc->vColor;

    m_vTurnAxis = XMLoadFloat4(&_TextureDesc->vTurnAxis);

    m_pShader = CGameInstance::GetInstance()->GetShader(TEXT("Shader_VtxPointTex"));
 
    m_pTexture = CGameInstance::GetInstance()->GetTexture(_strTextureKey);
    m_Components.emplace(TEXT("Com_Texture"), m_pTexture);

    m_pVIBuffer = CVIBuffer_UI::Create(CGameInstance::GetInstance()->GetDeviceInfo(),
        CGameInstance::GetInstance()->GetDeviceContextInfo());
    m_Components.emplace(TEXT("Com_VIBuffer"), m_pVIBuffer);

    m_TextureKey = _strTextureKey;
    m_strEffectName = _strName;

    m_fDurationStart = _TextureDesc->vDuration.x;
    m_fDurationEnd = _TextureDesc->vDuration.y;

    return S_OK;
}

void CEffectTexture::PriorityTick(_float _fTimeDelta)
{
}

void CEffectTexture::Tick(_float _fTimeDelta, _matrix _ParentMat)
{

    if (!m_IsEnabled)
        return;

    m_fAccTime += _fTimeDelta;
    m_fTimeDelta = _fTimeDelta;

    if (m_fAccTime >= m_fDurationStart) {

        if (m_fAccTime >= m_fDurationEnd) {
            m_IsEnabled = false;
        }
    }

    m_pTransformCom->Turn(m_vTurnAxis, _fTimeDelta);
    ScaleLerp();
    m_ParentMat = _ParentMat;


}

void CEffectTexture::LateTick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;

    if (FAILED(CGameInstance::GetInstance()->AddRenderGroup(CRenderer::RENDER_NONLIGHT, shared_from_this())))
        return;

}

HRESULT CEffectTexture::Render()
{
    _vector worldMat = m_pTransformCom->GetState(CTransform::STATE_POSITION);
    _vector vNewPos = m_ParentMat.r[3] + XMLoadFloat3(&m_TextureDesc.vCenter);
    m_pTransformCom->SetState(CTransform::STATE_POSITION, vNewPos);
    if (FAILED(m_pTransformCom->BindShaderResource(m_pShader, "g_WorldMatrix")))
        return E_FAIL;

    _float4x4 ViewMat = CGameInstance::GetInstance()->GetTransformFloat4x4(CPipeLine::D3DTS_VIEW);

    if (FAILED(m_pShader->BindMatrix("g_ViewMatrix", &ViewMat)))
        return E_FAIL;

    _float4x4 ProjMat = CGameInstance::GetInstance()->GetTransformFloat4x4(CPipeLine::D3DTS_PROJ);

    if (FAILED(m_pShader->BindMatrix("g_ProjMatrix", &ProjMat)))
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


void CEffectTexture::ComputeInitData()
{
    m_fMiddleTime = m_TextureDesc.fScaleChangeTime;

    _float fStartProcessTime = m_fMiddleTime - m_TextureDesc.vDuration.x;
    _float fEndProcessTime = m_TextureDesc.vDuration.y - m_fMiddleTime;

    //시작~ 중간 차이 계산 
    m_vStartScaleDiff = _float2({ (m_TextureDesc.vMiddleScale.x - m_TextureDesc.vStartScale.x) / fStartProcessTime,
    (m_TextureDesc.vMiddleScale.y - m_TextureDesc.vStartScale.y) / fStartProcessTime });

    //중간 ~ 끝 차이 계산


    m_vEndScaleDiff = _float2({ (m_TextureDesc.vEndScale.x - m_TextureDesc.vMiddleScale.x) / fEndProcessTime,
    (m_TextureDesc.vEndScale.y - m_TextureDesc.vMiddleScale.y) / fEndProcessTime });

}


void CEffectTexture::ScaleLerp()
{
    if (m_fMiddleTime >= m_fAccTime) {
        m_vCurrentScale += XMLoadFloat2(&m_vStartScaleDiff) * m_fTimeDelta;
    }
    else {
        m_vCurrentScale += XMLoadFloat2(&m_vEndScaleDiff) * m_fTimeDelta;
    }

    // SetScaling
    m_pTransformCom->SetScaling(m_vCurrentScale.m128_f32[0],
        m_vCurrentScale.m128_f32[1],
        1.f);



}


void CEffectTexture::ResetEffect() {

    m_vCurrentScale = XMLoadFloat2(&m_TextureDesc.vStartScale);
    m_fAccTime = 0.f;

}

shared_ptr<CEffectTexture> CEffectTexture::Create(const wstring& _strTextureKey, TEXEFFECT_DESC* _TextureDesc, char* _strName)
{

    shared_ptr<CEffectTexture> pInstance = make_shared<CEffectTexture>();

    if (FAILED(pInstance->Initialize(_strTextureKey, _TextureDesc, _strName)))
        MSG_BOX("Failed to Create : CEffectTexture");

    return pInstance;
}
