#include "pch.h"
#include "UIMapTitle.h"

#include "GameInstance.h"
#include "Shader.h"
#include "Texture.h"
#include "VIBuffer_UI.h"

#include "UI_SequenceTex.h"

CUIMapTitle::CUIMapTitle()
{
}

HRESULT CUIMapTitle::Initialize()
{
    CUI::tagUIInfo UIInfo = {};

    UIInfo.fSizeX = (_float)g_iWinSizeX;
    UIInfo.fSizeY = 400.f;
    UIInfo.fX = g_iWinSizeX * 0.5f;
    UIInfo.fY = g_iWinSizeY * 0.5f;

    __super::Initialize(UIInfo, 2);

    CUI::AddBaseComponent();

    m_pTextureCom = CGameInstance::GetInstance()->GetTexture(TEXT("QuestBar"));
    m_Components.emplace(TEXT("Com_Texture"), m_pTextureCom);
    m_IsEnabled = false;
    m_eUIGroup = CRenderer::UI_PRIORITY;


    /*½ÃÄö½º ÀÌÆåÆ®*/

    UIInfo.fSizeX = 800.f;
    UIInfo.fSizeY = 100.f;
    UIInfo.fX = g_iWinSizeX * 0.5f;
    UIInfo.fY = g_iWinSizeY * 0.5f;

    CUI_SequenceTex::SequenceTexInfo SequeneceInfo = {};
    SequeneceInfo.bLoop = false;
    SequeneceInfo.fScrollTime = 0.1f;
    SequeneceInfo.iCol = 5;
    SequeneceInfo.iRow = 1;

    m_pEffectTex = CUI_SequenceTex::Create(&UIInfo, TEXT("T_Common_Lightning_1X4"), 2, &SequeneceInfo);
    if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_UI"), m_pEffectTex)))
        return E_FAIL;
    m_pEffectTex->SetUIRenderGroup(CRenderer::UI_PRIORITY);

    return S_OK;
}

void CUIMapTitle::PriorityTick(_float _fTimeDelta)
{
}

void CUIMapTitle::Tick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;

    m_fAccTime += _fTimeDelta;

    if (m_fAccTime >= 5.f) {
        m_IsEnabled = false;
        m_fAccTime = 0.f;
    }

}

void CUIMapTitle::LateTick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;

    __super::LateTick(_fTimeDelta);
}

HRESULT CUIMapTitle::Render()
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


    CGameInstance::GetInstance()->RenderFont(TEXT("Font_Title_KR"), m_strQuiestInfo, {g_iWinSizeX * 0.5f - 300.f,g_iWinSizeY * 0.5f}, Colors::Black);


    return S_OK;
}

void CUIMapTitle::StartQuest(const wstring& _strQuestInfo)
{
    m_IsEnabled = true;
    m_pEffectTex->SetEnable(true);
    m_strQuiestInfo = _strQuestInfo;
    m_fAccTime = 0.f;

    CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_UI_FX);
    CGameInstance::GetInstance()->PlayAudio(TEXT("UI_TitleCard.wav"), CSoundMgr::CHANNELID::CH_UI_FX, 1.f);

}

shared_ptr<CUIMapTitle> CUIMapTitle::Create()
{
    shared_ptr<CUIMapTitle> pInstance = make_shared<CUIMapTitle>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CUIMapTitle");

    return pInstance;
}
