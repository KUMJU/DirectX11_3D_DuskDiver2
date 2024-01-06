#include "pch.h"
#include "BackGround.h"
#include "GameInstance.h"

#include "Shader.h"
#include "VIRect.h"
#include "Texture.h"

CBackGround::CBackGround()
    :CGameObject()
{
}

CBackGround::CBackGround(const CBackGround& _rhs)
    :CGameObject(_rhs)
{
}



void CBackGround::PreInitialize()
{
}

HRESULT CBackGround::Initialize()
{
    if (FAILED(__super::Initialize(nullptr)))
        return E_FAIL;

    if (FAILED(AddComponent()))
        return E_FAIL;

    m_pShaderCom = CGameInstance::GetInstance()->GetShader(TEXT("Shader_VtxPosTex"));
    m_pTextureCom = CGameInstance::GetInstance()->GetTexture(TEXT("title_BackGround"));

    m_fSizeX = g_iWinSizeX;
    m_fSizeY = g_iWinSizeY;
    m_fX = g_iWinSizeX * 0.5f;
    m_fY = g_iWinSizeY * 0.5f;

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

    m_pTransformCom->SetScaling(m_fSizeX, m_fSizeY, 1.f);
    m_pTransformCom->SetState(CTransform::STATE_POSITION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));


    return S_OK;
}

void CBackGround::PriorityTick(_float _fTimeDelta)
{
}

void CBackGround::Tick(_float _fTimeDelta)
{
}

void CBackGround::LateTick(_float _fTimeDelta)
{
    if (FAILED(CGameInstance::GetInstance()->AddRenderGroup(CRenderer::RENDER_PRIORITY, shared_from_this())))
        return;

}

HRESULT CBackGround::Render()
{

    if (FAILED(m_pTransformCom->BindShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    m_pShaderCom->BindMatrix("g_ViewMatrix", &m_ViewMatrix);
    m_pShaderCom->BindMatrix("g_ProjMatrix", &m_ProjMatrix);

    if (FAILED(m_pTextureCom->BindShaderResource(m_pShaderCom, "g_Texture", 0)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(0)))
        return E_FAIL;

    if (FAILED(m_pVIRect->BindBuffers()))
        return E_FAIL;

    if (FAILED(m_pVIRect->Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CBackGround::AddComponent()
{
 
    wrl::ComPtr<ID3D11Device> pDevice = CGameInstance::GetInstance()->GetDeviceInfo();
    wrl::ComPtr<ID3D11DeviceContext> pContext = CGameInstance::GetInstance()->GetDeviceContextInfo();

    m_pVIRect = CVIRect::Create(pDevice, pContext);
    m_Components.emplace(TEXT("Com_VIRect"), m_pVIRect);

    return S_OK;
}

shared_ptr<CBackGround> CBackGround::Create()
{
    shared_ptr<CBackGround> pInstance = make_shared<CBackGround>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CBackGround");

    return pInstance;
}
