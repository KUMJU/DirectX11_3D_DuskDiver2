#include "pch.h"
#include "Dummy.h"

#include "GameInstance.h"
#include "Shader.h"
#include "Model.h"
#include "ModelMaterial.h"
#include "Mesh.h"

#include "EffectMgr.h"
#include "EffectPreset.h"

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

    //m_pEffectPreset = CEffectMgr::GetInstance()->FindEffect(TEXT("ParticleMap"));
    //m_pEffectPreset->SetParentTransform(m_pTransformCom);
    //m_pEffectPreset->PlayEffect();
	return S_OK;
}

void CDummy::PriorityTick(_float _fTimeDelta)
{
}

void CDummy::Tick(_float _fTimeDelta)
{
   // m_pEffectPreset->Tick(_fTimeDelta);

}

void CDummy::LateTick(_float _fTimeDelta)
{
    if (FAILED(CGameInstance::GetInstance()->AddRenderGroup(CRenderer::RENDER_NONBLEND, shared_from_this())))
        return;

   // m_pEffectPreset->LateTick(_fTimeDelta);

}

HRESULT CDummy::Render()
{
    if (FAILED(BindShaderResources()))
        return E_FAIL;

    _uint iNumMeshes = m_pModelCom->GetNumMeshes();

    vector<shared_ptr<CMesh>> Meshes = m_pModelCom->GetMeshes();
    vector<shared_ptr<CMaterial>>* Materials = m_pModelCom->GetMaterial();

    for (size_t i = 0; i < iNumMeshes; i++) {

        if (FAILED(m_pModelCom->BindMaterialShaderResource(m_pShader, (_uint)i, aiTextureType::aiTextureType_DIFFUSE, "g_DiffuseTexture")))
            return E_FAIL;

        _uint iMaterialIndex = Meshes[i]->GetMaterialIndex();
        shared_ptr<CTexture> pTexture = (*Materials)[iMaterialIndex]->GetTextures()[aiTextureType::aiTextureType_NORMALS];
        _uint iPassNum = 0;

        shared_ptr<CTexture> pTexture2 = (*Materials)[iMaterialIndex]->GetTextures()[aiTextureType::aiTextureType_AMBIENT];

        if (pTexture2) {
            int a = 3;
        }

        if (pTexture) {

            if (FAILED(m_pModelCom->BindMaterialShaderResource(m_pShader, (_uint)i, aiTextureType::aiTextureType_NORMALS, "g_NormalTexture")))
                return E_FAIL;

            iPassNum = 6;
           
        }

        if (FAILED(m_pShader->Begin(iPassNum)))
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

    return S_OK;
}

shared_ptr<CDummy> CDummy::Create(const wstring& _strKey)
{
    shared_ptr<CDummy> pInstance = make_shared<CDummy>();

    if (FAILED(pInstance->Initialize(_strKey)))
        MSG_BOX("Failed to Create : CDummy");

	return pInstance;
}
