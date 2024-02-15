#include "pch.h"
#include "EffectMesh.h"

#include "Shader.h"
#include "Model.h"
#include "Texture.h"

#include "GameInstance.h"


CEffectMesh::CEffectMesh()
{
}

HRESULT CEffectMesh::Initialize(const wstring& _strTextureKey, const wstring& _strModelKey, MESH_DESC* _MeshDesc, char* _strName)
{
    m_MeshDesc = *_MeshDesc;
    m_vCurrentScale = XMLoadFloat3(&m_MeshDesc.vStartScale);

    m_MeshKey = _strModelKey;


    m_eEffectType = EFFECT_TYPE::TYPE_MESH;
    __super::Initialize(nullptr);
    ComputeInitSetting();

    m_pShader = CGameInstance::GetInstance()->GetShader(TEXT("Shader_VtxMesh"));


    m_pModel = CGameInstance::GetInstance()->GetModel(_strModelKey);
    m_Components.emplace(TEXT("Com_Model"), m_pModel);

    _tchar szNoiseKey[MAX_PATH] = TEXT("");
    MultiByteToWideChar(CP_ACP, 0, m_MeshDesc.szNoiseTexKey, (_int)strlen(m_MeshDesc.szNoiseTexKey), szNoiseKey, MAX_PATH);

    _tchar szMaskKey[MAX_PATH] = TEXT("");
    MultiByteToWideChar(CP_ACP, 0, m_MeshDesc.szMaskTexKey, (_int)strlen(m_MeshDesc.szMaskTexKey), szMaskKey, MAX_PATH);

    m_pMaskTexture = CGameInstance::GetInstance()->GetTexture(szMaskKey);
    m_Components.emplace(TEXT("Com_MaskTexture"), m_pTexture);

    m_pNoiseTexture = CGameInstance::GetInstance()->GetTexture(szNoiseKey);
    m_Components.emplace(TEXT("Com_NoiseTexture"), m_pTexture);

    m_pTransformCom->SetScaling(_MeshDesc->vStartScale.x, _MeshDesc->vStartScale.y, _MeshDesc->vStartScale.z);
    m_pTransformCom->SetState(CTransform::STATE_POSITION,  {_MeshDesc->vCenter.x, _MeshDesc->vCenter.y, _MeshDesc->vCenter.z, 1.f });

    m_strEffectName = _strName;

    m_vColor = _MeshDesc->vColor;

    return S_OK;
}

void CEffectMesh::PriorityTick(_float _fTimeDelta)
{
}

void CEffectMesh::Tick(_float _fTimeDelta)
{
    if (!m_IsEnabled) 
        return;
    
    m_fTimeDelta = _fTimeDelta;
    m_fAccTime += _fTimeDelta;

    //아직 시작 시간이 아닐때
    if (m_fAccTime < m_MeshDesc.vDuration.x) {
        return;
    }

    //루프가 아닐때
    if (!m_MeshDesc.bLoop) {
       
        //알파 러프

        if (m_fAccTime >= m_MeshDesc.vDuration.y) {
            m_IsEnabled = false;
            return;
        }

        ScaleLerp();


    }

}

void CEffectMesh::LateTick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;

    if (m_fAccTime < m_MeshDesc.vDuration.x) {
        return;
    }

    if (FAILED(CGameInstance::GetInstance()->AddRenderGroup(CRenderer::RENDER_NONLIGHT, shared_from_this())))
        return;
}

