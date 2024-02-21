#include "pch.h"
#include "BossHPBar.h"

#include "GameInstance.h"

#include "Shader.h"
#include "Texture.h"
#include "VIBuffer_UI.h"


CBossHPBar::CBossHPBar()
{
}

HRESULT CBossHPBar::Initialize()
{

    CUI::tagUIInfo UIInfo = {};

    UIInfo.fSizeX = 450.f;
    UIInfo.fSizeY = 15.f;
    UIInfo.fX = g_iWinSizeX * 0.5f;
    UIInfo.fY = g_iWinSizeY * 0.5f - 265.f;

    __super::Initialize(UIInfo, 2);

    CUI::AddBaseComponent();

    m_pTextureCom = CGameInstance::GetInstance()->GetTexture(TEXT("hud_img"));
    m_Components.emplace(TEXT("Com_Texture"), m_pTextureCom);

    m_fCurrentHPRatio = 1.f;
    m_fPrevHPRatio = 1.f;


    m_pBackgroundTexture = CGameInstance::GetInstance()->GetTexture(TEXT("hud_boss_bg"));
    m_Components.emplace(TEXT("Com_BackgroundTexture"), m_pBackgroundTexture);

    m_pBackGroundTransform = CTransform::Create(CGameInstance::GetInstance()->GetDeviceInfo(),
        CGameInstance::GetInstance()->GetDeviceContextInfo(),nullptr);
    m_pBackGroundTransform->SetState(CTransform::STATE_POSITION, { m_pUIDefaultInfo.fX - g_iWinSizeX * 0.5f, -m_pUIDefaultInfo.fY + g_iWinSizeY * 0.5f+ 10.f, 0.f , 1.f });
    m_pBackGroundTransform->SetScaling(500.f, 60.f, 1.f);

    m_pBackGroundBuffer = CVIBuffer_UI::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo());

    m_eUIGroup = CRenderer::UI_CONTENTS;

    return S_OK;
}

void CBossHPBar::PriorityTick(_float _fTimeDelta)
{
}

void CBossHPBar::Tick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;

    if (m_IsHPLerp)
        CalcHPDiff(_fTimeDelta);
}

void CBossHPBar::LateTick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;

    CUI::LateTick(_fTimeDelta);
}

HRESULT CBossHPBar::Render()
{

    /*****BackGournd*******/
    if (FAILED(m_pBackGroundTransform->BindShaderResource(m_pShader, "g_WorldMatrix")))
        return E_FAIL;

    _float4x4 ViewMatrix, ProjMatrix;
    XMStoreFloat4x4(&ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

    m_pShader->BindMatrix("g_ViewMatrix", &ViewMatrix);
    m_pShader->BindMatrix("g_ProjMatrix", &ProjMatrix);


    if (FAILED(m_pBackgroundTexture->BindShaderResource(m_pShader, "g_Texture", 0)))
        return E_FAIL;

    if (FAILED(m_pShader->Begin(0)))
        return E_FAIL;

    if (FAILED(m_pBackGroundBuffer->BindBuffers()))
        return E_FAIL;

    if (FAILED(m_pBackGroundBuffer->Render()))
        return E_FAIL;

    /*****HPBar*******/

   if (FAILED(m_pTransformCom->BindShaderResource(m_pShader, "g_WorldMatrix")))
        return E_FAIL;

    XMStoreFloat4x4(&ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

    m_pShader->BindMatrix("g_ViewMatrix", &ViewMatrix);
    m_pShader->BindMatrix("g_ProjMatrix", &ProjMatrix);

    m_pShader->BindRawValue("g_HPRatio", &m_fCurrentHPRatio, sizeof(_float));
    m_pShader->BindRawValue("g_PrevHPRatio", &m_fPrevHPRatio, sizeof(_float));

    if (FAILED(m_pTextureCom->BindShaderResource(m_pShader, "g_Texture", 0)))
        return E_FAIL;

    if (FAILED(m_pShader->Begin(3)))
        return E_FAIL;

    if (FAILED(m_VIRectCom->BindBuffers()))
        return E_FAIL;

    if (FAILED(m_VIRectCom->Render()))
        return E_FAIL;


    CGameInstance::GetInstance()->RenderFont(TEXT("Font_Default_KR"), TEXT("È¥µ·ÀÇ ¾ß¼ö"), {g_iWinSizeX * 0.5f- 50.f ,g_iWinSizeY * 0.5f - 300.f}, Colors::White);


    return S_OK;
}

void CBossHPBar::SetHP(_int _iHP)
{
    if (_iHP <= 0) {
        m_fPrevHPRatio = 0.f;
        m_fCurrentHPRatio = 0.f;
        m_IsHPLerp = false;
    }
    else {
        if (!m_IsHPLerp) {
            m_fPrevHPRatio = (_float)m_iHP / (_float)m_iMaxHP;
            m_IsHPLerp = true;
        }
    }

    m_iHP = _iHP;
    m_fCurrentHPRatio = (_float)m_iHP / (_float)m_iMaxHP;
    m_fCurrentDiff = (m_fPrevHPRatio - m_fCurrentHPRatio) * 2.f;

}

void CBossHPBar::CalcHPDiff(_float _fTimeDelta)
{
    if (m_fPrevHPRatio != m_fCurrentHPRatio) {
        m_fPrevHPRatio -= m_fCurrentDiff * _fTimeDelta;

        if (m_fPrevHPRatio <= m_fCurrentHPRatio) {
            m_fPrevHPRatio = m_fCurrentHPRatio;
            m_IsHPLerp = false;
        }
    }

}

shared_ptr<CBossHPBar> CBossHPBar::Create()
{
    shared_ptr<CBossHPBar> pInstance = make_shared<CBossHPBar>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CBossHPBar");

    return pInstance;
}
