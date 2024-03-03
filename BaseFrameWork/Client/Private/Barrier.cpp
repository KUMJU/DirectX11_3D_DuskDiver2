#include "pch.h"
#include "Barrier.h"
#include "GameInstance.h"
#include "Model.h"
#include "Texture.h"

#include "Collider.h"
#include "CollisionMgr.h"
#include "VIBuffer_UI.h"

#include "Shader.h"

CBarrier::CBarrier()
{
}

HRESULT CBarrier::Initialize(_vector _vPos, _float3 _vRotation)
{

    m_eObjType = EObjType::OBJ_WALL;

    __super::Initialize(nullptr);

    m_pTransformCom->SetScaling(2.f, 2.f, 1.f);
    m_pTransformCom->Rotation({ 0.f, 1.f , 0.f }, XMConvertToRadians(_vRotation.y));
    m_pTransformCom->SetState(CTransform::STATE_POSITION, _vPos);


    m_pVIBuffer = CVIBuffer_UI::Create(CGameInstance::GetInstance()->GetDeviceInfo(),
        CGameInstance::GetInstance()->GetDeviceContextInfo());

    CCollider::COLLIDER_DESC CollDesc = {};

    CollDesc.vCenter = _float3(0.f, 0.f, 0.f);
    CollDesc.vExtents = _float3(2.3f, 1.2f, 2.3f);
    
    m_pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_AABB, CollDesc);
    m_pCollider->SetOwner(shared_from_this());

    m_pTexture = CGameInstance::GetInstance()->GetTexture(TEXT("T_Map12_WallVFX01"));
    m_pShader = CGameInstance::GetInstance()->GetShader(TEXT("Shader_VtxPointTex"));


    return S_OK;
}

void CBarrier::PriorityTick(_float _fTimeDelta)
{
}

void CBarrier::Tick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;



    ComputeColor(_fTimeDelta);

    m_pCollider->Tick(m_pTransformCom->GetWorldMatrix());
    CGameInstance::GetInstance()->AddCollider(CCollisionMgr::COL_BLOCK, m_pCollider);
}

void CBarrier::LateTick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;

    CGameInstance::GetInstance()->AddRenderGroup(CRenderer::RENDER_NONBLEND, shared_from_this());

#ifdef _DEBUG
    CGameInstance::GetInstance()->AddDebugComponent(m_pCollider);
#endif // _DEBUG
}

HRESULT CBarrier::Render()
{

    if (FAILED(m_pTransformCom->BindShaderResource(m_pShader, "g_WorldMatrix")))
        return E_FAIL;

    _float4x4 ViewMat = CGameInstance::GetInstance()->GetTransformFloat4x4(CPipeLine::D3DTS_VIEW);

    if (FAILED(m_pShader->BindMatrix("g_ViewMatrix", &ViewMat)))
        return E_FAIL;

    _float4x4 ProjMat = CGameInstance::GetInstance()->GetTransformFloat4x4(CPipeLine::D3DTS_PROJ);

    if (FAILED(m_pShader->BindMatrix("g_ProjMatrix", &ProjMat)))
        return E_FAIL;

    if (FAILED(m_pShader->BindRawValue("g_RGBColor", &m_vRGBColor, sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pTexture->BindShaderResource(m_pShader, "g_Texture", 0)))
        return E_FAIL;

    if (FAILED(m_pShader->Begin(2)))
        return E_FAIL;


    if (FAILED(m_pVIBuffer->BindBuffers()))
        return E_FAIL;

    if (FAILED(m_pVIBuffer->Render()))
        return E_FAIL;

    return S_OK;
}

void CBarrier::OnCollide(EObjType _eObjType, shared_ptr<CCollider> _pCollider)
{
}

void CBarrier::ComputeColor(_float _fDeltaTime)
{
    m_fColorChangeTime += _fDeltaTime * m_fSign;


    if (m_fColorChangeTime >= 1.f) {

        m_fColorChangeTime = 1.f;
        m_fSign = -1.f;

    }
    else if (m_fColorChangeTime <= 0.f) {

        m_fColorChangeTime = 0.f;
        m_fSign = 1.f;

    }


    _vector vLerpColor = XMVectorLerp(m_vOriginColor, m_vChangeColor, m_fColorChangeTime / 1.f);
    XMStoreFloat4(&m_vRGBColor, vLerpColor);

}

shared_ptr<CBarrier> CBarrier::Create(_vector _vPos , _float3 _vRotation)
{
    shared_ptr<CBarrier> pInstance = make_shared<CBarrier>();

    if (FAILED(pInstance->Initialize(_vPos, _vRotation)))
        MSG_BOX("Failed to Create : CBarrier");

    return pInstance;
}