HRESULT CEffectMesh::Render()
{

    if (FAILED(m_pTransformCom->BindShaderResource(m_pShader, "g_WorldMatrix")))
        return E_FAIL;

    _float4x4 ViewMat = CGameInstance::GetInstance()->GetTransformFloat4x4(CPipeLine::D3DTS_VIEW);

    if (FAILED(m_pShader->BindMatrix("g_ViewMatrix", &ViewMat)))
        return E_FAIL;

    _float4x4 ProjMat = CGameInstance::GetInstance()->GetTransformFloat4x4(CPipeLine::D3DTS_PROJ);

    if (FAILED(m_pShader->BindMatrix("g_ProjMatrix", &ProjMat)))
        return E_FAIL;

    if (FAILED(m_pShader->BindRawValue("g_vColor", &m_vColor, sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pShader->BindRawValue("g_vLerpColor", &m_MeshDesc.vLerpColor, sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pShader->BindRawValue("g_bMaskTex", &m_MeshDesc.bMask, sizeof(_bool))))
        return E_FAIL;

    if (FAILED(m_pShader->BindRawValue("g_fDeltaTime", &m_fTimeDelta, sizeof(_float))))
        return E_FAIL;


    if (m_MeshDesc.bMask) {
        m_pMaskTexture->BindShaderResource(m_pShader, "g_MaskTexture", 0);
       
        if (FAILED(m_pShader->BindRawValue("g_vMaskSpeed", &m_fCurrentSpeed, sizeof(_float))))
            return E_FAIL;

        m_fCurrentSpeed += m_MeshDesc.fMaskSpeed;

        if (FAILED(m_pShader->BindRawValue("g_vMaskDirection", &m_MeshDesc.vMaskDir, sizeof(_float2))))
            return E_FAIL;
    
    }

    if (FAILED(m_pShader->BindRawValue("g_bNoiseTex", &m_MeshDesc.bNoise, sizeof(_bool))))
        return E_FAIL;

    if (m_MeshDesc.bNoise) {
        m_pNoiseTexture->BindShaderResource(m_pShader, "g_NoiseTexture", 0);

        if (FAILED(m_pShader->BindRawValue("g_vNoiseSpeed", &m_MeshDesc.fNoiseSpeed, sizeof(_float))))
            return E_FAIL;

        if (FAILED(m_pShader->BindRawValue("g_vNoiseDirection", &m_MeshDesc.vNoiseDir, sizeof(_float2))))
            return E_FAIL;
    }
   
    _uint iPassNum = 3; //Clamp

    if (m_MeshDesc.bUVLoop) {
        iPassNum = 2; //Wrap
    }


    _uint iNumMeshes = m_pModel->GetNumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++) {

        if (FAILED(m_pModel->BindMaterialShaderResource(m_pShader, (_uint)i, aiTextureType::aiTextureType_DIFFUSE, "g_DiffuseTexture")))
            return E_FAIL;

        if (FAILED(m_pShader->Begin(iPassNum)))
            return E_FAIL;

        if (FAILED(m_pModel->Render((_uint)i)))
            return E_FAIL;
    }

    return S_OK;
}

void CEffectMesh::EditEffect(const wstring& _strTextureKey, const wstring& _strModelKey, MESH_DESC* _MeshDesc)
{
    ResetEffect();

    m_MeshDesc = *_MeshDesc;
    m_vCurrentScale = XMLoadFloat3(&m_MeshDesc.vStartScale);

    ComputeInitSetting();

    m_pModel = CGameInstance::GetInstance()->GetModel(_strModelKey);

    _tchar szNoiseKey[MAX_PATH] = TEXT("");
    MultiByteToWideChar(CP_ACP, 0, m_MeshDesc.szNoiseTexKey, (_int)strlen(m_MeshDesc.szNoiseTexKey), szNoiseKey, MAX_PATH);

    _tchar szMaskKey[MAX_PATH] = TEXT("");
    MultiByteToWideChar(CP_ACP, 0, m_MeshDesc.szMaskTexKey, (_int)strlen(m_MeshDesc.szMaskTexKey), szMaskKey, MAX_PATH);

    m_pMaskTexture = CGameInstance::GetInstance()->GetTexture(szMaskKey);
    m_pNoiseTexture = CGameInstance::GetInstance()->GetTexture(szNoiseKey);

    m_pTransformCom->SetScaling(_MeshDesc->vStartScale.x, _MeshDesc->vStartScale.y, _MeshDesc->vStartScale.z);
    m_pTransformCom->SetState(CTransform::STATE_POSITION, { _MeshDesc->vCenter.x, _MeshDesc->vCenter.y, _MeshDesc->vCenter.z, 1.f });

    m_vColor = _MeshDesc->vColor;

}

void CEffectMesh::ComputeInitSetting()
{

    m_fMiddleTime = m_MeshDesc.fScaleChangeTime;

    _float fStartProcessTime = m_fMiddleTime - m_MeshDesc.vDuration.x;
    _float fEndProcessTime = m_MeshDesc.vDuration.y - m_fMiddleTime;

    //시작~ 중간 차이 계산 
    m_vStartScaleDiff = _float3({ (m_MeshDesc.vMiddleScale.x - m_MeshDesc.vStartScale.x) / fStartProcessTime,
    (m_MeshDesc.vMiddleScale.y - m_MeshDesc.vStartScale.y) / fStartProcessTime,
    (m_MeshDesc.vMiddleScale.z - m_MeshDesc.vStartScale.z) / fStartProcessTime });


    //중간 ~ 끝 차이 계산


    m_vEndScaleDiff = _float3({ (m_MeshDesc.vEndScale.x - m_MeshDesc.vMiddleScale.x) / fEndProcessTime,
    (m_MeshDesc.vEndScale.y - m_MeshDesc.vMiddleScale.y) / fEndProcessTime,
    (m_MeshDesc.vEndScale.z - m_MeshDesc.vMiddleScale.z) / fEndProcessTime });

}

void CEffectMesh::ScaleLerp()
{
    if (m_fMiddleTime >= m_fAccTime) {
        m_vCurrentScale += XMLoadFloat3(&m_vStartScaleDiff) * m_fTimeDelta;
    }
    else {
        m_vCurrentScale += XMLoadFloat3(&m_vEndScaleDiff) * m_fTimeDelta;
    }

    // SetScaling
    m_pTransformCom->SetScaling(m_vCurrentScale.m128_f32[0],
        m_vCurrentScale.m128_f32[1],
        m_vCurrentScale.m128_f32[2]);

}

void CEffectMesh::ParsingData(Json::Value& _root)
{

    Json::Value EffectInfo;


    char* strMultiByte = new char[256];
    WideCharToMultiByte(CP_ACP, 0, m_MeshKey.c_str(), -1, strMultiByte, 256, NULL, NULL);

    //Mesh
    EffectInfo["EffectType"] = 2;
    EffectInfo["MeshKey"] = strMultiByte;

    Json::Value Center;

    Center["x"] = m_MeshDesc.vCenter.x;
    Center["y"] = m_MeshDesc.vCenter.y;
    Center["z"] = m_MeshDesc.vCenter.z;

    EffectInfo["Center"] = Center;

    Json::Value Color;

    Color["x"] = m_MeshDesc.vColor.x;
    Color["y"] = m_MeshDesc.vColor.y;
    Color["z"] = m_MeshDesc.vColor.z;
    Color["w"] = m_MeshDesc.vColor.w;


    EffectInfo["Color"] = Color;

    Json::Value LerpColor;

    LerpColor["x"] = m_MeshDesc.vColor.x;
    LerpColor["y"] = m_MeshDesc.vColor.y;
    LerpColor["z"] = m_MeshDesc.vColor.z;
    LerpColor["w"] = m_MeshDesc.vColor.w;

    EffectInfo["LerpColor"] = LerpColor;

    Json::Value Mask;
    Mask["Using"] = m_MeshDesc.bMask;
    Mask["Speed"] = m_MeshDesc.fMaskSpeed;
    Mask["x"] = m_MeshDesc.vMaskDir.x;
    Mask["y"] = m_MeshDesc.vMaskDir.y;
    Mask["TexKey"] = m_MeshDesc.szMaskTexKey;


    EffectInfo["Mask"] = Mask;

    Json::Value Noise;
   
    Noise["Using"] = m_MeshDesc.bNoise;
    Noise["Speed"] = m_MeshDesc.fNoiseSpeed;
    Noise["x"] = m_MeshDesc.vNoiseDir.x;
    Noise["y"] = m_MeshDesc.vNoiseDir.y;
    Noise["TexKey"] = m_MeshDesc.szNoiseTexKey;

    EffectInfo["Noise"] = Noise;

    Json::Value StartScale;

    StartScale["x"] = m_MeshDesc.vStartScale.x;
    StartScale["y"] = m_MeshDesc.vStartScale.y;
    StartScale["z"] = m_MeshDesc.vStartScale.z;

    EffectInfo["StartScale"] = StartScale;

    Json::Value MiddleScale;

    MiddleScale["x"] = m_MeshDesc.vMiddleScale.x;
    MiddleScale["y"] = m_MeshDesc.vMiddleScale.y;
    MiddleScale["z"] = m_MeshDesc.vMiddleScale.z;

    EffectInfo["MiddleScale"] = MiddleScale;

    Json::Value EndScale;

    EndScale["x"] = m_MeshDesc.vEndScale.x;
    EndScale["y"] = m_MeshDesc.vEndScale.y;
    EndScale["z"] = m_MeshDesc.vEndScale.z;

    EffectInfo["EndScale"] = EndScale;

    Json::Value Duration;

    Duration["x"] = m_MeshDesc.vDuration.x;
    Duration["y"] = m_MeshDesc.vDuration.y;

    EffectInfo["Duration"] = Duration;

    Json::Value Loop;

    Loop["UVLoop"] = m_MeshDesc.bUVLoop;
    Loop["EffectLoop"] = m_MeshDesc.bLoop;

    EffectInfo["Loop"] = Loop;

    Json::Value ChangeTime;

    EffectInfo["ChangeTime"] = m_MeshDesc.fScaleChangeTime;


    _root[m_strEffectName] = EffectInfo;


}

void CEffectMesh::ResetEffect()
{
    m_fCurrentSpeed = 0.f;
    m_fAccTime = 0.f;
    m_vCurrentScale = XMLoadFloat3(&m_MeshDesc.vStartScale);
}

shared_ptr<CEffectMesh> CEffectMesh::Create(const wstring& _strTextureKey, const wstring& _strModelKey, MESH_DESC* _MeshDesc, char* _strName)
{

    shared_ptr<CEffectMesh> pInstance = make_shared<CEffectMesh>();

    if (FAILED(pInstance->Initialize(_strTextureKey, _strModelKey, _MeshDesc, _strName)))
        MSG_BOX("Failed to Create : cEffectMesh");

    return pInstance;
}
