#include "pch.h"
#include "UIScreenEffect.h"

#include "GameInstance.h"
#include "Shader.h"
#include "Texture.h"
#include "VIBuffer_UI.h"

CUIScreenEffect::CUIScreenEffect()
{
}

HRESULT CUIScreenEffect::Initialize()
{
    CUI::tagUIInfo UIInfo = {};

    UIInfo.fSizeX = (_float)g_iWinSizeX;
    UIInfo.fSizeY = (_float)g_iWinSizeY;
    UIInfo.fX = g_iWinSizeX * 0.5f;
    UIInfo.fY = g_iWinSizeY * 0.5f;

    __super::Initialize(UIInfo, 2);
    CUI::AddBaseComponent();


    m_pEffectTexture[0] = CGameInstance::GetInstance()->GetTexture(TEXT("T_Screen_Blood"));
    m_pEffectTexture[1] = CGameInstance::GetInstance()->GetTexture(TEXT("Center_Line"));
    m_pEffectTexture[2] = CGameInstance::GetInstance()->GetTexture(TEXT("T_Screen_Blood_side"));

    m_eUIGroup = CRenderer::UI_SCREEN;

    return S_OK;
}

void CUIScreenEffect::PriorityTick(_float _fTimeDelta)
{
}

void CUIScreenEffect::Tick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;


    if (CUIScreenEffect::EFFECTTYPE::TYPE_FADE == m_eEffectType) {
        m_fAccTime += _fTimeDelta;

        if (m_fAccTime >= m_fTotalTime * 0.5f) {
            m_fAlpha = (1.f - ((m_fAccTime - (m_fTotalTime * 0.5f)) / (m_fTotalTime * 0.5f)));

        }
        else {
            m_fAlpha = (m_fAccTime / (m_fTotalTime * 0.5f));
        }

        if (m_fTotalTime <= m_fAccTime) {
            m_IsEnabled = false;
            m_fAlpha = 0.f;
            m_fAccTime = 0.f;
        }

    }
    else {
        m_fAccTime += _fTimeDelta;
        m_fAlpha = (1.f - (m_fAccTime / m_fTotalTime));

        if (m_fTotalTime <= m_fAccTime) {
            m_IsEnabled = false;
            m_fAlpha = 0.f;
            m_fAccTime = 0.f;
        }

    }

}

void CUIScreenEffect::LateTick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;


    __super::LateTick(_fTimeDelta);
}

HRESULT CUIScreenEffect::Render()
{
    _float4x4 ViewMatrix, ProjMatrix;
    XMStoreFloat4x4(&ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));


    if (FAILED(m_pTransformCom->BindShaderResource(m_pShader, "g_WorldMatrix")))
        return E_FAIL;

    m_pShader->BindMatrix("g_ViewMatrix", &ViewMatrix);
    m_pShader->BindMatrix("g_ProjMatrix", &ProjMatrix);


    if (FAILED(m_pShader->BindRawValue("g_fScreenFXAlpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;

    _int iPassNum = 10;

    if (CUIScreenEffect::EFFECTTYPE::TYPE_FADE == m_eEffectType) {

        iPassNum = 11;

    }
    else {

        if (FAILED(m_pEffectTexture[m_eEffectType]->BindShaderResource(m_pShader, "g_Texture", 0)))
            return E_FAIL;

        iPassNum = 10;
    }

    if (FAILED(m_pShader->Begin(iPassNum)))
        return E_FAIL;

    if (FAILED(m_VIRectCom->BindBuffers()))
        return E_FAIL;

    if (FAILED(m_VIRectCom->Render()))
        return E_FAIL;

    return S_OK;
}

void CUIScreenEffect::SwitchScreenEffect(EFFECTTYPE _eEffectType)
{
    if (m_IsEnabled)
        return;

    if (CUIScreenEffect::EFFECTTYPE::TYPE_FADE == _eEffectType) {
        m_eUIGroup = CRenderer::UI_EFFECT;

    }
    else {

        m_eUIGroup = CRenderer::UI_SCREEN;
    }


    m_eEffectType = _eEffectType;
    m_fTotalTime = m_TotalTimes[_eEffectType];
    m_IsEnabled = true;

}

shared_ptr<CUIScreenEffect> CUIScreenEffect::Create()
{
    shared_ptr<CUIScreenEffect> pInstance = make_shared<CUIScreenEffect>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CUIScreenEffect");

    return pInstance;
}
