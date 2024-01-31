#include "pch.h"
#include "Mole.h"

#include "GameInstance.h"
#include "Collider.h"
#include "Shader.h"
#include "Model.h"

#include "MinigameMole.h"
#include "Player.h"

#include "GameMgr.h"


CMole::CMole()
{
}

HRESULT CMole::Initialize(_uint _iTypeNum)
{
    __super::Initialize(nullptr);

    m_MoleTypeNum = _iTypeNum;

    //정답 
    if (0 == m_MoleTypeNum) {
        m_pModelCom = CGameInstance::GetInstance()->GetModel(TEXT("Mole_BearAa"));
    }
    else if (1 == m_MoleTypeNum) {
        m_pModelCom = CGameInstance::GetInstance()->GetModel(TEXT("Mole_BearA"));
    }
    else if (2 == m_MoleTypeNum) {
        m_pModelCom = CGameInstance::GetInstance()->GetModel(TEXT("Mole_FakeBear"));
    }

    m_pTransformCom->SetScaling(10.f, 10.f, 10.f);
    m_pTransformCom->Rotation({ 0.f, 1.f , 0.f }, XMConvertToRadians(-90.f));
    m_IsEnabled = false;
    m_pShader = CGameInstance::GetInstance()->GetShader(TEXT("Shader_VtxMesh"));


    CCollider::COLLIDER_DESC desc = {};
    desc.fRadius = 0.2f;
    desc.vCenter = { 0.f, 0.1f, 0.f };

    m_pColider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_SPHERE, desc);
    m_Components.emplace(TEXT("Com_Collider"), m_pColider);
    m_pColider->SetOwner(shared_from_this());

    m_pPlayer = CGameMgr::GetInstance()->GetPlayer();

    return S_OK;
}

void CMole::PriorityTick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;
}

void CMole::Tick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;

    m_MovingTime += _fTimeDelta;

    _vector vPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
    vPos.m128_f32[1] += _fTimeDelta * m_fSpeed * m_fDir;

    m_pTransformCom->SetState(CTransform::STATE_POSITION, vPos);

        if (1.f == m_fDir && m_MovingTime >= 0.6f) {
            m_fDir = 0.f;
            m_MovingTime = 0.f;
        }
        else if (0.f == m_fDir && m_MovingTime >= 2.5f) {
            m_fDir = -1.f;
            m_MovingTime = 0.f;
        }
        else if (-1.f == m_fDir && m_MovingTime >= 0.6f) {
            m_IsEnabled = false;
            m_fDir = 1.f;
            m_MovingTime = 0.f;
        }
 

    m_pColider->Tick(m_pTransformCom->GetWorldMatrix());
    CGameInstance::GetInstance()->AddCollider(CCollisionMgr::COL_MONSTER, m_pColider);

}

void CMole::LateTick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;

    if (FAILED(CGameInstance::GetInstance()->AddRenderGroup(CRenderer::RENDER_NONBLEND, shared_from_this())))
        return;

}

HRESULT CMole::Render()
{
    if (!m_IsEnabled)
        return S_OK;

    m_pColider->Render();

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

void CMole::OnCollide(EObjType _eObjType, shared_ptr<CCollider> _pCollider)
{

    if (_eObjType == EObjType::OBJ_PROJ) {

        if (m_bCollLock)
            return;

        //정답
        if (0 == m_MoleTypeNum) {
            m_pMinigame.lock()->GetScore();
            m_bCollLock = true;
        }
        //오답
        else {
            m_pPlayer->OnHitMinigame();
            m_bCollLock = true;
        }
    }

}

void CMole::StateReset()
{
    m_fDir = 1.f;
    m_MovingTime = 0.f;
    m_bCollLock = false;

}

HRESULT CMole::BindShaderResources()
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

    return S_OK;
}

shared_ptr<CMole> CMole::Create(_uint _iTypeNum)
{
    shared_ptr<CMole> pInstance = make_shared<CMole>();

    if (FAILED(pInstance->Initialize(_iTypeNum)))
        MSG_BOX("Failed to Create : CMole");

    return pInstance;
}
