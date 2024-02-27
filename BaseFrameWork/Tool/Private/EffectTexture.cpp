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

    m_TextureDesc = *_TextureDesc;

    CTransform::TRANSFORM_DESC desc = {};
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


    m_vRowUVRatio.x = 0.f;
    m_vRowUVRatio.y = (_float)(1.f) / (_float)_TextureDesc->iRow;

    m_vColUVRatio.x = 0.f;
    m_vColUVRatio.y = (_float)(1.f) / (_float)_TextureDesc->iCol;


    return S_OK;
}

void CEffectTexture::PriorityTick(_float _fTimeDelta)
{
}

void CEffectTexture::Tick(_float _fTimeDelta)
{

    if (!m_IsEnabled)
        return;

    m_fAccTime += _fTimeDelta;
    m_fTimeDelta = _fTimeDelta;

    if (m_fAccTime >= m_fDurationStart) {

        if (m_fAccTime >= m_fDurationEnd) {
            m_IsEnabled = false;
        }

        if (m_TextureDesc.fScrollTime != 0) {

            m_fSequenceAccTime += _fTimeDelta;

            if (m_fSequenceAccTime >= m_TextureDesc.fScrollTime) {

                SlideNextTexIdx();
                m_fSequenceAccTime = 0.f;

            }

        }


        if (m_vTurnAxis.m128_f32[0] != 0 &&
            m_vTurnAxis.m128_f32[1] != 0 &&
            m_vTurnAxis.m128_f32[2] != 0) {
            m_pTransformCom->Turn(m_vTurnAxis, _fTimeDelta);
        }

        ScaleLerp();
    }

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

    _int iPassNum = 0;

    _matrix WorldOwnMatrix = m_pTransformCom->GetWorldMatrix();

    //시퀀스 텍스쳐 
    if (m_TextureDesc.bSequenceTex) {


        _float4x4 ViewCopyMat = ViewMat;

        ViewCopyMat._41 = 0.f;
        ViewCopyMat._42 = 0.f;
        ViewCopyMat._43 = 0.f;

        _matrix ViewInv = {};

        ViewInv = XMMatrixInverse(NULL, XMLoadFloat4x4(&ViewCopyMat));
        ViewInv = WorldOwnMatrix * ViewInv;
        ViewInv.r[3] = WorldOwnMatrix.r[3];

        _float4x4 BillboardWorld = {};
        XMStoreFloat4x4(&BillboardWorld, ViewInv);

        if (FAILED(m_pShader->BindMatrix("g_WorldMatrix", &BillboardWorld)))
            return E_FAIL;


        if (FAILED(m_pShader->BindRawValue("g_fStartRowUV", &m_vRowUVRatio.x, sizeof(_float))))
            return E_FAIL;

        if (FAILED(m_pShader->BindRawValue("g_fEndRowUV", &m_vRowUVRatio.y, sizeof(_float))))
            return E_FAIL;

        if (FAILED(m_pShader->BindRawValue("g_fStartColUV", &m_vColUVRatio.x, sizeof(_float))))
            return E_FAIL;

        if (FAILED(m_pShader->BindRawValue("g_fEndColUV", &m_vColUVRatio.y, sizeof(_float))))
            return E_FAIL;

        iPassNum = 1;
    }


    if (FAILED(m_pTexture->BindShaderResource(m_pShader, "g_Texture", 0)))
        return E_FAIL;

    if (FAILED(m_pShader->Begin(iPassNum)))
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

void CEffectTexture::ParsingData(Json::Value& _root)
{

    Json::Value EffectInfo;


    char* strMultiByte = new char[256];
    WideCharToMultiByte(CP_ACP, 0, m_TextureKey.c_str(), -1, strMultiByte, 256, NULL, NULL);

    //Texture
    EffectInfo["EffectType"] = 1;
    EffectInfo["TextureKey"] = strMultiByte;

    Json::Value StartScale;

    StartScale["x"] = m_TextureDesc.vStartScale.x;
    StartScale["y"] = m_TextureDesc.vStartScale.y;

    EffectInfo["StartScale"] = StartScale;

    Json::Value MiddleScale;

    MiddleScale["x"] = m_TextureDesc.vMiddleScale.x;
    MiddleScale["y"] = m_TextureDesc.vMiddleScale.y;

    EffectInfo["MiddleScale"] = MiddleScale;

    Json::Value EndScale;

    EndScale["x"] = m_TextureDesc.vEndScale.x;
    EndScale["y"] = m_TextureDesc.vEndScale.y;

    EffectInfo["EndScale"] = EndScale;


    Json::Value Rotation;

    Rotation["x"] = m_TextureDesc.vRotation.x;
    Rotation["y"] = m_TextureDesc.vRotation.y;
    Rotation["z"] = m_TextureDesc.vRotation.z;

    EffectInfo["Rotation"] = Rotation;

    Json::Value Center;

    Center["x"] = m_TextureDesc.vCenter.x;
    Center["y"] = m_TextureDesc.vCenter.y;
    Center["z"] = m_TextureDesc.vCenter.z;

    EffectInfo["Center"] = Center;

    Json::Value Color;
    
    Color["x"] = m_TextureDesc.vColor.x;
    Color["y"] = m_TextureDesc.vColor.y;
    Color["z"] = m_TextureDesc.vColor.z;
    Color["w"] = m_TextureDesc.vColor.w;

    EffectInfo["Color"] = Color;

    Json::Value Duration;

    Duration["x"] = m_TextureDesc.vDuration.x;
    Duration["y"] = m_TextureDesc.vDuration.y;

    EffectInfo["Duration"] = Duration;

    Json::Value Speed;

    EffectInfo["Speed"] = m_TextureDesc.fTurnSpeed;


    Json::Value Axis;

    Axis["x"] = m_TextureDesc.vTurnAxis.x;
    Axis["y"] = m_TextureDesc.vTurnAxis.y;
    Axis["z"] = m_TextureDesc.vTurnAxis.z;

    EffectInfo["Axis"] = Axis;

    Json::Value ScaleChageTime;

    EffectInfo["ScaleChageTime"] = m_TextureDesc.fScaleChangeTime;
  
    Json::Value Loop;
    Loop["Loop"] = m_TextureDesc.bLoop;
    EffectInfo["Loop"] = Loop;

    _root[m_strEffectName] = EffectInfo;


}

void CEffectTexture::ResetEffect() {

    m_vCurrentScale = XMLoadFloat2(&m_TextureDesc.vStartScale);
    m_fAccTime = 0.f;

    m_iCurrentRow = 0;
    m_iCurrentCol = 0;
    

}

void CEffectTexture::EditDesc(const wstring& _strTextureKey, TEXEFFECT_DESC _desc)
{
    //각도 초기화 
    m_pTransformCom->RotaitionRollYawPitch(
        XMConvertToRadians(-1.f * m_TextureDesc.vRotation.x),
        XMConvertToRadians(-1.f * m_TextureDesc.vRotation.y),
        XMConvertToRadians(-1.f * m_TextureDesc.vRotation.z));

    ResetEffect();

    m_TextureDesc = _desc;
    m_pTransformCom->SetTurnSpeed(_desc.fTurnSpeed);

    //재계산
    ComputeInitData();

    m_pTransformCom->SetState(CTransform::STATE_POSITION, { _desc.vCenter.x, _desc.vCenter.y, _desc.vCenter.z, 1.f });
    m_pTransformCom->SetScaling(_desc.vStartScale.x, _desc.vStartScale.y, 1.f);

    m_pTransformCom->RotaitionRollYawPitch(
        XMConvertToRadians(_desc.vRotation.x),
        XMConvertToRadians(_desc.vRotation.y),
        XMConvertToRadians(_desc.vRotation.z));

    m_vColor = _desc.vColor;
    m_vTurnAxis = XMLoadFloat4(&_desc.vTurnAxis);

    m_pShader = CGameInstance::GetInstance()->GetShader(TEXT("Shader_VtxPointTex"));

    m_pTexture = CGameInstance::GetInstance()->GetTexture(_strTextureKey);
    m_Components.emplace(TEXT("Com_Texture"), m_pTexture);

    m_pVIBuffer = CVIBuffer_UI::Create(CGameInstance::GetInstance()->GetDeviceInfo(),
        CGameInstance::GetInstance()->GetDeviceContextInfo());
    m_Components.emplace(TEXT("Com_VIBuffer"), m_pVIBuffer);

    m_TextureKey = _strTextureKey;

    m_fDurationStart = _desc.vDuration.x;
    m_fDurationEnd = _desc.vDuration.y;

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

void CEffectTexture::SlideNextTexIdx()
{
    ++m_iCurrentRow;

    //가로 끝까지 갔을 때 
    if (m_TextureDesc.iRow == m_iCurrentRow) {
        m_iCurrentRow = 0;
        ++m_iCurrentCol;

        if (m_TextureDesc.iCol == m_iCurrentCol) {
            m_iCurrentCol = 0;
        }
    }

    m_vRowUVRatio.x = (_float)m_iCurrentRow / (_float)m_TextureDesc.iRow;
    m_vRowUVRatio.y = (_float)(m_iCurrentRow + 1.f) / (_float)m_TextureDesc.iRow;

    m_vColUVRatio.x = (_float)(m_iCurrentCol) / (_float)m_TextureDesc.iCol;
    m_vColUVRatio.y = (_float)(m_iCurrentCol + 1.f) / (_float)m_TextureDesc.iCol;

}

shared_ptr<CEffectTexture> CEffectTexture::Create(const wstring& _strTextureKey, TEXEFFECT_DESC* _TextureDesc, char* _strName)
{

    shared_ptr<CEffectTexture> pInstance = make_shared<CEffectTexture>();

    if (FAILED(pInstance->Initialize(_strTextureKey, _TextureDesc, _strName)))
        MSG_BOX("Failed to Create : CEffectTexture");

    return pInstance;
}
