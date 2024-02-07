#include "pch.h"
#include "UIQuest.h"

#include "GameInstance.h"
#include "Shader.h"
#include "Texture.h"
#include "VIBuffer_UI.h"


CUIQuest::CUIQuest()
{
}

HRESULT CUIQuest::Initialize()
{
    CUI::tagUIInfo UIInfo = {};

    UIInfo.fSizeX = 300.f;
    UIInfo.fSizeY = 100.f;
    UIInfo.fX = g_iWinSizeX * 0.5f + 513.f;
    UIInfo.fY = g_iWinSizeY * 0.5f - 278.f;

    __super::Initialize(UIInfo, 2);

    CUI::AddBaseComponent();

	m_pTextureCom = CGameInstance::GetInstance()->GetTexture(TEXT("hud_task"));
    m_Components.emplace(TEXT("Com_Texture"), m_pTextureCom);

    strCurrentQuest = TEXT("이상현상을 제거하라");

    m_pTransformCom->Rotation({ 0.f, 0.f, 1.f ,0.f }, XMConvertToRadians(3.5f));
    m_IsEnabled = true;

	return S_OK;
}

void CUIQuest::PriorityTick(_float _fTimeDelta)
{
}

void CUIQuest::Tick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;


}

void CUIQuest::LateTick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;

    __super::LateTick(_fTimeDelta);
}

HRESULT CUIQuest::Render()
{
    if (!m_IsEnabled)
        return S_OK;

    if (FAILED(m_pTransformCom->BindShaderResource(m_pShader, "g_WorldMatrix")))
        return E_FAIL;

    _float4x4 ViewMatrix, ProjMatrix;

    XMStoreFloat4x4(&ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

    m_pShader->BindMatrix("g_ViewMatrix", &ViewMatrix);
    m_pShader->BindMatrix("g_ProjMatrix", &ProjMatrix);

    if (FAILED(m_pTextureCom->BindShaderResource(m_pShader, "g_Texture", 0)))
        return E_FAIL;

    if (FAILED(m_pShader->Begin(0)))
        return E_FAIL;

    if (FAILED(m_VIRectCom->BindBuffers()))
        return E_FAIL;

    if (FAILED(m_VIRectCom->Render()))
        return E_FAIL;


    CGameInstance::GetInstance()->RenderFont(TEXT("Font_Default_KR"), strCurrentQuest, { g_iWinSizeX * 0.5f + 430.f,g_iWinSizeY * 0.5f - 274.f }, Colors::White, XMConvertToRadians(-3.5f));


    return S_OK;
}

shared_ptr<CUIQuest> CUIQuest::Create()
{
    shared_ptr<CUIQuest> pInstance = make_shared<CUIQuest>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CUIQuest");

    return pInstance;
}
