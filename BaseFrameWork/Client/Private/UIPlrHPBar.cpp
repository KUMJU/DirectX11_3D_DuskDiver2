#include "pch.h"
#include "UIPlrHPBar.h"

#include "GameInstance.h"

#include "Shader.h"
#include "Texture.h"
#include "VIRect.h"

#include "VIBuffer_UI.h"


CUIPlrHPBar::CUIPlrHPBar()
{
}

HRESULT CUIPlrHPBar::Initialize()
{

    CUI::tagUIInfo UIInfo = {};

    UIInfo.fSizeX = 150.f;
    UIInfo.fSizeY = 12.f;
    UIInfo.fX = g_iWinSizeX * 0.5f - 435.f;
    UIInfo.fY = g_iWinSizeY * 0.5f - 270.f;

    m_fUIStartPoint = UIInfo.fX - (UIInfo.fSizeX * 0.5f);
    m_fUIEndPoint = UIInfo.fX + (UIInfo.fSizeX * 0.5f);

    __super::Initialize(UIInfo, 2);

    CUI::AddBaseComponent();

    m_pTextureCom = CGameInstance::GetInstance()->GetTexture(TEXT("hud_img"));
    m_Components.emplace(TEXT("Com_Texture"), m_pTextureCom);

    m_fCurrentHPRatio = 1.f;
    m_fPrevHPRatio = 1.f;

    return S_OK;
}

void CUIPlrHPBar::PriorityTick(_float _fTimeDelta)
{
}

void CUIPlrHPBar::Tick(_float _fTimeDelta)
{
    if (m_IsHPLerp)
        CalcHPDiff(_fTimeDelta);

}

void CUIPlrHPBar::LateTick(_float _fTimeDelta)
{
    CUI::LateTick(_fTimeDelta);
}

HRESULT CUIPlrHPBar::Render()
{
    if (FAILED(m_pTransformCom->BindShaderResource(m_pShader, "g_WorldMatrix")))
        return E_FAIL;

    _float4x4 ViewMatrix, ProjMatrix;
    XMStoreFloat4x4(&ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

    m_pShader->BindMatrix("g_ViewMatrix", &ViewMatrix);
    m_pShader->BindMatrix("g_ProjMatrix", &ProjMatrix);

    m_pShader->BindRawValue("g_HPRatio", &m_fCurrentHPRatio, sizeof(_float));
    m_pShader->BindRawValue("g_PrevHPRatio", &m_fPrevHPRatio, sizeof(_float));

    if (FAILED(m_pTextureCom->BindShaderResource(m_pShader, "g_Texture", 0)))
        return E_FAIL;

    if (FAILED(m_pShader->Begin(2)))
        return E_FAIL;

    if (FAILED(m_VIRectCom->BindBuffers()))
        return E_FAIL;

    if (FAILED(m_VIRectCom->Render()))
        return E_FAIL;

    return S_OK;
}

void CUIPlrHPBar::SetHP(_int _iHP)
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

void CUIPlrHPBar::CalcHPDiff(_float _fTimeDelta)
{
    if (m_fPrevHPRatio != m_fCurrentHPRatio) {
        m_fPrevHPRatio -= m_fCurrentDiff * _fTimeDelta;

        if (m_fPrevHPRatio <= m_fCurrentHPRatio) {
            m_fPrevHPRatio = m_fCurrentHPRatio;
            m_IsHPLerp = false;
        }
    }

}

shared_ptr<CUIPlrHPBar> CUIPlrHPBar::Create()
{
    shared_ptr<CUIPlrHPBar> pInstance = make_shared<CUIPlrHPBar>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CUIPlrHPBar");

    return pInstance;
}
