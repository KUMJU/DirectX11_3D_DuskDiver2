#include "pch.h"
#include "UIBurstSkillGauge.h"

#include "GameInstance.h"

#include "Shader.h"
#include "Texture.h"
#include "VIBuffer_UI.h"

#include "UI_SequenceTex.h"

CUIBurstSkillGauge::CUIBurstSkillGauge()
{
}

HRESULT CUIBurstSkillGauge::Initialize()
{

    CUI::tagUIInfo UIInfo = {};

    UIInfo.fSizeX = 140.f;
    UIInfo.fSizeY = 140.f;
    UIInfo.fX = g_iWinSizeX * 0.5f - 480.f;
    UIInfo.fY = g_iWinSizeY * 0.5f;

    __super::Initialize(UIInfo, 2);
    CUI::AddBaseComponent();

//메인 게이지 이미지
    m_pTextureCom = CGameInstance::GetInstance()->GetTexture(TEXT("hud_burst_bar"));
    m_Components.emplace(TEXT("Com_Texture"), m_pTextureCom);

//배경 이미지 
    m_pBackgroundTexture = CGameInstance::GetInstance()->GetTexture(TEXT("hud_burst_bg"));
    m_Components.emplace(TEXT("Com_BackGroundTexture"), m_pTextureCom);

    m_pBackGroundTransform = CTransform::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), nullptr);

    m_pBackGroundTransform->SetState(CTransform::STATE_POSITION, { UIInfo.fX - g_iWinSizeX * 0.5f ,-UIInfo.fY + g_iWinSizeY * 0.5f + 10.f , 0.f, 1.f });
    m_pBackGroundTransform->SetScaling(270.f, 270.f, 1.f);

    m_eUIGroup = CRenderer::UI_CONTENTS;

    UIInfo.fSizeX = 300.f;
    UIInfo.fSizeY = 300.f;

    CUI_SequenceTex::SequenceTexInfo SequeneceInfo = {};
    SequeneceInfo.bLoop = false;
    SequeneceInfo.fScrollTime = 0.1f;
    SequeneceInfo.iCol = 4;
    SequeneceInfo.iRow = 4;

    m_pUIEffect = CUI_SequenceTex::Create(&UIInfo, TEXT("fx_burst_break"), 2, &SequeneceInfo);
   // m_pUIEffect->SetEnable(true);
    if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_UI"), m_pUIEffect)))
        return E_FAIL;

    return S_OK;
}

void CUIBurstSkillGauge::PriorityTick(_float _fTimeDelta)
{
}

void CUIBurstSkillGauge::Tick(_float _fTimeDelta)
{
}

void CUIBurstSkillGauge::LateTick(_float _fTimeDelta)
{
    CUI::LateTick(_fTimeDelta);
}

HRESULT CUIBurstSkillGauge::Render()
{
    //배경
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

    if (FAILED(m_VIRectCom->BindBuffers()))
        return E_FAIL;

    if (FAILED(m_VIRectCom->Render()))
        return E_FAIL;


    //게이지바 
    if (FAILED(m_pTransformCom->BindShaderResource(m_pShader, "g_WorldMatrix")))
        return E_FAIL;

    m_pShader->BindMatrix("g_ViewMatrix", &ViewMatrix);
    m_pShader->BindMatrix("g_ProjMatrix", &ProjMatrix);

    //240 대신에 현재 시간에 맞는 비율 각도 넣어줄 것 
    _float fRadian = XMConvertToRadians(m_fCurrentAngle - 180.f);
    if(FAILED(m_pShader->BindRawValue("g_fBurstSkillRadian",&fRadian, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pTextureCom->BindShaderResource(m_pShader, "g_Texture", 0)))
        return E_FAIL;

    if (FAILED(m_pShader->Begin(7)))
        return E_FAIL;

    if (FAILED(m_VIRectCom->BindBuffers()))
        return E_FAIL;

    if (FAILED(m_VIRectCom->Render()))
        return E_FAIL;


    wstring strSkillRate = to_wstring((_int)((m_fCurrentRate) * 100) + 100) + TEXT("%");
    CGameInstance::GetInstance()->RenderFont(TEXT("Font_Number30"), strSkillRate, { g_iWinSizeX * 0.5f - 560.f ,g_iWinSizeY * 0.5f + 10.f }, Colors::White);

    return S_OK;
}

void CUIBurstSkillGauge::SetBurstSkillGauge(_float _fGaugeAmount)
{
    if (m_fCurrentBurstGauge == m_fMaxGauge)
        return;

    if (_fGaugeAmount >= m_fMaxGauge) {
        m_fCurrentBurstGauge = m_fMaxGauge;
        m_pUIEffect->SetEnable(true);
    }
    else {
        m_fCurrentBurstGauge = _fGaugeAmount;
    }

    ComputeUIAngle();
}

void CUIBurstSkillGauge::ComputeUIAngle()
{
    float fGaugeRate = m_fCurrentBurstGauge/ (m_fMaxGauge + (m_fMaxGauge * 0.25f));
    m_fCurrentAngle = 360.f * fGaugeRate;
    m_fCurrentRate = m_fCurrentBurstGauge / m_fMaxGauge;
}

shared_ptr<CUIBurstSkillGauge> CUIBurstSkillGauge::Create()
{
    shared_ptr<CUIBurstSkillGauge> pInstance = make_shared<CUIBurstSkillGauge>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Create to Failed : CUIBurstSkillGauge");

    return pInstance;
}
