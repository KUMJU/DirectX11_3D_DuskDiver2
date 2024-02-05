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

    UIInfo.fSizeX = 90.f;
    UIInfo.fSizeY = 10.f;
    UIInfo.fX = 0.f;
    UIInfo.fY = 0.f;

    __super::Initialize(UIInfo, 1);
    CUI::AddBaseComponent();

    m_vOriginSize.x = UIInfo.fSizeX;
    m_vOriginSize.y = UIInfo.fSizeY;

    m_pTextureCom = CGameInstance::GetInstance()->GetTexture(TEXT("hud_img"));
    m_Components.emplace(TEXT("Com_Texture"), m_pTextureCom);

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


}

void CWorldHPBar::LateTick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;


    __super::LateTick(_fTimeDelta);

}

HRESULT CWorldHPBar::Render()
{
    if (FAILED(m_pTransformCom->BindShaderResource(m_pShader, "g_WorldMatrix")))
        return E_FAIL;

    _float4x4 ViewMatrix, ProjMatrix;
    XMStoreFloat4x4(&ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

    m_pShader->BindMatrix("g_ViewMatrix", &ViewMatrix);
    m_pShader->BindMatrix("g_ProjMatrix", &ProjMatrix);

    m_pShader->BindRawValue("g_HPRatio", &m_fCurrentHPRatio, sizeof(_float));

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

void CWorldHPBar::SetHP(_int _iHP)
{
    m_iHP = _iHP;
    m_fCurrentHPRatio = m_iHP / m_iMaxHP;
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
    m_pTransformCom->SetScaling(m_vOriginSize.x, m_vOriginSize.y, 1.f);
    //z값에 따라서 스케일링도 해줘야함 
}

shared_ptr<CWorldHPBar> CWorldHPBar::Create()
{
    shared_ptr<CWorldHPBar> pInstance = make_shared<CWorldHPBar>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CWorldHPBar");


    return pInstance;
}
