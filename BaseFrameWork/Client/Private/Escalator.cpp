#include "pch.h"
#include "Escalator.h"
#include "Model.h"
#include "Shader.h"

#include "GameInstance.h"

CEscalator::CEscalator()
{
}

HRESULT CEscalator::Initialize(_uint _ModelType)
{

    wstring strModelKey;
    
    if (0 == _ModelType) {
        strModelKey = TEXT("Escalator_Base");
        m_pStepModel = CGameInstance::GetInstance()->GetModel(TEXT("Escalator_Step"));
    }
    else if(1 == _ModelType){
        strModelKey = TEXT("Escalator_Base");
        m_pStepModel = CGameInstance::GetInstance()->GetModel(TEXT("Escalator_Step"));

    }

    m_pAnimShader = CGameInstance::GetInstance()->GetShader(TEXT("Shader_VtxAnim"));
    __super::Initialize(strModelKey, nullptr);

    return S_OK;
}

void CEscalator::PriorityTick(_float _fTimeDelta)
{
}

void CEscalator::Tick(_float _fTimeDelta)
{
    _float3 m_vCurrentAnimPos;
    m_pStepModel->PlayAnimation(_fTimeDelta * 0.2f, true, &m_vCurrentAnimPos);
}

void CEscalator::LateTick(_float _fTimeDelta)
{
    __super::LateTick(_fTimeDelta);
}

HRESULT CEscalator::Render()
{
   __super::Render();

    BindAnimShader();

    //AnimMesh Render
    _uint iNumMeshes = m_pStepModel->GetNumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++) {

        if (FAILED(m_pStepModel->BindMaterialShaderResource(m_pAnimShader, (_uint)i, aiTextureType::aiTextureType_DIFFUSE, "g_DiffuseTexture")))
            return E_FAIL;

        if (FAILED(m_pStepModel->BindBoneMatrices(m_pAnimShader, "g_BoneMatrices", (_uint)i)))
            return E_FAIL;

        if (FAILED(m_pAnimShader->Begin(0)))
            return E_FAIL;

        if (FAILED(m_pStepModel->Render((_uint)i)))
            return E_FAIL;
    }

    return S_OK;
}

HRESULT CEscalator::BindAnimShader()
{
    if (FAILED(m_pTransformCom->BindShaderResource(m_pAnimShader, "g_WorldMatrix")))
        return E_FAIL;

    _float4x4 ViewMat = CGameInstance::GetInstance()->GetTransformFloat4x4(CPipeLine::D3DTS_VIEW);

    if (FAILED(m_pAnimShader->BindMatrix("g_ViewMatrix", &ViewMat)))
        return E_FAIL;

    _float4x4 ProjMat = CGameInstance::GetInstance()->GetTransformFloat4x4(CPipeLine::D3DTS_PROJ);

    if (FAILED(m_pAnimShader->BindMatrix("g_ProjMatrix", &ProjMat)))
        return E_FAIL;

    _float4 CamPos = CGameInstance::GetInstance()->GetCamPosition();

    if (FAILED(m_pAnimShader->BindRawValue("g_vCamPosition", &CamPos, sizeof(_float4))))
        return E_FAIL;

    const LIGHT_DESC* pLightDesc = CGameInstance::GetInstance()->GetLightDesc(0);
    if (nullptr == pLightDesc)
        return E_FAIL;

    return S_OK;
}

void CEscalator::StepOn()
{
}

shared_ptr<CEscalator> CEscalator::Create(_uint _ModelType)
{
    shared_ptr<CEscalator> pInstance = make_shared<CEscalator>();

    if (FAILED(pInstance->Initialize(_ModelType)))
        MSG_BOX("Failed to Create : CEscalator");

    return pInstance;
}
