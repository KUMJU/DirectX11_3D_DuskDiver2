#include "pch.h"
#include "Dummy.h"

#include "GameInstance.h"
#include "Shader.h"
#include "Model.h"

CDummy::CDummy()
{
}


HRESULT CDummy::Initialize(const wstring& _strKey)
{
    if (FAILED(__super::Initialize(nullptr)))
        return E_FAIL;

    if (FAILED(AddComponent()))
        return E_FAIL;

    m_pShader = CGameInstance::GetInstance()->GetShader(TEXT("Shader_VtxMesh"));

    m_pModelCom = CGameInstance::GetInstance()->GetModel(_strKey);
    m_Components.emplace(TEXT("Com_Model"), m_pModelCom);

	return S_OK;
}

void CDummy::PriorityTick(_float _fTimeDelta)
{
}

void CDummy::Tick(_float _fTimeDelta)
{
}

void CDummy::LateTick(_float _fTimeDelta)
{
    if (FAILED(CGameInstance::GetInstance()->AddRenderGroup(CRenderer::RENDER_NONBLEND, shared_from_this())))
        return;

}

HRESULT CDummy::Render()
{
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

HRESULT CDummy::AddComponent()
{
    wrl::ComPtr<ID3D11Device> pDevice = CGameInstance::GetInstance()->GetDeviceInfo();
    wrl::ComPtr<ID3D11DeviceContext> pContext = CGameInstance::GetInstance()->GetDeviceContextInfo();

    return S_OK;
}                       

HRESULT CDummy::BindShaderResources()
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

    const LIGHT_DESC* pLightDesc = CGameInstance::GetInstance()->GetLightDesc(0);
    if (nullptr == pLightDesc)
        return E_FAIL;

    if (FAILED(m_pShader->BindRawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShader->BindRawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShader->BindRawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShader->BindRawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
        return E_FAIL;

    return S_OK;
}

shared_ptr<CDummy> CDummy::Create(const wstring& _strKey)
{
    shared_ptr<CDummy> pInstance = make_shared<CDummy>();

    if (FAILED(pInstance->Initialize(_strKey)))
        MSG_BOX("Failed to Create : CDummy");

	return pInstance;
}
