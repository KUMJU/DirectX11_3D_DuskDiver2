#include "pch.h"
#include "Player.h"
#include "GameInstance.h"
#include "GameMgr.h"

#include "Model.h"
#include "Shader.h"

#include "CameraMgr.h"

CPlayer::CPlayer()
{
}

HRESULT CPlayer::Initialize()
{

    CTransform::TRANSFORM_DESC TransformDesc = {};
    TransformDesc.fSpeedPerSet = 10.f;
    TransformDesc.fRotationPerSet = 20.f;

    if (FAILED(__super::Initialize(&TransformDesc)))
        return E_FAIL;

    if (FAILED(AddComponent()))
        return E_FAIL;

    m_pShader = CGameInstance::GetInstance()->GetShader(TEXT("Shader_VtxAnim"));

    m_pModelCom = CGameInstance::GetInstance()->GetModel(TEXT("Hero1Walk"));
    m_Components.emplace(TEXT("Com_Model"), m_pModelCom);

    return S_OK;
}

void CPlayer::PriorityTick(_float _fTimeDelta)
{
}

void CPlayer::Tick(_float _fTimeDelta)
{
   // _float3 CamLook = CCameraMgr::GetInstance()->GetCamLook();
   // m_pTransformCom->LookAt(XMLoadFloat3(&CamLook));
    KeyInput(_fTimeDelta);
    m_pModelCom->PlayAnimation(_fTimeDelta);

}

void CPlayer::LateTick(_float _fTimeDelta)
{
    if (FAILED(CGameInstance::GetInstance()->AddRenderGroup(CRenderer::RENDER_NONBLEND, shared_from_this())))
        return;
}

HRESULT CPlayer::Render()
{
    if (FAILED(BindShaderResources()))
        return E_FAIL;

    _uint iNumMeshes = m_pModelCom->GetNumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++) {

        if (FAILED(m_pModelCom->BindMaterialShaderResource(m_pShader, (_uint)i, aiTextureType::aiTextureType_DIFFUSE, "g_DiffuseTexture")))
            return E_FAIL;

        if (FAILED(m_pModelCom->BindBoneMatrices(m_pShader, "g_BoneMatrices", i)))
            return E_FAIL;

        if (FAILED(m_pShader->Begin(0)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Render((_uint)i)))
            return E_FAIL;
    }


    return S_OK;
}

HRESULT CPlayer::AddComponent()
{
    return S_OK;
}

HRESULT CPlayer::BindShaderResources()
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

void CPlayer::KeyInput(_float _fTimeDelta)
{
    if (GetKeyState('W') & 0x8000)
    {
        m_pTransformCom->GoStraight(_fTimeDelta);
    }

    if (GetKeyState('S') & 0x8000)
    {
        m_pTransformCom->GoBackward(_fTimeDelta);
    }

    if (GetKeyState('A') & 0x8000)
    {
        m_pTransformCom->GoLeft(_fTimeDelta);
    }

    if (GetKeyState('D') & 0x8000)
    {
        m_pTransformCom->GoRight(_fTimeDelta);
    }


////////////////Camera/////////////////////////

    if (GetKeyState('1') & 0x8000)
    {
        CCameraMgr::GetInstance()->SwitchingCamera(CCameraMgr::ECAMERATYPE::THIRDPERSON);
    }


    if (GetKeyState('2') & 0x8000)
    {
        CCameraMgr::GetInstance()->SwitchingCamera(CCameraMgr::ECAMERATYPE::FREE);
    }

}

shared_ptr<CPlayer> CPlayer::Create()
{
    shared_ptr<CPlayer> pInstance = make_shared<CPlayer>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CPlayer");

    return pInstance;
}
