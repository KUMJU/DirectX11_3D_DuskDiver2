#include "pch.h"
#include "WorldHPBar.h"
#include "GameInstance.h"

#include "Shader.h"
#include "Texture.h"
#include "VIRect.h"

#include "VIBuffer_UI.h"


CWorldHPBar::CWorldHPBar()
{
}

HRESULT CWorldHPBar::Initialize()
{
    CUI::tagUIInfo UIInfo = {};

    UIInfo.fSizeX = 80.f;
    UIInfo.fSizeY = 5.f;
    UIInfo.fX = 0.f;
    UIInfo.fY = 0.f;

    __super::Initialize(UIInfo, 1);
    CUI::AddBaseComponent();

    m_vOriginSize.x = UIInfo.fSizeX;
    m_vOriginSize.y = UIInfo.fSizeY;

    m_pTextureCom = CGameInstance::GetInstance()->GetTexture(TEXT("hud_img"));
    m_Components.emplace(TEXT("Com_Texture"), m_pTextureCom);

    m_pBackgroundTexture = CGameInstance::GetInstance()->GetTexture(TEXT("hud_enemy_bg"));
    m_Components.emplace(TEXT("Com_BackGroundTexture"), m_pTextureCom);

    m_pBackGroundTransform = CTransform::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), nullptr);
   
    m_pBackGroundTransform->SetState(CTransform::STATE_POSITION, { 0.f ,0.f , 0.f, 1.f });
    m_pBackGroundTransform->SetScaling(UIInfo.fSizeX +20.f, UIInfo.fSizeY + 8.f, 1.f);

    m_pBackGroundBuffer = CVIBuffer_UI::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo());

    m_IsEnabled = false;

    return S_OK;
}

void CWorldHPBar::PriorityTick(_float _fTimeDelta)
{
}

void CWorldHPBar::Tick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;


    if (m_IsHPLerp)
        CalcHPDiff(_fTimeDelta);



}

void CWorldHPBar::LateTick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;


    __super::LateTick(_fTimeDelta);

}

HRESULT CWorldHPBar::Render()
{
    if (!m_IsEnabled)
        return S_OK;

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

    if (FAILED(m_pBackGroundBuffer->BindBuffers()))
        return E_FAIL;

    if (FAILED(m_pBackGroundBuffer->Render()))
        return E_FAIL;


    //체력바
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



 

    return S_OK;
}

void CWorldHPBar::SetHP(_int _iHP)
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

void CWorldHPBar::HPBarReset()
{
    m_iHP = m_iMaxHP;
    m_fPrevHPRatio = 1.f;
    m_fCurrentHPRatio = 1.f;
    m_fCurrentDiff = 0.f;
    m_IsHPLerp = false;
}

void CWorldHPBar::CalcHPDiff(_float _fTimeDelta)
{
    if (m_fPrevHPRatio != m_fCurrentHPRatio) {
        m_fPrevHPRatio -= m_fCurrentDiff* _fTimeDelta;

        if (m_fPrevHPRatio <= m_fCurrentHPRatio) {
            m_fPrevHPRatio = m_fCurrentHPRatio;
            m_IsHPLerp = false;
        }
    }
}

void CWorldHPBar::CalcScreenPos(_fvector _vWorldPos)
{
    _matrix ViewMat = CGameInstance::GetInstance()->GetTransformMatrix(CPipeLine::D3DTS_VIEW);
    _matrix ProjMat = CGameInstance::GetInstance()->GetTransformMatrix(CPipeLine::D3DTS_PROJ);

    _vector vPosition = XMVector3TransformCoord(_vWorldPos, ViewMat);
    vPosition = XMVector3TransformCoord(vPosition, ProjMat);

    _float fScreenXNormal = (vPosition.m128_f32[0] + 1.f) * 0.5f;
    _float fScreenYNormal = (vPosition.m128_f32[1] - 1.f) * -0.5f;
    //월드 공간에서는 위가 + 아래가 -지만 스크린 좌표는 반대이기 때문에 변환을 한번 해줘야하는듯
    fScreenYNormal = 1.f - fScreenYNormal;

    _float fX = g_iWinSizeX * fScreenXNormal - (g_iWinSizeX *0.5f);
    _float fY = g_iWinSizeY * fScreenYNormal -  (g_iWinSizeY * 0.5f);

    m_pTransformCom->SetState(CTransform::STATE_POSITION, XMVectorSet(fX, fY, 0.f, 1.f));
    m_pBackGroundTransform->SetState(CTransform::STATE_POSITION, XMVectorSet(fX, fY, 0.f, 1.f));
}

shared_ptr<CWorldHPBar> CWorldHPBar::Create()
{
    shared_ptr<CWorldHPBar> pInstance = make_shared<CWorldHPBar>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CWorldHPBar");


    return pInstance;
}
