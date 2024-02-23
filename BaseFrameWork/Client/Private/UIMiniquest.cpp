#include "pch.h"
#include "UIMiniquest.h"
#include "GameInstance.h"
#include "Shader.h"
#include "Texture.h"
#include "VIBuffer_UI.h"

CUIMiniquest::CUIMiniquest()
{
}

HRESULT CUIMiniquest::Initialize()
{
    CUI::tagUIInfo UIInfo = {};

    UIInfo.fSizeX = 440.f;
    UIInfo.fSizeY = 220.f;
    UIInfo.fX = g_iWinSizeX * 0.5f;
    UIInfo.fY = g_iWinSizeY * 0.5f + 250.f;

    __super::Initialize(UIInfo, 2);
    CUI::AddBaseComponent();

    m_pTextureCom = CGameInstance::GetInstance()->GetTexture(TEXT("MiniQuestUI"));
    m_Components.emplace(TEXT("Com_Texture"), m_pTextureCom);


    /**Icon Setting**/

    //코인
    m_pIconTexture[0] = CGameInstance::GetInstance()->GetTexture(TEXT("hud_icon_coin"));
    m_Components.emplace(TEXT("Com_IconTex1"), m_pIconTexture[0]);

    //두더지
    m_pIconTexture[1] = CGameInstance::GetInstance()->GetTexture(TEXT("icon_store_tumaz"));
    m_Components.emplace(TEXT("Com_IconTex2"), m_pIconTexture[1]);

    m_pIconTransform = CTransform::Create(CGameInstance::GetInstance()->GetDeviceInfo(),
        CGameInstance::GetInstance()->GetDeviceContextInfo(),
        nullptr);

    m_pIconTransform->SetScaling( 80.f, 80.f, 1.f );
    m_pIconTransform->SetState(CTransform::STATE_POSITION, { UIInfo.fX - g_iWinSizeX * 0.5f - 115.f ,-UIInfo.fY + g_iWinSizeY * 0.5f , 0.f, 1.f });
    m_Components.emplace(TEXT("Com_IconTransform"), m_pIconTransform);

    m_eUIGroup = CRenderer::UI_BACKGROUND;

    m_IsEnabled = false;

    return S_OK;
}

void CUIMiniquest::PriorityTick(_float _fTimeDelta)
{
}

void CUIMiniquest::Tick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;

}

void CUIMiniquest::LateTick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;

    __super::LateTick(_fTimeDelta);

}

HRESULT CUIMiniquest::Render()
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

    if (FAILED(m_pShader->Begin(0)))
        return E_FAIL;

    if (FAILED(m_VIRectCom->BindBuffers()))
        return E_FAIL;

    if (FAILED(m_VIRectCom->Render()))
        return E_FAIL;

    //아이콘

    if (FAILED(m_pIconTransform->BindShaderResource(m_pShader, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pIconTexture[m_iCurrentIConNum]->BindShaderResource(m_pShader, "g_Texture", 0)))
        return E_FAIL;

    if (FAILED(m_pShader->Begin(0)))
        return E_FAIL;

    if (FAILED(m_VIRectCom->BindBuffers()))
        return E_FAIL;

    if (FAILED(m_VIRectCom->Render()))
        return E_FAIL;

    //폰트 
    wstring strGoal = TEXT(" / ") + to_wstring(m_iGoalNum);

    CGameInstance::GetInstance()->RenderFont(TEXT("Font_Number30"), to_wstring(m_iCurrentSuccessNum), {g_iWinSizeX * 0.5f - 20.f ,g_iWinSizeY * 0.5f + 235.f}, Colors::Aquamarine);
    CGameInstance::GetInstance()->RenderFont(TEXT("Font_Number15"), strGoal, {g_iWinSizeX * 0.5f + 25.f ,g_iWinSizeY * 0.5f + 250.f}, Colors::White);


    return S_OK;
}

void CUIMiniquest::StartCoinQuest()
{
    m_IsEnabled = true;
    m_iCurrentIConNum = 0;

    m_iCurrentSuccessNum = 0;
    m_iGoalNum = 10;
}

void CUIMiniquest::StartMoleGame()
{
    m_IsEnabled = true;
    m_iCurrentIConNum = 1;

    m_iCurrentSuccessNum = 0;
    m_iGoalNum = 10;

}

void CUIMiniquest::ResetUI()
{
    m_iGoalNum = 0.f;
    m_iCurrentIConNum = 0.f;
    m_iCurrentSuccessNum = 0.f;
}

shared_ptr<CUIMiniquest> CUIMiniquest::Create()
{
    shared_ptr<CUIMiniquest> pInstance = make_shared<CUIMiniquest>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CUIMinigame");

    return pInstance;
}
