#include "pch.h"
#include "UISkillBar.h"

#include "Texture.h"
#include "Transform.h"
#include "VIBuffer_UI.h"

#include "GameInstance.h"
#include "Shader.h"

#include "UI_SequenceTex.h"

CUISkillBar::CUISkillBar()
{
}

HRESULT CUISkillBar::Initialize()
{
    m_pTexTransforms.reserve(4);

    CUI::tagUIInfo UIInfo2 = {};

    UIInfo2.fSizeX = 25.f;
    UIInfo2.fSizeY = 23.f;

    CUI_SequenceTex::SequenceTexInfo TexInfo = {};
    TexInfo.bLoop = false;
    TexInfo.fScrollTime = 0.1f;
    TexInfo.iCol = 4;
    TexInfo.iRow = 4;

    for (size_t i = 0; i < 4; ++i) {
        m_pTexTransforms.push_back(CTransform::Create(CGameInstance::GetInstance()->GetDeviceInfo(),
            CGameInstance::GetInstance()->GetDeviceContextInfo(), nullptr));
        m_pTexTransforms[i]->SetState(CTransform::STATE_POSITION, { - 497.f + (i * 27.f), 255.f, 0.f , 1.f});
        m_pTexTransforms[i]->SetScaling(25.f, 15.f, 1.f);

        UIInfo2.fX =  (g_iWinSizeX * 0.5f- 497.f) + (i * 27.f);
        UIInfo2.fY =  g_iWinSizeY * 0.5f- 255.f;

        m_pUIEffect[i] = CUI_SequenceTex::Create(&UIInfo2, TEXT("fx_sp_full"), 2, &TexInfo);
        CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_Effect"), m_pUIEffect[i]);
    }


    CUI::tagUIInfo UIInfo = {};

    UIInfo.fSizeX = 0.f;
    UIInfo.fSizeY = 0.f;
    UIInfo.fX = 0.f;
    UIInfo.fY = 0.f;

    __super::Initialize(UIInfo, 2);
    CUI::AddBaseComponent();
    m_pTextureCom = CGameInstance::GetInstance()->GetTexture(TEXT("hud_img"));
    m_Components.emplace(TEXT("Com_Texture"), m_pTextureCom);

    m_eUIGroup = CRenderer::UI_CONTENTS;


    


    return S_OK;
}

void CUISkillBar::PriorityTick(_float _fTimeDelta)
{
}

void CUISkillBar::Tick(_float _fTimeDelta)
{
}

void CUISkillBar::LateTick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;

    CUI::LateTick(_fTimeDelta);

}

HRESULT CUISkillBar::Render()
{

    for (size_t i = 0; i < 4; ++i) {

       if (FAILED(m_pTexTransforms[i]->BindShaderResource(m_pShader, "g_WorldMatrix")))
            return E_FAIL;

        _float4x4 ViewMatrix, ProjMatrix;
        XMStoreFloat4x4(&ViewMatrix, XMMatrixIdentity());
        XMStoreFloat4x4(&ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

        m_pShader->BindMatrix("g_ViewMatrix", &ViewMatrix);
        m_pShader->BindMatrix("g_ProjMatrix", &ProjMatrix);

        if (FAILED(m_pTextureCom->BindShaderResource(m_pShader, "g_Texture", 0)))
            return E_FAIL;

        if (FAILED(m_pShader->BindRawValue("g_fSkillGaugeRatio", &m_fGaugeRatio[i], sizeof(_float))))
            return E_FAIL;

        if (FAILED(m_pShader->Begin(5)))
            return E_FAIL;

        if (FAILED(m_VIRectCom->BindBuffers()))
            return E_FAIL;

        if (FAILED(m_VIRectCom->Render()))
            return E_FAIL;
    }

    return S_OK;
}

void CUISkillBar::SetSkillGauge(_int _iSkillGauge, _bool _isIncrease)
{
    m_IsGaugeFull[0] = false;
    m_IsGaugeFull[1] = false;
    m_IsGaugeFull[2] = false;
    m_IsGaugeFull[3] = false;

    m_fGaugeRatio[0] = 0.f;
    m_fGaugeRatio[1] = 0.f;
    m_fGaugeRatio[2] = 0.f;
    m_fGaugeRatio[3] = 0.f;

    _int iFullSkillNum = -1;

    for (_int i = 0; i < _iSkillGauge; ++i) {
        m_IsGaugeFull[i] = true;
        m_fGaugeRatio[i] = 1.f;
        iFullSkillNum = i;
    }

    if (iFullSkillNum != -1 && iFullSkillNum != m_iPrevFullSkillNum && m_iPrevFullSkillNum < iFullSkillNum) {
        m_pUIEffect[iFullSkillNum]->SetEnable(true);
        m_iPrevFullSkillNum = iFullSkillNum;
    }
    else if (iFullSkillNum == -1) {
        m_iPrevFullSkillNum = -1;
    }
    else if (m_iPrevFullSkillNum != iFullSkillNum) {
        m_iPrevFullSkillNum = iFullSkillNum;
    }


    if (_isIncrease) {
        m_fCurrentGaugingRatio = 0.f;
    }
    else {
        m_fGaugeRatio[_iSkillGauge] = m_fCurrentGaugingRatio;
    }

    m_iFirstEmptyIdx = _iSkillGauge;

}

void CUISkillBar::ComputeRatio(_float _fTimeRate)
{


}

void CUISkillBar::SetCurrentGauging(_float _fRatio)
{
    m_fCurrentGaugingRatio = _fRatio;
    m_fGaugeRatio[m_iFirstEmptyIdx] = _fRatio;

}

shared_ptr<CUISkillBar> CUISkillBar::Create()
{
    shared_ptr<CUISkillBar> pInstance = make_shared<CUISkillBar>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CUISkillBar");

    return pInstance;
}
