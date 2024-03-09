#include "pch.h"
#include "GroundCrack.h"

#include "GameInstance.h"

#include "Model.h"
#include "Shader.h"

CGroundCrack::CGroundCrack()
{
}

HRESULT CGroundCrack::Initialize()
{
    __super::Initialize(nullptr);

    m_eObjType = EObjType::OBJ_ENV;

    m_pModel = CGameInstance::GetInstance()->GetModel(TEXT("GroundCrackAnim"));
    m_pShader = CGameInstance::GetInstance()->GetShader(TEXT("Shader_VtxAnim"));

    return S_OK;
}

void CGroundCrack::PriorityTick(_float _fTimeDelta)
{
}

void CGroundCrack::Tick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;


    m_fAccTime += _fTimeDelta;


    if (m_fAccTime >= 7.f) {
        m_IsEnabled = false;
        return;
    }

    _float3 m_vCurrentAnimPos;
    m_pModel->PlayAnimation(_fTimeDelta, false, &m_vCurrentAnimPos);

}

void CGroundCrack::LateTick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;

    CGameInstance::GetInstance()->AddRenderGroup(CRenderer::RENDER_NONBLEND, shared_from_this());

}

HRESULT CGroundCrack::Render()
{
    if (FAILED(BindAnimShader()))
        return E_FAIL;

    _uint iNumMeshes = m_pModel->GetNumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++) {

        if (FAILED(m_pModel->BindMaterialShaderResource(m_pShader, (_uint)i, aiTextureType::aiTextureType_DIFFUSE, "g_DiffuseTexture")))
            return E_FAIL;

        if (FAILED(m_pModel->BindBoneMatrices(m_pShader, "g_BoneMatrices", (_uint)i)))
            return E_FAIL;

        if (FAILED(m_pShader->Begin(0)))
            return E_FAIL;

        if (FAILED(m_pModel->Render((_uint)i)))
            return E_FAIL;
    }

    return S_OK;
}

void CGroundCrack::ActiveSkill()
{
    m_fAccTime = 0.f;
    m_IsEnabled = true;
    m_pModel->ChangeAnimation(0);

}

HRESULT CGroundCrack::BindAnimShader()
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

shared_ptr<CGroundCrack> CGroundCrack::Create()
{

    shared_ptr<CGroundCrack> pInstance = make_shared<CGroundCrack>();
    
    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CGroundCrack");

    return pInstance;
}
