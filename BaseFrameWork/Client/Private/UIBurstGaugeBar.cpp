#include "pch.h"
#include "UIBurstGaugeBar.h"

#include "GameInstance.h"
#include "Texture.h"
#include "Shader.h"
#include "VIBuffer_UI.h"

#include "UI_SequenceTex.h"

CUIBurstGaugeBar::CUIBurstGaugeBar()
{
}

HRESULT CUIBurstGaugeBar::Initialize()
{
    m_pTexTransforms.reserve(3);

    for (size_t i = 0; i < 3; ++i) {
        m_pTexTransforms.push_back(CTransform::Create(CGameInstance::GetInstance()->GetDeviceInfo(),
            CGameInstance::GetInstance()->GetDeviceContextInfo(), nullptr));
        m_pTexTransforms[i]->SetState(CTransform::STATE_POSITION, { -485.f + (i * 53.f), 238.f, 0.f , 1.f });
        m_pTexTransforms[i]->SetScaling(50.f, 15.f, 1.f);

    }


    CUI::tagUIInfo UIInfo = {};

    UIInfo.fSizeX = 0.f;
    UIInfo.fSizeY = 0.f;
    UIInfo.fX = 0.f;
    UIInfo.fY = 0.f;

    __super::Initialize(UIInfo, 1);
    CUI::AddBaseComponent();
    m_pTextureCom = CGameInstance::GetInstance()->GetTexture(TEXT("hud_img"));
    m_Components.emplace(TEXT("Com_Texture"), m_pTextureCom);


    /*Sequence Texture(UI Effect)*/

    UIInfo.fSizeX = 230.f;
    UIInfo.fSizeY = 75.f;
    // * 0.5f - 435.f
    UIInfo.fX = g_iWinSizeX * 0.5f - 433.f;
    UIInfo.fY = g_iWinSizeY * 0.5f - 240.f;

    CUI_SequenceTex::SequenceTexInfo SequeneceInfo = {};
    SequeneceInfo.bLoop = true;
    SequeneceInfo.fScrollTime = 0.1f;
    SequeneceInfo.iCol = 4;
    SequeneceInfo.iRow = 4;

    m_pUIEffect = CUI_SequenceTex::Create(&UIInfo, TEXT("fx_BP_battle"), 2, &SequeneceInfo);
    m_pUIEffect->SetEnable(false);

    if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_UI"), m_pUIEffect)))
        return E_FAIL;

    m_eUIGroup = CRenderer::UI_CONTENTS;


    return S_OK;
}

void CUIBurstGaugeBar::PriorityTick(_float _fTimeDelta)
{
}

void CUIBurstGaugeBar::Tick(_float _fTimeDelta)
{
}

void CUIBurstGaugeBar::LateTick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;

    CUI::LateTick(_fTimeDelta);
}

HRESULT CUIBurstGaugeBar::Render()
{
    for (size_t i = 0; i < 3; ++i) {

        if (FAILED(m_pTexTransforms[i]->BindShaderResource(m_pShader, "g_WorldMatrix")))
            return E_FAIL;

        _float4x4 ViewMatrix, ProjMatrix;
        XMStoreFloat4x4(&ViewMatrix, XMMatrixIdentity());
        XMStoreFloat4x4(&ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

        m_pShader->BindMatrix("g_ViewMatrix", &ViewMatrix);
        m_pShader->BindMatrix("g_ProjMatrix", &ProjMatrix);

        if (FAILED(m_pTextureCom->BindShaderResource(m_pShader, "g_Texture", 0)))
            return E_FAIL;

        if (FAILED(m_pShader->BindRawValue("g_fBurstGaugeRatio", &m_fGaugeRatio[i], sizeof(_float))))
            return E_FAIL;
  
        //g_fBurstGaugeRatio
        if (FAILED(m_pShader->Begin(6)))
            return E_FAIL;

        if (FAILED(m_VIRectCom->BindBuffers()))
            return E_FAIL;

        if (FAILED(m_VIRectCom->Render()))
            return E_FAIL;
    }

    return S_OK;
}

void CUIBurstGaugeBar::SetCurrentBurstGauge(_float _fBurstEnergy)
{
    m_fCurrentBurstGauge = _fBurstEnergy;
    ComputeRatio();
}

void CUIBurstGaugeBar::AddBurstGauge()
{
    if (m_fCurrentBurstGauge >= 90.f) {
        return;
    }

    m_fCurrentBurstGauge += 3.f;
    ComputeRatio();

}

void CUIBurstGaugeBar::SetBurstMode(_bool _bBurst)
{
    m_bBurstMode = _bBurst;

    if (m_bBurstMode) {
        m_pUIEffect->SetEnable(true);
    }
    else {
        m_pUIEffect->SetEnable(false);
    }

}

void CUIBurstGaugeBar::ComputeRatio()
{
    if (m_fCurrentBurstGauge > 90.f) {
        m_fCurrentBurstGauge = 90.f;
    }


    _int iFullGaugeNum = (_int)(m_fCurrentBurstGauge / 30);

    //매번 초기화 시켜주고 꽉찬 애들을 채워주는 방식 
    m_IsGaugeFull[0] = false;
    m_IsGaugeFull[1] = false;
    m_IsGaugeFull[2] = false;

    m_fGaugeRatio[0] = 0.f;
    m_fGaugeRatio[1] = 0.f;
    m_fGaugeRatio[2] = 0.f;


    for (_int i = 0; i < iFullGaugeNum; ++i) {
        m_IsGaugeFull[i] = true;
        m_fGaugeRatio[i] = 1.f;
    }

    if (iFullGaugeNum != 3) {
        m_fGaugeRatio[iFullGaugeNum] = ((m_fCurrentBurstGauge / 30.f) - iFullGaugeNum);
    }

}

shared_ptr<CUIBurstGaugeBar> CUIBurstGaugeBar::Create()
{
    shared_ptr<CUIBurstGaugeBar> pInstance = make_shared<CUIBurstGaugeBar>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CUIBurstGaugeBar");

    return pInstance;
}
