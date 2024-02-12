#include "pch.h"
#include "MapObject.h"
#include "GameInstance.h"
#include "Shader.h"
#include "Model.h"

CMapObject::CMapObject()
{
}

HRESULT CMapObject::Initialize(const wstring& _ModelKey, CTransform::TRANSFORM_DESC* _pDesc)
{
    __super::Initialize(_pDesc);

    m_pShader = CGameInstance::GetInstance()->GetShader(TEXT("Shader_VtxMesh"));
    m_pModelCom = CGameInstance::GetInstance()->GetModel(_ModelKey);

    m_Components.emplace(TEXT("Com_Model"), m_pModelCom);

    if (!m_pModelCom)
        MSG_BOX("Failed to Get Model! : MapObject");

    return S_OK;
}

void CMapObject::PriorityTick(_float _fTimeDelta)
{
}

void CMapObject::Tick(_float _fTimeDelta)
{
}

void CMapObject::LateTick(_float _fTimeDelta)
{
    if (FAILED(CGameInstance::GetInstance()->AddRenderGroup(CRenderer::RENDER_NONBLEND, shared_from_this())))
        return;
}

HRESULT CMapObject::Render()
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

HRESULT CMapObject::AddComponent()
{
    return S_OK;
}

HRESULT CMapObject::BindShaderResources()
{
    if (FAILED(m_pTransformCom->BindShaderResource(m_pShader, "g_WorldMatrix")))
        return E_FAIL;

    _float4x4 ViewMat = CGameInstance::GetInstance()->GetTransformFloat4x4(CPipeLine::D3DTS_VIEW);

    if (FAILED(m_pShader->BindMatrix("g_ViewMatrix", &ViewMat)))
        return E_FAIL;

    _float4x4 ProjMat = CGameInstance::GetInstance()->GetTransformFloat4x4(CPipeLine::D3DTS_PROJ);

    if (FAILED(m_pShader->BindMatrix("g_ProjMatrix", &ProjMat)))
        return E_FAIL;

    return S_OK;
}

shared_ptr<CMapObject> CMapObject::Create(const wstring& _ModelKey, CTransform::TRANSFORM_DESC* _pDesc)
{
    shared_ptr<CMapObject> pInstance = make_shared<CMapObject>();

    if (FAILED(pInstance->Initialize(_ModelKey, _pDesc)))
        MSG_BOX("Create Failed : CMapObject");


    return pInstance;
}
