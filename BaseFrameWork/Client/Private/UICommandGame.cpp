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

    //밑에 키 안내 이미지 
    m_pTextureCom = CGameInstance::GetInstance()->GetTexture(TEXT("CommandUI"));
    m_Components.emplace(TEXT("Com_Texture"), m_pTextureCom);

    m_eUIGroup = CRenderer::UI_CONTENTS;

    // 게이지바
    m_pGaugeTex = CGameInstance::GetInstance()->GetTexture(TEXT("GUI_EnduranceGauage01"));
    m_Components.emplace(TEXT("Com_GaugeTex"), m_pGaugeTex);

    //게이지바 배경 
    m_pGaugeBackGround = CGameInstance::GetInstance()->GetTexture(TEXT("hud_boss_bg"));
    m_Components.emplace(TEXT("Com_GaugeBackgroundTex"), m_pGaugeBackGround);

    m_pGaugeBackGroundTransform = CTransform::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), nullptr);
    m_pGaugeBackGroundTransform->SetState(CTransform::STATE_POSITION, { UIInfo.fX - g_iWinSizeX * 0.5f ,-UIInfo.fY + g_iWinSizeY * 0.5f + 500.f  , 0.f, 1.f });
    m_pGaugeBackGroundTransform->SetScaling(630.f, 40.f, 1.f);

    m_pGaugeTransform = CTransform::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), nullptr);
    m_pGaugeTransform->SetState(CTransform::STATE_POSITION, { UIInfo.fX - g_iWinSizeX * 0.5f ,-UIInfo.fY + g_iWinSizeY * 0.5f + 500.f  , 0.f, 1.f });
    m_pGaugeTransform->SetScaling(560.f, 30.f, 1.f);

    m_pAlarmTexture[0] = CGameInstance::GetInstance()->GetTexture(TEXT("HUD_popup_Item06"));
    m_pAlarmTexture[1] = CGameInstance::GetInstance()->GetTexture(TEXT("HUD_popup_Item03"));

    m_pAlarmTransform = CTransform::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), nullptr);
    m_pAlarmTransform->SetState(CTransform::STATE_POSITION, { UIInfo.fX - g_iWinSizeX * 0.5f ,-UIInfo.fY + g_iWinSizeY * 0.5f+ 300.f  , 0.f, 1.f });
    m_pAlarmTransform->SetScaling(270.f, 100.f, 1.f);

    return S_OK;
}

void CUICommandGame::PriorityTick(_float _fTimeDelta)
{
}

void CUICommandGame::Tick(_float _fTimeDelta)
{

    if (m_bStart || m_bEnd) {

        m_fAlarmAccTime += _fTimeDelta;

        if (3.f <= m_fAlarmAccTime) {
            m_fAlarmAccTime = 0.f;
            m_bStart = false;
            m_bEnd = false;
        }
    }

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

    /*게이지 백그라운드*/
    if (FAILED(m_pGaugeBackGroundTransform->BindShaderResource(m_pShader, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pGaugeBackGround->BindShaderResource(m_pShader, "g_Texture", 0)))
        return E_FAIL;

    if (FAILED(m_pShader->Begin(0)))
        return E_FAIL;

    if (FAILED(m_VIRectCom->BindBuffers()))
        return E_FAIL;

    if (FAILED(m_VIRectCom->Render()))
        return E_FAIL;


    /*게이지바*/

    if (FAILED(m_pGaugeTransform->BindShaderResource(m_pShader, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShader->BindRawValue("g_fCommandSuccessRate", &m_fSuccessRate, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pGaugeTex->BindShaderResource(m_pShader, "g_Texture", 0)))
        return E_FAIL;

    if (FAILED(m_pShader->Begin(9)))
        return E_FAIL;

    if (FAILED(m_VIRectCom->BindBuffers()))
        return E_FAIL;

    if (FAILED(m_VIRectCom->Render()))
        return E_FAIL;


    //시작/ 끝 알림

    if (m_bStart || m_bEnd) {

        if (FAILED(m_pAlarmTransform->BindShaderResource(m_pShader, "g_WorldMatrix")))
            return E_FAIL;

        if (FAILED(m_pAlarmTexture[m_iAlarmTexNum]->BindShaderResource(m_pShader, "g_Texture", 0)))
            return E_FAIL;

        if (FAILED(m_pShader->Begin(8)))
            return E_FAIL;

        if (FAILED(m_VIRectCom->BindBuffers()))
            return E_FAIL;

        if (FAILED(m_VIRectCom->Render()))
            return E_FAIL;

    }


    return S_OK;
}

void CUICommandGame::ComputeRate()
{
    m_fSuccessRate = (_float)m_iCompleteNum / (_float)m_iClearMaxNum;
}

shared_ptr<CUICommandGame> CUICommandGame::Create()
{
    shared_ptr<CUICommandGame> pInstance = make_shared<CUICommandGame>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CUICommandGame");

    return pInstance;
}
