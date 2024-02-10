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
    m_eEffectType = EFFECT_TYPE::TYPE_MESH;
    __super::Initialize(nullptr);

    m_pShader = CGameInstance::GetInstance()->GetShader(TEXT("Shader_VtxMesh"));

    m_pMaskTexture = CGameInstance::GetInstance()->GetTexture(_strTextureKey);
    m_Components.emplace(TEXT("Com_Texture"), m_pTexture);

    m_pModel = CGameInstance::GetInstance()->GetModel(_strModelKey);
    m_Components.emplace(TEXT("Com_Model"), m_pModel);


    m_pTransformCom->SetScaling(_MeshDesc->vScale.x, _MeshDesc->vScale.y, _MeshDesc->vScale.z);
    m_pTransformCom->SetState(CTransform::STATE_POSITION,  {_MeshDesc->vCenter.x, _MeshDesc->vCenter.y, _MeshDesc->vCenter.z, 1.f });

    m_strEffectName = _strName;

    return S_OK;
}

void CEffectMesh::PriorityTick(_float _fTimeDelta)
{
}

void CEffectMesh::Tick(_float _fTimeDelta)
{
}

void CEffectMesh::LateTick(_float _fTimeDelta)
{
    if (FAILED(CGameInstance::GetInstance()->AddRenderGroup(CRenderer::RENDER_NONBLEND, shared_from_this())))
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

    _float4 CamPos = CGameInstance::GetInstance()->GetCamPosition();

    if (FAILED(m_pShader->BindRawValue("g_vCamPosition", &CamPos, sizeof(_float4))))
        return E_FAIL;


    m_pMaskTexture->BindShaderResource(m_pShader, "g_MaskTexture", 0);


    _uint iNumMeshes = m_pModel->GetNumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++) {

        if (FAILED(m_pModel->BindMaterialShaderResource(m_pShader, (_uint)i, aiTextureType::aiTextureType_DIFFUSE, "g_DiffuseTexture")))
            return E_FAIL;

        if (FAILED(m_pShader->Begin(2)))
            return E_FAIL;

        if (FAILED(m_pModel->Render((_uint)i)))
            return E_FAIL;
    }

    return S_OK;
}

void CEffectMesh::ParsingData(Json::Value& _root)
{


}

shared_ptr<CEffectMesh> CEffectMesh::Create(const wstring& _strTextureKey, const wstring& _strModelKey, MESH_DESC* _MeshDesc, char* _strName)
{

    shared_ptr<CEffectMesh> pInstance = make_shared<CEffectMesh>();

    if (FAILED(pInstance->Initialize(_strTextureKey, _strModelKey, _MeshDesc, _strName)))
        MSG_BOX("Failed to Create : cEffectMesh");

    return pInstance;
}
