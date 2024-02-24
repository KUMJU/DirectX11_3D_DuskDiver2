#include "pch.h"
#include "UICommandGame.h"

#include "Shader.h"
#include "Texture.h"
#include "VIBuffer_UI.h"

#include "GameInstance.h"

CUICommandGame::CUICommandGame()
{
}

HRESULT CUICommandGame::Initialize()
{
    CUI::tagUIInfo UIInfo = {};

    UIInfo.fSizeX = 950.f;
    UIInfo.fSizeY = 200.f;
    UIInfo.fX = g_iWinSizeX * 0.5f;
    UIInfo.fY = g_iWinSizeY * 0.5f + 250.f;

    __super::Initialize(UIInfo, 2);
    CUI::AddBaseComponent();

    //배경이미지
    m_pTextureCom = CGameInstance::GetInstance()->GetTexture(TEXT("CommandUI"));
    m_Components.emplace(TEXT("Com_Texture"), m_pTextureCom);

    m_eUIGroup = CRenderer::UI_CONTENTS;

    return S_OK;
}

void CUICommandGame::PriorityTick(_float _fTimeDelta)
{
}

void CUICommandGame::Tick(_float _fTimeDelta)
{
}

void CUICommandGame::LateTick(_float _fTimeDelta)
{
    CUI::LateTick(_fTimeDelta);
}

HRESULT CUICommandGame::Render()
{
    _float4x4 ViewMatrix, ProjMatrix;
    XMStoreFloat4x4(&ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));


    if (FAILED(m_pTransformCom->BindShaderResource(m_pShader, "g_WorldMatrix")))
        return E_FAIL;

    m_pShader->BindMatrix("g_ViewMatrix", &ViewMatrix);
    m_pShader->BindMatrix("g_ProjMatrix", &ProjMatrix);

    if (FAILED(m_pTextureCom->BindShaderResource(m_pShader, "g_Texture", 0)))
        return E_FAIL;

    if (FAILED(m_pShader->Begin(8)))
        return E_FAIL;

    if (FAILED(m_VIRectCom->BindBuffers()))
        return E_FAIL;

    if (FAILED(m_VIRectCom->Render()))
        return E_FAIL;

    return S_OK;
}

shared_ptr<CUICommandGame> CUICommandGame::Create()
{
    shared_ptr<CUICommandGame> pInstance = make_shared<CUICommandGame>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CUICommandGame");

    return pInstance;
}
